#ifndef SDL_VIDEO_DOT_CPP
#define SDL_VIDEO_DOT_CPP

#include <SDL3/SDL.h>
#include <ostream>
#include <unordered_map>
#include <array>

namespace{

	inline static const auto& get_window_flags_table() noexcept{
		static const std::array arr = {
			std::pair{SDL_WINDOW_FULLSCREEN,"SDL_WINDOW_FULLSCREEN"},
			std::pair{SDL_WINDOW_OPENGL,"SDL_WINDOW_OPENGL"},
			std::pair{SDL_WINDOW_OCCLUDED,"SDL_WINDOW_OCCLUDED"},
			std::pair{SDL_WINDOW_HIDDEN,"SDL_WINDOW_HIDDEN"},
			std::pair{SDL_WINDOW_BORDERLESS,"SDL_WINDOW_BORDERLESS"},
			std::pair{SDL_WINDOW_RESIZABLE,"SDL_WINDOW_RESIZABLE"},
			std::pair{SDL_WINDOW_MINIMIZED,"SDL_WINDOW_MINIMIZED"},
			std::pair{SDL_WINDOW_MAXIMIZED,"SDL_WINDOW_MAXIMIZED"},
			std::pair{SDL_WINDOW_MOUSE_GRABBED,"SDL_WINDOW_MOUSE_GRABBED"},
			std::pair{SDL_WINDOW_INPUT_FOCUS,"SDL_WINDOW_INPUT_FOCUS"},
			std::pair{SDL_WINDOW_MOUSE_FOCUS,"SDL_WINDOW_MOUSE_FOCUS"},
			std::pair{SDL_WINDOW_EXTERNAL,"SDL_WINDOW_EXTERNAL"},
			std::pair{SDL_WINDOW_MODAL,"SDL_WINDOW_MODAL"},
			std::pair{SDL_WINDOW_HIGH_PIXEL_DENSITY,"SDL_WINDOW_HIGH_PIXEL_DENSITY"},
			std::pair{SDL_WINDOW_MOUSE_CAPTURE,"SDL_WINDOW_MOUSE_CAPTURE"},
			std::pair{SDL_WINDOW_MOUSE_RELATIVE_MODE,"SDL_WINDOW_MOUSE_RELATIVE_MODE"},
			std::pair{SDL_WINDOW_ALWAYS_ON_TOP,"SDL_WINDOW_ALWAYS_ON_TOP"},
			std::pair{SDL_WINDOW_UTILITY,"SDL_WINDOW_UTILITY"},
			std::pair{SDL_WINDOW_TOOLTIP,"SDL_WINDOW_TOOLTIP"},
			std::pair{SDL_WINDOW_POPUP_MENU,"SDL_WINDOW_POPUP_MENU"},
			std::pair{SDL_WINDOW_KEYBOARD_GRABBED,"SDL_WINDOW_KEYBOARD_GRABBED"},
			std::pair{SDL_WINDOW_FILL_DOCUMENT,"SDL_WINDOW_FILL_DOCUMENT"},
			std::pair{SDL_WINDOW_VULKAN,"SDL_WINDOW_VULKAN"},
			std::pair{SDL_WINDOW_METAL,"SDL_WINDOW_METAL"},
			std::pair{SDL_WINDOW_TRANSPARENT,"SDL_WINDOW_TRANSPARENT"},
			std::pair{SDL_WINDOW_NOT_FOCUSABLE,"SDL_WINDOW_NOT_FOCUSABLE"}
		};

		return arr;
	}

}

namespace core{


	inline const char* get_name_sdl_system_theme(void){

		SDL_SystemTheme theme = SDL_GetSystemTheme();

		switch (theme){
			case SDL_SYSTEM_THEME_DARK:
				return "SDL_SYSTEM_THEME_DARK";
			case SDL_SYSTEM_THEME_LIGHT:
				return "SDL_SYSTEM_THEME_LIGHT";
			case SDL_SYSTEM_THEME_UNKNOWN:
				return "SDL_SYSTEM_THEME_UNKNOWN";
			default:
				return nullptr;
		}
		
	}


