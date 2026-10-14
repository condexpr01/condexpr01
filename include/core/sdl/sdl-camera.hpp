#ifndef CORE_HEADER_SDL_CAMERA_GUARD
#define CORE_HEADER_SDL_CAMERA_GUARD

#include <SDL3/SDL.h>
#include <string>
#include "sdl-video.hpp"

namespace core{

	class capture_device{
		//error status
		private:
			bool status = false;
			std::string reason;

		//vars
		private:
			SDL_Camera *cur_capt = nullptr;
			SDL_Surface *frame = nullptr;
			Uint64 timestamp_ns = 0;
			SDL_CameraID *capt_ids = nullptr;
			int capture_device_num{};


		//get error status methods
		public:
			bool is_ok() noexcept{return status;}
			const char *what() noexcept{return reason.c_str();}

		//methods
		public:
			void detect_capture() noexcept{
				if(!status)return;

				SDL_free(capt_ids);capt_ids = nullptr;

				capt_ids = SDL_GetCameras(&capture_device_num);
				if(!capt_ids){
					status=false;reason=SDL_GetError();return;
				}
			}

			int get_capture_num() noexcept {return capture_device_num;}

			SDL_CameraID get_capture_id(int index) noexcept{
				if(index >=0 && index < capture_device_num){
					return capt_ids[index];
				}

				return 0;//out of range
			}

			SDL_CameraID get_current_capture_id() noexcept{
				if(cur_capt){
					return SDL_GetCameraID(cur_capt);
				}else{
					return 0;
				}
			}

			void open_capture(int index, const SDL_CameraSpec *spec = nullptr) noexcept{
				if(!status)return;

				if(frame)release_frame();
				timestamp_ns = 0;
				if(cur_capt)SDL_CloseCamera(cur_capt);

				if(index >=0 && index < capture_device_num){
					cur_capt = SDL_OpenCamera(capt_ids[index],spec);
					if(!cur_capt){
						status=false;reason=SDL_GetError();return;
					}
				}
			}

			void open_capture(SDL_CameraID c_id, const SDL_CameraSpec *spec = nullptr) noexcept{
				if(!status)return;

				if(frame)release_frame();
				timestamp_ns = 0;
				if(cur_capt)SDL_CloseCamera(cur_capt);

				cur_capt = SDL_OpenCamera(c_id,spec);
				if(!cur_capt){
					status=false;reason=SDL_GetError();return;
				}
			}

			void close_capture() noexcept{
				if(cur_capt)SDL_CloseCamera(cur_capt);

				cur_capt = nullptr;
			}

			//acquire frame
			void acquire_frame() noexcept{
				if(cur_capt){
					frame = SDL_AcquireCameraFrame(cur_capt,&timestamp_ns);
				}
			}

			const Uint64 get_timestamp_ns() noexcept{
				return timestamp_ns;
			}

			//return frame
			SDL_Surface *get_frame() noexcept{
				return frame;
			}

			//if frame is not nullptr, release_frame is requred before set_capture
			void release_frame() noexcept{
				if(frame)SDL_ReleaseCameraFrame(cur_capt,frame);
			}

			SDL_CameraPermissionState get_permission() noexcept{
				return SDL_GetCameraPermissionState(cur_capt);
			}

			SDL_CameraPosition get_pos() noexcept{
				return SDL_GetCameraPosition(get_current_capture_id());
			}

			bool get_fmt(SDL_CameraSpec *spec) noexcept{
				return SDL_GetCameraFormat(cur_capt,spec);
			}


		//RAII
		public:
			capture_device() noexcept{
				if (!(SDL_WasInit(SDL_INIT_CAMERA) & SDL_INIT_CAMERA)){
					if (!SDL_Init(SDL_INIT_CAMERA)){
						status=false;reason = SDL_GetError();return;
					}
				}

				status = true;
			}

			capture_device(capture_device  &other) = delete;
			capture_device(capture_device &&other) = delete;
			capture_device &operator=(capture_device  &other) = delete;
			capture_device &operator=(capture_device &&other) = delete;

			~capture_device() noexcept{
				if(cur_capt)SDL_CloseCamera(cur_capt);
				if(capt_ids)SDL_free(capt_ids);
			}
	};


