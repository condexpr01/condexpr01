#ifndef CORE_HEADER_SDL_INIT_GUARD
#define CORE_HEADER_SDL_INIT_GUARD

#include <SDL3/SDL.h>
#include <unordered_map>

namespace core{

	//init_flags
	template<typename LogT>
	void log_sdl_init_flags(LogT &&Log = SDL_Log){

		bool audio = false;
		bool video = false;
		bool joystick = false;
		bool haptic = false;
		bool gamepad = false;
		bool events = false;
		bool sensor = false;
		bool camera = false;

		if(SDL_WasInit(SDL_INIT_AUDIO))audio = true;
		if(SDL_WasInit(SDL_INIT_VIDEO))video = true;
		if(SDL_WasInit(SDL_INIT_JOYSTICK))joystick = true;
		if(SDL_WasInit(SDL_INIT_HAPTIC))haptic = true;
		if(SDL_WasInit(SDL_INIT_GAMEPAD))gamepad = true;
		if(SDL_WasInit(SDL_INIT_EVENTS))events = true;
		if(SDL_WasInit(SDL_INIT_SENSOR))sensor = true;
		if(SDL_WasInit(SDL_INIT_CAMERA))camera = true;

		if(audio)Log("[log_sdl_init_flags] SDL_INIT_AUDIO");
		if(video)Log("[log_sdl_init_flags] SDL_INIT_VIDEO");
		if(joystick)Log("[log_sdl_init_flags] SDL_INIT_JOYSTICK");
		if(haptic)Log("[log_sdl_init_flags] SDL_INIT_HAPTIC");
		if(gamepad)Log("[log_sdl_init_flags] SDL_INIT_GAMEPAD");
		if(events)Log("[log_sdl_init_flags] SDL_INIT_EVENTS");
		if(sensor)Log("[log_sdl_init_flags] SDL_INIT_SENSOR");
		if(camera)Log("[log_sdl_init_flags] SDL_INIT_CAMERA");
	}

	template<typename LogT>
	void log_sdl_is_main_thread(LogT &&Log = SDL_Log){
		if (SDL_IsMainThread()){
			Log("[log_sdl_is_main_thread] true");
		}else{
			Log("[log_sdl_is_main_thread] false");
		}
	}
	
	//a preset, app_meta_data
	inline void set_sdl_app_metadata_preset(void){
		const char* appname = "appname";
		const char* appversion = "appversion";
		const char* appid = "appid";
		SDL_SetAppMetadata(appname,appversion,appid);
	}
	