	inline const char *get_name_sdl_pixel_format(SDL_PixelFormat pfmt) noexcept{
		
		std::unordered_map<SDL_PixelFormat,const char*> v = {
			{SDL_PIXELFORMAT_UNKNOWN,"SDL_PIXELFORMAT_UNKNOWN"},
			{SDL_PIXELFORMAT_INDEX1LSB,"SDL_PIXELFORMAT_INDEX1LSB"},
			{SDL_PIXELFORMAT_INDEX1MSB,"SDL_PIXELFORMAT_INDEX1MSB"},
			{SDL_PIXELFORMAT_INDEX2LSB,"SDL_PIXELFORMAT_INDEX2LSB"},
			{SDL_PIXELFORMAT_INDEX2MSB,"SDL_PIXELFORMAT_INDEX2MSB"},
			{SDL_PIXELFORMAT_INDEX4LSB,"SDL_PIXELFORMAT_INDEX4LSB"},
			{SDL_PIXELFORMAT_INDEX4MSB,"SDL_PIXELFORMAT_INDEX4MSB"},
			{SDL_PIXELFORMAT_INDEX8,"SDL_PIXELFORMAT_INDEX8"},
			{SDL_PIXELFORMAT_RGB332,"SDL_PIXELFORMAT_RGB332"},
			{SDL_PIXELFORMAT_XRGB4444,"SDL_PIXELFORMAT_XRGB4444"},
			{SDL_PIXELFORMAT_XBGR4444,"SDL_PIXELFORMAT_XBGR4444"},
			{SDL_PIXELFORMAT_XRGB1555,"SDL_PIXELFORMAT_XRGB1555"},
			{SDL_PIXELFORMAT_XBGR1555,"SDL_PIXELFORMAT_XBGR1555"},
			{SDL_PIXELFORMAT_ARGB4444,"SDL_PIXELFORMAT_ARGB4444"},
			{SDL_PIXELFORMAT_RGBA4444,"SDL_PIXELFORMAT_RGBA4444"},
			{SDL_PIXELFORMAT_ABGR4444,"SDL_PIXELFORMAT_ABGR4444"},
			{SDL_PIXELFORMAT_BGRA4444,"SDL_PIXELFORMAT_BGRA4444"},
			{SDL_PIXELFORMAT_ARGB1555,"SDL_PIXELFORMAT_ARGB1555"},
			{SDL_PIXELFORMAT_RGBA5551,"SDL_PIXELFORMAT_RGBA5551"},
			{SDL_PIXELFORMAT_ABGR1555,"SDL_PIXELFORMAT_ABGR1555"},
			{SDL_PIXELFORMAT_BGRA5551,"SDL_PIXELFORMAT_BGRA5551"},
			{SDL_PIXELFORMAT_RGB565,"SDL_PIXELFORMAT_RGB565"},
			{SDL_PIXELFORMAT_BGR565,"SDL_PIXELFORMAT_BGR565"},
			{SDL_PIXELFORMAT_RGB24,"SDL_PIXELFORMAT_RGB24"},
			{SDL_PIXELFORMAT_BGR24,"SDL_PIXELFORMAT_BGR24"},
			{SDL_PIXELFORMAT_XRGB8888,"SDL_PIXELFORMAT_XRGB8888"},
			{SDL_PIXELFORMAT_RGBX8888,"SDL_PIXELFORMAT_RGBX8888"},
			{SDL_PIXELFORMAT_XBGR8888,"SDL_PIXELFORMAT_XBGR8888"},
			{SDL_PIXELFORMAT_BGRX8888,"SDL_PIXELFORMAT_BGRX8888"},
			{SDL_PIXELFORMAT_ARGB8888,"SDL_PIXELFORMAT_ARGB8888"},
			{SDL_PIXELFORMAT_RGBA8888,"SDL_PIXELFORMAT_RGBA8888"},
			{SDL_PIXELFORMAT_ABGR8888,"SDL_PIXELFORMAT_ABGR8888"},
			{SDL_PIXELFORMAT_BGRA8888,"SDL_PIXELFORMAT_BGRA8888"},
			{SDL_PIXELFORMAT_XRGB2101010,"SDL_PIXELFORMAT_XRGB2101010"},
			{SDL_PIXELFORMAT_XBGR2101010,"SDL_PIXELFORMAT_XBGR2101010"},
			{SDL_PIXELFORMAT_ARGB2101010,"SDL_PIXELFORMAT_ARGB2101010"},
			{SDL_PIXELFORMAT_ABGR2101010,"SDL_PIXELFORMAT_ABGR2101010"},
			{SDL_PIXELFORMAT_RGB48,"SDL_PIXELFORMAT_RGB48"},
			{SDL_PIXELFORMAT_BGR48,"SDL_PIXELFORMAT_BGR48"},
			{SDL_PIXELFORMAT_RGBA64,"SDL_PIXELFORMAT_RGBA64"},
			{SDL_PIXELFORMAT_ARGB64,"SDL_PIXELFORMAT_ARGB64"},
			{SDL_PIXELFORMAT_BGRA64,"SDL_PIXELFORMAT_BGRA64"},
			{SDL_PIXELFORMAT_ABGR64,"SDL_PIXELFORMAT_ABGR64"},
			{SDL_PIXELFORMAT_RGB48_FLOAT,"SDL_PIXELFORMAT_RGB48_FLOAT"},
			{SDL_PIXELFORMAT_BGR48_FLOAT,"SDL_PIXELFORMAT_BGR48_FLOAT"},
			{SDL_PIXELFORMAT_RGBA64_FLOAT,"SDL_PIXELFORMAT_RGBA64_FLOAT"},
			{SDL_PIXELFORMAT_ARGB64_FLOAT,"SDL_PIXELFORMAT_ARGB64_FLOAT"},
			{SDL_PIXELFORMAT_BGRA64_FLOAT,"SDL_PIXELFORMAT_BGRA64_FLOAT"},
			{SDL_PIXELFORMAT_ABGR64_FLOAT,"SDL_PIXELFORMAT_ABGR64_FLOAT"},
			{SDL_PIXELFORMAT_RGB96_FLOAT,"SDL_PIXELFORMAT_RGB96_FLOAT"},
			{SDL_PIXELFORMAT_BGR96_FLOAT,"SDL_PIXELFORMAT_BGR96_FLOAT"},
			{SDL_PIXELFORMAT_RGBA128_FLOAT,"SDL_PIXELFORMAT_RGBA128_FLOAT"},
			{SDL_PIXELFORMAT_ARGB128_FLOAT,"SDL_PIXELFORMAT_ARGB128_FLOAT"},
			{SDL_PIXELFORMAT_BGRA128_FLOAT,"SDL_PIXELFORMAT_BGRA128_FLOAT"},
			{SDL_PIXELFORMAT_ABGR128_FLOAT,"SDL_PIXELFORMAT_ABGR128_FLOAT"},
			{SDL_PIXELFORMAT_YV12,"SDL_PIXELFORMAT_YV12"},
			{SDL_PIXELFORMAT_IYUV,"SDL_PIXELFORMAT_IYUV"},
			{SDL_PIXELFORMAT_YUY2,"SDL_PIXELFORMAT_YUY2"},
			{SDL_PIXELFORMAT_UYVY,"SDL_PIXELFORMAT_UYVY"},
			{SDL_PIXELFORMAT_YVYU,"SDL_PIXELFORMAT_YVYU"},
			{SDL_PIXELFORMAT_NV12,"SDL_PIXELFORMAT_NV12"},
			{SDL_PIXELFORMAT_NV21,"SDL_PIXELFORMAT_NV21"},
			{SDL_PIXELFORMAT_P010,"SDL_PIXELFORMAT_P010"},
			{SDL_PIXELFORMAT_EXTERNAL_OES,"SDL_PIXELFORMAT_EXTERNAL_OES"},
			{SDL_PIXELFORMAT_MJPG,"SDL_PIXELFORMAT_MJPG"},
		};

		if (v.find(pfmt) != v.end()){
			return v[pfmt];
		}else{
			return nullptr;
		}
	}