	inline const char* get_name_sdl_color_space(SDL_Colorspace cs) noexcept{
		switch(cs){
			case SDL_COLORSPACE_UNKNOWN: return "SDL_COLORSPACE_UNKNOWN";
			case SDL_COLORSPACE_SRGB: return "SDL_COLORSPACE_SRGB";
			case SDL_COLORSPACE_SRGB_LINEAR: return "SDL_COLORSPACE_SRGB_LINEAR";
			case SDL_COLORSPACE_HDR10: return "SDL_COLORSPACE_HDR10";
			case SDL_COLORSPACE_JPEG: return "SDL_COLORSPACE_JPEG";
			case SDL_COLORSPACE_BT601_LIMITED: return "SDL_COLORSPACE_BT601_LIMITED";
			case SDL_COLORSPACE_BT601_FULL: return "SDL_COLORSPACE_BT601_FULL";
			case SDL_COLORSPACE_BT709_LIMITED: return "SDL_COLORSPACE_BT709_LIMITED";
			case SDL_COLORSPACE_BT709_FULL: return "SDL_COLORSPACE_BT709_FULL";
			case SDL_COLORSPACE_BT2020_LIMITED: return "SDL_COLORSPACE_BT2020_LIMITED";
			case SDL_COLORSPACE_BT2020_FULL: return "SDL_COLORSPACE_BT2020_FULL";
			default: return "";//Unknown
		}
	}

	inline const char *get_name_sdl_camera_permission_state(SDL_CameraPermissionState s) noexcept{
		switch(s){
			case SDL_CAMERA_PERMISSION_STATE_DENIED:return "SDL_CAMERA_PERMISSION_STATE_DENIED";
			case SDL_CAMERA_PERMISSION_STATE_PENDING:return "SDL_CAMERA_PERMISSION_STATE_PENDING";
			case SDL_CAMERA_PERMISSION_STATE_APPROVED:return "SDL_CAMERA_PERMISSION_STATE_APPROVED";
			default: return "";//Unknown
		}
	}

	inline const char *get_name_sdl_camera_position(SDL_CameraPosition s) noexcept{
		switch(s){
			case SDL_CAMERA_POSITION_UNKNOWN: return "SDL_CAMERA_POSITION_UNKNOWN";
			case SDL_CAMERA_POSITION_FRONT_FACING: return "SDL_CAMERA_POSITION_FRONT_FACING";
			case SDL_CAMERA_POSITION_BACK_FACING: return "SDL_CAMERA_POSITION_BACK_FACING";
			default: return "";//Unknown
		}

	}

	template <typename LogT>
	void log_sdl_camera_spec(SDL_CameraSpec cs, LogT &&Log = SDL_Log) noexcept{
		Log("[log_sdl_camera_spec]\nw:%d h:%d fps:%d/%d=%d\npix_fmt:%s color_spece:%s",
			cs.width,cs.height,
			cs.framerate_numerator,cs.framerate_denominator,
			cs.framerate_numerator/cs.framerate_denominator,
			get_name_sdl_pixel_format(cs.format),
			get_name_sdl_color_space(cs.colorspace)
			);
	}

	template <typename LogT>
	inline void log_sdl_camera_spec(SDL_CameraID c_id, LogT &&Log = SDL_Log) noexcept{
		int count{};
		SDL_CameraSpec **cs = SDL_GetCameraSupportedFormats(c_id,&count);
		if (cs == nullptr)return;
		
		for(int i=0;i<count;i++){
			Log("[log_sdl_camera] name: %s count: %d",SDL_GetCameraName(c_id),i);
			log_sdl_camera_spec(*cs[i], Log);
		}

		SDL_free(cs);
	}

	template <typename LogT>
	void log_sdl_camera_permission(SDL_Camera *camera, LogT &&Log = SDL_Log) noexcept{
		SDL_CameraPermissionState s = SDL_GetCameraPermissionState(camera);
		Log("%s",get_name_sdl_camera_permission_state(s));
	}

	template <typename LogT>
	void log_sdl_camera_drivers(LogT &&Log = SDL_Log) noexcept{
		int num = SDL_GetNumCameraDrivers();

		for (int i=0; i<num ;i++){
			Log("[log_sdl_camera_drivers] %s",SDL_GetCameraDriver(i));
		}

		Log("[log_sdl_camera_drivers] current: %s",SDL_GetCurrentCameraDriver());
	}

	template <typename LogT>
	void log_sdl_camera(LogT &&Log = SDL_Log) noexcept{
		int count{};
		SDL_CameraID *c_ids = SDL_GetCameras(&count);
		if (c_ids == nullptr)return;
		
		for(int i=0;i<count;i++){
			log_sdl_camera_spec(c_ids[i], Log);
		}

		SDL_free(c_ids);
	}

}

#endif
