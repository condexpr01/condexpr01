#include "core.hpp"

//manager layer
void app(int argc, char **argv){

	if (argc >= 3){
		bool value = std::stoi(argv[2]);
		core::is_offscreen = value;
	}

	if (core::is_offscreen){
		SDL_SetHint(SDL_HINT_VIDEO_DRIVER,"offscreen");
		SDL_SetHint(SDL_HINT_AUDIO_DRIVER,"dummy");
	}

	core::sdl_ctx_manager scm{};
	if(!scm.is_ok()){throw std::runtime_error{scm.what()};}

	core::sdl_window_manager swm{"",1024,1024,SDL_WINDOW_OPENGL
		| SDL_WINDOW_BORDERLESS
		| SDL_WINDOW_TRANSPARENT
		| SDL_WINDOW_RESIZABLE
	};

	if(!swm.is_ok()){throw std::runtime_error{swm.what()};}

	core::sdl_gl_ctx_manager sgcm{swm.window};
	if(!sgcm.is_ok()){throw std::runtime_error{sgcm.what()};}

	core::sdl3_gl3_imgui_ctx_manager sgicm{swm.window,sgcm.gl_ctx};
	if(!sgicm.is_ok()){throw std::runtime_error{sgicm.what()};}

	core::sdl_event_manager sem{swm,sgcm,sgicm};

	//core::set_up_sdl_event_ctx_datas(argc,argv,sem.get_ctx());
	auto &ctx = sem.get_ctx();

	if (argc <= 1){
		SDL_Log("No args provide");
		SDL_Log("args: <png_path> <is_offscreen[0,1]> <profile[alpha, beta]>");
	}

	if (argc >= 2){
		ctx.png_path = argv[1];
	}

	if (argc >= 4){
		if(strcmp(argv[3], "alpha") == 0){
			ctx.page = core::PAGE_PROFILE_ALPHA;
		}else if (strcmp(argv[3], "beta") == 0){
			ctx.page = core::PAGE_PROFILE_BETA;
		}
	}

	ctx.hide_sidebar = core::is_offscreen;

	if (!core::is_offscreen){
		core::sdl_tray_manager stm{swm.window};
	}

	SDL_SetWindowHitTest(sem.get_ctx().swm.window,
		[](SDL_Window *window, const SDL_Point *area, void *data) -> SDL_HitTestResult{
			if (data && *static_cast<bool*>(data)){
				return SDL_HITTEST_DRAGGABLE;
			}else{
				return SDL_HITTEST_NORMAL;
			}
		},
		&sem.get_ctx().window_draggable
	);

	sem.loop();
	if(!sem.is_ok()){throw std::runtime_error{sem.what()};}
}

int main(int argc, char **argv) try{
	app(argc,argv);

	return 0;
}

catch(std::runtime_error &e){
	SDL_Log("%s",e.what());
	return 1;
}

catch(std::exception &e){
	SDL_Log("%s",e.what());
	return 2;
}