	inline const char *get_name_sdl_orientation(SDL_DisplayOrientation orientation){
		switch (orientation){
			case SDL_ORIENTATION_UNKNOWN:
				return "SDL_ORIENTATION_UNKNOWN";

			case SDL_ORIENTATION_LANDSCAPE:
				return "SDL_ORIENTATION_LANDSCAPE";

			case SDL_ORIENTATION_LANDSCAPE_FLIPPED:
				return "SDL_ORIENTATION_LANDSCAPE_FLIPPED";

			case SDL_ORIENTATION_PORTRAIT:
				return "SDL_ORIENTATION_PORTRAIT";

			case SDL_ORIENTATION_PORTRAIT_FLIPPED:
				return "SDL_ORIENTATION_PORTRAIT_FLIPPED";
			
			default:return nullptr;
		}
	}


	//not flags rather than a particular flag
	inline const char *get_name_sdl_window_flag(SDL_WindowFlags flag){
		auto arr = get_window_flags_table();
		std::unordered_map<SDL_WindowFlags, const char*> v(arr.begin(),arr.end(),arr.size());

		if (v.find(flag) != v.end()){
			return v[flag];
		}else{
			return nullptr;
		}

	}

	template<typename LogT>
	void ofstream_sdl_get_window_iccprofile(SDL_Window *window, std::ostream &ofs, LogT &&Log = SDL_Log){
		size_t size = 0;
		void* iccprofile = SDL_GetWindowICCProfile(window,&size);

		if (iccprofile != nullptr){
			ofs.write(static_cast<const char*>(iccprofile),size);
		}else{
			Log("[ofstream_sdl_get_window_iccprofile]empty (size:%ld)",size);
		}

		SDL_free(iccprofile);
	}

