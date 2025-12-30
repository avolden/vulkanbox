#include "cam/orbital.hh"
#include "core/time.hh"
#include "input/input_system.hh"
#include "ui/context.hh"
#include "vk/context.hh"
#include "vk/instance.hh"
#include "vk/material.hh"
#include "vk/material/coordinates.hh"
#include "vk/material/module.hh"
#include "vk/material/sky_sphere.hh"
#include "vk/surface.hh"
#include "win/display.hh"
#include "win/window.hh"

#include "log.hh"
#include "math/math.hh"
#include "math/trig.hh"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#ifdef USE_SUPERLUMINAL
#include <Superluminal/PerformanceAPI.h>
#endif

int main(int argc, char** argv)
{
	bool enable_validation = false;
	if (argc > 1)
		enable_validation = strcmp(argv[1], "--validate") == 0;
	using namespace vkb;

	math::init_random();

	display      disp;
	input_system is;
	window       main_window("main_window", &is);

	vk::instance inst(enable_validation);
	vk::surface  surface(main_window);

	inst.create_device(surface);
	surface.create_swapchain();

	vk::context ctx(main_window, surface);

	cam::orbital cam(is, main_window);
	ui::context  ui_ctx(main_window, is, ctx);

	vk::sky_sphere sky;

	bool running {ctx.created()};
	// vkb::log::assert(running, "Failed to initialize Vulkan context");
	time::stamp last = time::now();

	vk::model::vert verts[] {
		// upper face
		{{-1.0f, 1.0f, 1.0f, 1.0f},   {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
		{{1.0f, 1.0f, 1.0f, 1.0f},    {0.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
		{{-1.0f, -1.0f, 1.0f, 1.0f},  {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
		{{1.0f, -1.0f, 1.0f, 1.0f},   {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
		// bottom face
		{{-1.0f, -1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
		{{1.0f, -1.0f, -1.0f, 1.0f},  {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
		{{-1.0f, 1.0f, -1.0f, 1.0f},  {0.0f, 0.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
		{{1.0f, 1.0f, -1.0f, 1.0f},   {1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
		// front face
		{{-1.0f, -1.0f, 1.0f, 1.0f},  {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
		{{1.0f, -1.0f, 1.0f, 1.0f},   {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
		{{-1.0f, -1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
		{{1.0f, -1.0f, -1.0f, 1.0f},  {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
		// back face
		{{-1.0f, 1.0f, -1.0f, 1.0f},  {0.0f, 0.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
		{{1.0f, 1.0f, -1.0f, 1.0f},   {1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
		{{-1.0f, 1.0f, 1.0f, 1.0f},   {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
		{{1.0f, 1.0f, 1.0f, 1.0f},    {0.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
		// left face
		{{-1.0f, 1.0f, 1.0f, 1.0f},   {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
		{{-1.0f, -1.0f, 1.0f, 1.0f},  {1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
		{{-1.0f, 1.0f, -1.0f, 1.0f},  {0.0f, 0.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
		{{-1.0f, -1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
		// right face
		{{1.0f, -1.0f, 1.0f, 1.0f},   {0.0f, 0.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
		{{1.0f, 1.0f, 1.0f, 1.0f},    {0.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
		{{1.0f, -1.0f, -1.0f, 1.0f},  {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
		{{1.0f, 1.0f, -1.0f, 1.0f},   {1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
	};
	uint16_t idcs[] {
		0,  2,  1,  2,  3,  1,  // upper face
		4,  6,  5,  6,  7,  5,  // bottom face
		8,  10, 9,  10, 11, 9,  // front face
		12, 14, 13, 14, 15, 13, // back face
		16, 18, 17, 18, 19, 17, // left face
		20, 22, 21, 22, 23, 21, // right face
	};

	ctx.set_proj(0.1f, 1000.f, 70.f);

	mc::vector<vk::object> objs;
	objs.reserve(100);

	srand(0);

	vk::model   model;
	vk::texture tex;
	ctx.init_model(model, verts, idcs);
	ctx.init_texture(tex, "res/textures/tex.png");

	vk::module mod(tex);

	vk::coordinates coords;

	// TODO Create a screen space context handling resizing
	auto [w, h] = surface.get_extent();
	mat4 coords_proj = mat4::ortho_proj(-50.f, 50.f, 0, w, h, 0);
	vec2 translate {(75.f * 2 / w), ((h - 75.f) * 2 / h)};

	mc::vector<mat4> modules;
	modules.emplace_back(mat4::scale({.5f, .5f, .5f, 1.f}));
	modules.emplace_back(mat4::scale({.5f, .5f, .5f, 1.f}) *
	                     mat4::translate({0.f, 0.f, 2.f, 1.f}));
	modules.emplace_back(mat4::scale({.5f, .5f, .5f, 1.f}) *
	                     mat4::translate({0.f, 0.f, 4.f, 1.f}));
	modules.emplace_back(mat4::scale({.5f, .5f, .5f, 1.f}) *
	                     mat4::translate({0.f, 2.f, 0.f, 1.f}));
	modules.emplace_back(mat4::scale({.5f, .5f, .5f, 1.f}) *
	                     mat4::translate({2.f, 0.f, 0.f, 1.f}));

	while (running)
	{
#ifdef USE_SUPERLUMINAL
		PerformanceAPI_BeginEvent("Frame", nullptr, PERFORMANCEAPI_DEFAULT_COLOR);
#endif
		time::stamp now = time::now();
		double      dt = time::elapsed_sec(last, now);
		last = now;

		is.clear_transitions();
		disp.update();
		cam.update(dt);

		for (uint32_t i {0}; i < objs.size(); i++)
			objs[i].update(dt);

		if (!main_window.closed() && !main_window.minimized())
		{
			ui_ctx.update(dt);
			if (!ctx.prepare_draw())
				continue;
			sky.prepare_draw(ctx.current_command_buffer(), ctx.current_img_idx(), cam,
			                 ctx.get_proj());
			mod.prepare_draw(ctx.current_command_buffer(), ctx.current_img_idx(), cam,
			                 ctx.get_proj());
			coords.prepare_draw(ctx.current_command_buffer(), ctx.current_img_idx(), cam,
			                    coords_proj, translate);

			ctx.begin_draw();
			sky.draw(ctx.current_command_buffer(), ctx.current_img_idx());
			mod.draw(ctx.current_command_buffer(), ctx.current_img_idx(), model, modules);
			coords.draw(ctx.current_command_buffer(), ctx.current_img_idx());
			ui_ctx.draw();
			if (ctx.present())
			{
				auto [w, h] = surface.get_extent();
				coords_proj = mat4::ortho_proj(-50.f, 50.f, 0, w, h, 0);
				translate = {(75.f * 2 / w), ((h - 75.f) * 2 / h)};
			}
		}

		if (main_window.closed())
			running = false;
#ifdef USE_SUPERLUMINAL
		PerformanceAPI_EndEvent();
#endif
	}

	ctx.wait_completion();

	ctx.destroy_texture(tex);
	ctx.destroy_model(model);

	return 0;
}