	//a preset, app_meta_data_properties
	//和set_sdl_app_metadata_preset有相交字段
	inline void set_sdl_app_metadata_properties_preset(void){
		const char* name = "name";
		const char* version = "version";
		const char* identifier = "id";
		const char* creator = "creator";
		const char* copyright = "copyright";
		const char* url = "url";
		const char* type = "type";

		SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING,name);
		SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_VERSION_STRING,version);
		SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_IDENTIFIER_STRING,identifier);
		SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_CREATOR_STRING,creator);
		SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_COPYRIGHT_STRING,copyright);
		SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_URL_STRING,url);
		SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_TYPE_STRING,type);
	}
	
	//app_metadata_properties
	template <typename LogT>
	void log_sdl_app_metadata_properties(LogT &&Log = SDL_Log){
		const char* name=SDL_GetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING);
		const char* version=SDL_GetAppMetadataProperty(SDL_PROP_APP_METADATA_VERSION_STRING);
		const char* identifier=SDL_GetAppMetadataProperty(SDL_PROP_APP_METADATA_IDENTIFIER_STRING);
		const char* creator=SDL_GetAppMetadataProperty(SDL_PROP_APP_METADATA_CREATOR_STRING);
		const char* copyright=SDL_GetAppMetadataProperty(SDL_PROP_APP_METADATA_COPYRIGHT_STRING);
		const char* url=SDL_GetAppMetadataProperty(SDL_PROP_APP_METADATA_URL_STRING);
		const char* type=SDL_GetAppMetadataProperty(SDL_PROP_APP_METADATA_TYPE_STRING);

		Log("[log_sdl_app_metadata_properties] name:%s",name);
		Log("[log_sdl_app_metadata_properties] version:%s",version);
		Log("[log_sdl_app_metadata_properties] identifier:%s",identifier);
		Log("[log_sdl_app_metadata_properties] creator:%s",creator);
		Log("[log_sdl_app_metadata_properties] copyright:%s",copyright);
		Log("[log_sdl_app_metadata_properties] url:%s",url);
		Log("[log_sdl_app_metadata_properties] type:%s",type);
	}

	inline const char* get_name_sdl_log_priority(SDL_LogPriority priority) noexcept{
		std::unordered_map<SDL_LogPriority,const char*> v = {
			{SDL_LOG_PRIORITY_INVALID,"SDL_LOG_PRIORITY_INVALID"},
			{SDL_LOG_PRIORITY_TRACE,"SDL_LOG_PRIORITY_TRACE"},
			{SDL_LOG_PRIORITY_VERBOSE,"SDL_LOG_PRIORITY_VERBOSE"},
			{SDL_LOG_PRIORITY_DEBUG,"SDL_LOG_PRIORITY_DEBUG"},
			{SDL_LOG_PRIORITY_INFO,"SDL_LOG_PRIORITY_INFO"},
			{SDL_LOG_PRIORITY_WARN,"SDL_LOG_PRIORITY_WARN"},
			{SDL_LOG_PRIORITY_ERROR,"SDL_LOG_PRIORITY_ERROR"},
			{SDL_LOG_PRIORITY_CRITICAL,"SDL_LOG_PRIORITY_CRITICAL"},
			{SDL_LOG_PRIORITY_COUNT,"SDL_LOG_PRIORITY_COUNT"},
		};
		
		if (v.find(priority) != v.end()){
			return v[priority];
		}else{
			return nullptr;
		}

	}


	template <typename LogT>
	void log_sdl_log_priority(SDL_LogPriority priority, LogT &&Log = SDL_Log){
		Log("[log_sdl_log_priority] %s",get_name_sdl_log_priority(priority));
	}

	inline const char* get_name_sdl_log_category(SDL_LogCategory priority) noexcept{
		std::unordered_map<SDL_LogCategory,const char*> v = {
			{SDL_LOG_CATEGORY_APPLICATION,"SDL_LOG_CATEGORY_APPLICATION"},
			{SDL_LOG_CATEGORY_ERROR,"SDL_LOG_CATEGORY_ERROR"},
			{SDL_LOG_CATEGORY_ASSERT,"SDL_LOG_CATEGORY_ASSERT"},
			{SDL_LOG_CATEGORY_SYSTEM,"SDL_LOG_CATEGORY_SYSTEM"},
			{SDL_LOG_CATEGORY_AUDIO,"SDL_LOG_CATEGORY_AUDIO"},
			{SDL_LOG_CATEGORY_VIDEO,"SDL_LOG_CATEGORY_VIDEO"},
			{SDL_LOG_CATEGORY_RENDER,"SDL_LOG_CATEGORY_RENDER"},
			{SDL_LOG_CATEGORY_INPUT,"SDL_LOG_CATEGORY_INPUT"},
			{SDL_LOG_CATEGORY_TEST,"SDL_LOG_CATEGORY_TEST"},
			{SDL_LOG_CATEGORY_GPU,"SDL_LOG_CATEGORY_GPU"},
			{SDL_LOG_CATEGORY_RESERVED2,"SDL_LOG_CATEGORY_RESERVED2"},
			{SDL_LOG_CATEGORY_RESERVED3,"SDL_LOG_CATEGORY_RESERVED3"},
			{SDL_LOG_CATEGORY_RESERVED4,"SDL_LOG_CATEGORY_RESERVED4"},
			{SDL_LOG_CATEGORY_RESERVED5,"SDL_LOG_CATEGORY_RESERVED5"},
			{SDL_LOG_CATEGORY_RESERVED6,"SDL_LOG_CATEGORY_RESERVED6"},
			{SDL_LOG_CATEGORY_RESERVED7,"SDL_LOG_CATEGORY_RESERVED7"},
			{SDL_LOG_CATEGORY_RESERVED8,"SDL_LOG_CATEGORY_RESERVED8"},
			{SDL_LOG_CATEGORY_RESERVED9,"SDL_LOG_CATEGORY_RESERVED9"},
			{SDL_LOG_CATEGORY_RESERVED10,"SDL_LOG_CATEGORY_RESERVED10"},
			{SDL_LOG_CATEGORY_CUSTOM,"SDL_LOG_CATEGORY_CUSTOM"},
		};

		if (v.find(priority) != v.end()){
			return v[priority];
		}else{
			return nullptr;
		}
	}

	template <typename LogT>
	void log_sdl_log_category(SDL_LogCategory priority, LogT &&Log = SDL_Log){
		Log("[log_sdl_log_category] %s",get_name_sdl_log_category(priority));
	}


}

#endif