	template<typename LogT>
	void log_sdl_num_video_drivers(LogT &&Log = SDL_Log){
		Log("[log_sdl_num_video_drivers] %d",SDL_GetNumVideoDrivers());
	}

	template<typename LogT>
	void log_sdl_video_drivers(LogT &&Log = SDL_Log){
		int num = SDL_GetNumVideoDrivers();
		int index = 0;
		while(index < num){
			Log("[log_sdl_video_drivers] %s",SDL_GetVideoDriver(index));
			index++;
		}
	}

	template<typename LogT>
	void log_sdl_system_theme(LogT &&Log = SDL_Log){
		Log("[log_sdl_system_theme] %s",get_name_sdl_system_theme());
	}

	template<typename LogT>
	void log_sdl_orientation(SDL_DisplayOrientation orientation,LogT &&Log = SDL_Log){
		Log("[log_sdl_orientation] %s",get_name_sdl_orientation(orientation));
	}

	template<typename LogT>
	void log_sdl_display_mode(SDL_DisplayMode m,LogT &&Log = SDL_Log){

		Log("[log_sdl_display_mode] displayID:%d",m.displayID);
		Log("[log_sdl_display_mode] format:%s",get_name_sdl_pixel_format(m.format));
		Log("[log_sdl_display_mode] w:%d, h:%d",m.w,m.h);
		Log("[log_sdl_display_mode] pixel_density:%f",m.pixel_density);
		Log("[log_sdl_display_mode] refresh_rate:%f",m.refresh_rate);
		Log("[log_sdl_display_mode] refresh_rate_numberator:%d",m.refresh_rate_numerator);
		Log("[log_sdl_display_mode] refresh_rate_denominator:%d",m.refresh_rate_denominator);

	}

	template<typename LogT>
	void log_sdl_fullscreen_display_modes(SDL_DisplayID id, LogT &&Log = SDL_Log){
		int count = 0;
		SDL_DisplayMode **modes = SDL_GetFullscreenDisplayModes(id,&count);

		if (modes != nullptr){
			for (int begin = 0; begin < count ; begin++){
				log_sdl_display_mode(*(*modes + begin), Log);
			}
		}

		SDL_free(modes);
	}

