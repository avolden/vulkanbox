-- local mtl_dest_dir = mg.get_build_dir() .. 'deps/metal-cpp/'
-- local mtl_url = 'https://github.com/bkaradzic/metal-cpp/archive/refs/heads/metal-cpp_26.zip'
-- if net.download(mtl_url, mtl_dest_dir) then
-- 	io.write('metal-cpp: Updated \'metal-cpp_26\' branch\n')
-- else
-- 	io.write('metal-cpp: up-to-date\n')
-- end

local mtl_cpp = mg.project({
	name = "metal-cpp",
	type = mg.project_type.sources,
	sources = {'metal-cpp.cc'},
	includes = {'metal-cpp/'},
	compile_options = {'-g', '--std=c++17', '-nostdinc++'}
})

return {project = mtl_cpp, includes = {'deps/mac/metal-cpp'}}