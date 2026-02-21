require('mg/helpers')

if mg.need_generate() then
	mg.configurations({'debug', 'release'})
end

imgui = require('deps/imgui')
mincore = require('deps/mincore')
stb = require('deps/stb')
yyjson = require('deps/yyjson')


ext_include_dirs = merge(
	{mg.get_build_dir() .. 'deps/'},
	imgui.includes,
	mincore.includes,
	-- stb.includes
	yyjson.includes
)

platform_define = {}
if (mg.platform() == 'windows') then
	platform_define = {
		'-D"VKB_WINDOWS"',
		'-D"_CRT_SECURE_NO_WARNINGS"',
		'-D"_MT"',
		'-D"_DLL"',
		'-D_DISABLE_STRING_ANNOTATION',
		'-D_DISABLE_VECTOR_ANNOTATION',
	}
elseif (mg.platform() == 'linux') then
	platform_define = {'-D"VKB_LINUX"'}
elseif (mg.platform() == 'mac') then
	platform_define = {'-D"VKB_MAC"'}
else
	error("Unsupported platform")
end

platform_compile_options = {}
if (mg.platform() == 'windows') then
	platform_compile_options = {
		'-Wno-ignored-pragma-intrinsic' -- Warning raised on win32/misc.h pragma intrinsic
	}
end

platform_link_options = {}
if (mg.platform() == 'windows') then
	platform_link_options = {
		'-fuse-ld=lld-link',
		'-Xlinker /incremental:no',
		'-Xlinker /nodefaultlib:libcmt.lib',
		'-Xlinker /defaultlib:msvcrt.lib',
		-- '-lclang_rt.asan_dynamic_runtime_thunk-x86_64.lib',
		-- '-lclang_rt.asan_dynamic-x86_64.lib',
	}
elseif (mg.platform() == 'mac') then
	platform_link_options = {
		'-framework Cocoa',
		'-framework Metal',
		'-framework Foundation',
		'-framework QuartzCore',
	}
end

if (mg.platform() ~= 'mac') then
	vulkan = require('deps/vulkan')
	vma = require('deps/vma')
	volk = require('deps/volk')

	ext_include_dirs = merge(ext_include_dirs, vulkan.includes, vma.includes)
end

platform_deps = {}
if (mg.platform() == 'windows') then
	modular_win32 = require('deps/windows/modular_win32')
	superluminal = require('deps/windows/superluminal')
	if superluminal ~= nil then
		ext_include_dirs = merge(
			ext_include_dirs,
			superluminal.includes,
			modular_win32.includes
		)
		platform_deps = {superluminal.project}
		table.insert(platform_define, '-D"USE_SUPERLUMINAL"')
		table.insert(platform_link_options, '-Xlinker /ignore:4099')
	else
		ext_include_dirs = merge(
			ext_include_dirs,
			modular_win32.includes
		)
	end
elseif (mg.platform() == 'linux') then
	wayland = require('deps/linux/wayland')
	xkb = require('deps/linux/xkbcommon')
	libdecor = require('deps/linux/libdecor')

	ext_include_dirs = merge(
		ext_include_dirs,
		wayland.includes,
		xkb.includes,
		libdecor.includes)
	platform_deps = merge(wayland.projects, xkb.project, libdecor.project)
elseif (mg.platform() == 'mac') then
	mtl = require('deps/mac/metal-cpp')
	ext_include_dirs = merge(ext_include_dirs, mtl.includes)
	platform_deps = {mtl.project}
end

sources = {'src/vkb/**.cc'}
if (mg.platform() == 'mac') then
	sources[2] = 'src/vkb/**.mm';
end

local vkb = mg.project({
	name = 'vulkanbox',
	type = mg.project_type.executable,
	sources = sources,
	includes = {'src/'},
	external_includes = ext_include_dirs,
	compile_options = merge('-g', '-std=c++20', '-Wall', '-Wextra', '-Werror', '-nostdinc++', platform_define, platform_compile_options),
	link_options = merge(platform_link_options, '-g'),
	dependencies = merge(imgui.project, mincore.project, yyjson.project, platform_deps),
	release = {
		compile_options = {'-O2'}
	}
})

remove_platform_sources(vkb)

i = 1
while i <= #vkb.sources do
	if (string.find(vkb.sources[i].file, 'vma') ~= nil) then
		vkb.sources[i].compile_options = string.gsub(vkb.compile_options, ' %-nostdinc%+%+', '')
	end
	if (string.find(vkb.sources[i].file, 'vkb/vk') ~= nil and mg.platform() == 'mac') then
		table.remove(vkb.sources, i)
		i = i - 1
	elseif (string.find(vkb.sources[i].file, 'vkb/mtl') ~= nil and mg.platform() ~= 'mac') then
		table.remove(vkb.sources, i)
		i = i - 1
	end
	i = i + 1
end

projects_to_generate = {vkb}

if (mg.platform() ~= 'mac') then
	slang = require('deps/slang')
	local slangrc = mg.project({
		name = 'slangrc',
		type = mg.project_type.executable,
		sources = {'src/slangrc/**.cc'},
		includes = merge(ext_include_dirs, slang.includes),
		compile_options = merge('-g', '-std=c++20', '-Wall', '-Wextra', '-Werror', platform_define, platform_compile_options),
		link_options = merge('-g', platform_link_options),
		dependencies = merge(slang.project, mincore.project),
		release = {
			compile_options = {'-O2'}
		}
	})
	table.insert(projects_to_generate, slangrc)

	remove_platform_sources(slangrc)

	slangrc_ext = ''
	if (mg.platform() == 'windows') then
		slangrc_ext = '.exe'
	end
	slangrc_bin = '"' .. mg.get_build_dir() .. 'bin/slangrc' .. slangrc_ext .. '"'
	shaders = mg.collect_files('res/shaders/*.slang')
	for i=1,#shaders do
		spirv = mg.get_build_dir() .. 'bin/' .. string.gsub(shaders[i], '.slang', '.spv')
		mg.add_post_build_cmd(slangrc, {
			input = shaders[i],
			output = spirv,
			cmd = slangrc_bin .. ' ${in} ${out}'
		})
	end

else
	shaders = mg.collect_files('res/shaders/metal/*.metal')
	for i=1,#shaders do
		mtl_lib = mg.get_build_dir() .. 'bin/' .. string.gsub(shaders[i], '%.metal', '.metallib')
		mg.add_post_build_cmd(vkb, {
			input = shaders[i],
			output = mtl_lib,
			cmd = 'xcrun -sdk macosx metal -frecord-sources -gline-tables-only ${in} -o ${out}'
		})
	end
end
-- Textures
textures = mg.collect_files('res/textures/*.png')
for i=1,#textures do
	mg.add_post_build_copy(vkb, {
		input = textures[i],
		output = mg.get_build_dir() .. 'bin/' .. textures[i];
	})
end

if mg.need_generate() then
	mg.generate(projects_to_generate)
end