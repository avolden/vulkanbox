local volk_dest_dir = mg.get_build_dir() .. 'deps/volk/'
local volk_url = 'https://github.com/zeux/volk/archive/refs/heads/master.zip'
if net.download(volk_url, volk_dest_dir) then
	io.write('volk: Updated \'master\' branch', '\n')
else
	io.write('volk: up-to-date\n')
end

-- return {includes = {'deps/' .. volk_dest_dir .. 'include/'}}