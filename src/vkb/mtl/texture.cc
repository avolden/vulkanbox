#include "texture.hh"
#include "instance.hh"

#include <Metal/MTLDevice.hpp>
#include <Metal/MTLTexture.hpp>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace vkb::mtl
{
	texture::texture(mc::string_view path)
	{
		stbi_set_flip_vertically_on_load(true);

		FILE* f {nullptr};
		if (path.size() < 256)
		{
			char buf[256];
			memcpy(buf, path.data(), path.size());
			buf[path.size()] = '\0';
			f = fopen(buf, "r");
		}
		else
		{
			char* buf = new char[path.size() + 1];
			memcpy(buf, path.data(), path.size());
			buf[path.size()] = '\0';
			f = fopen(buf, "r");
			delete[] buf;
		}

		if (f)
		{
			int32_t  channels;
			uint8_t* buf =
				stbi_load_from_file(f, (int32_t*)&width, (int32_t*)&height, &channels, 4);

			if (buf)
			{
				MTL::TextureDescriptor* desc = MTL::TextureDescriptor::alloc()->init();
				desc->setPixelFormat(MTL::PixelFormatRGBA8Unorm);
				desc->setWidth(width);
				desc->setHeight(height);

				tex = instance::get().get_device()->newTexture(desc);
				MTL::Region reg = MTL::Region(0, 0, 0, width, height, 1);

				tex->replaceRegion(reg, 0, buf, 4 * width);
				desc->release();
				stbi_image_free(buf);
			}
			fclose(f);
		}

		// fopen(const char *filename, const char *mode)
	}

	texture::~texture()
	{
		tex->release();
	}
}