	template<typename LogT>
	void log_sdl_displayid_info(SDL_DisplayID id, LogT &&Log = SDL_Log){

		SDL_Rect rect{};
		SDL_Rect usable_rect{};
		
		SDL_GetDisplayBounds(id, &rect);
		SDL_GetDisplayUsableBounds(id, &usable_rect);

		Log("[log_sdl_displayid_info] (id:%d):(name) %s",id,SDL_GetDisplayName(id));
		
		Log("[log_sdl_displayid_info] (id:%d):(bounds) x:%d, y:%d, w:%d, h:%d",
			id,rect.x,rect.y,rect.w,rect.h);

		Log("[log_sdl_displayid_info] (id:%d):(usable_bounds) x:%d, y:%d, w:%d, h:%d",
			id,usable_rect.x,usable_rect.y,usable_rect.w,usable_rect.h);

		Log("[log_sdl_displayid_info] (id:%d):(natural orientation) %s",
			id, get_name_sdl_orientation(SDL_GetNaturalDisplayOrientation(id)));

		Log("[log_sdl_displayid_info] (id:%d):(current orientation) %s",
			id, get_name_sdl_orientation(SDL_GetCurrentDisplayOrientation(id)));

		Log("[log_sdl_displayid_info] (id:%d):(content scale) %f",
			id, SDL_GetDisplayContentScale(id));
		
		Log("[log_sdl_displayid_info] (id:%d):(DesktopDisplayMode)",id);
		log_sdl_display_mode(*SDL_GetDesktopDisplayMode(id), Log);

		Log("[log_sdl_displayid_info] (id:%d):(CurrentDisplayMode)",id);
		log_sdl_display_mode(*SDL_GetCurrentDisplayMode(id), Log);

	}

	template<typename LogT>
	void log_sdl_displays(LogT &&Log = SDL_Log){
		int count = 0;

		SDL_DisplayID *d_id = SDL_GetDisplays(&count);
		Log("[log_sdl_displays] count:%d",count);

		int begin = 0;
		while (begin < count){
			log_sdl_displayid_info(d_id[begin], Log);
			begin++;
		}

		SDL_free(d_id);
	}

	template<typename LogT>
	void log_sdl_primary_display(LogT &&Log = SDL_Log){
		log_sdl_displayid_info(SDL_GetPrimaryDisplay(), Log);
	}


	template<typename LogT>
	void log_sdl_window_id(SDL_Window *window, LogT &&Log = SDL_Log){
		Log("[log_sdl_window_id] %d",SDL_GetWindowID(window));
	}

	template<typename LogT>
	void log_sdl_window_pixel_format(SDL_Window *window, LogT &&Log = SDL_Log){
		Log("[log_sdl_window_pixel_format] %s",get_name_sdl_pixel_format(SDL_GetWindowPixelFormat(window)));
	}

	template<typename LogT>
	void log_sdl_window_flags(SDL_WindowFlags flags, LogT &&Log = SDL_Log){
		auto arr = get_window_flags_table();

		for (auto &&k :arr){
			if (k.first & flags){
				Log("[log_sdl_window_flags] %s",k.second);
			}
		}
	}

	template<typename LogT>
	void log_sdl_window_size_and_position(SDL_Window *window, LogT &&Log = SDL_Log){
		int x=0,y=0,w=0,h=0;
		if (SDL_GetWindowSize(window,&w,&h) && SDL_GetWindowPosition(window,&x,&y)){
			Log("[log_sdl_window_size_and_position] x:%d, y:%d, w:%d, h:%d",x,y,w,h);
		}
	}

	template<typename LogT>
	void log_sdl_window_safe_area(SDL_Window *window, LogT &&Log = SDL_Log){
		SDL_Rect rect{};
		if(SDL_GetWindowSafeArea(window,&rect)){
			Log("[log_sdl_window_safe_area] x:%d y:%d w:%d h:%d",rect.x,rect.y,rect.w,rect.h);
		}
	}

	template<typename LogT>
	void log_sdl_windows(LogT &&Log = SDL_Log){
		int count = 0;
		SDL_Window **windows = SDL_GetWindows(&count);
		if (windows != nullptr){
			for (int begin = 0;begin < count; begin ++){
				log_sdl_window_id(*(windows + begin), Log);
				log_sdl_window_pixel_format(*(windows + begin), Log);
				log_sdl_window_flags(SDL_GetWindowFlags(*(windows + begin)), Log);
				log_sdl_window_size_and_position(*(windows + begin), Log);
				log_sdl_window_safe_area(*(windows + begin), Log);
			}
		}
	}

}

#endif
