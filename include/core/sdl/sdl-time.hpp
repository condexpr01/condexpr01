#ifndef CORE_HEADER_SDL_TIME_GUARD
#define CORE_HEADER_SDL_TIME_GUARD

#include <SDL3/SDL.h>
#include <chrono>

namespace core{
	using std::chrono::time_point;
	using std::chrono::duration;
	using std::chrono::system_clock;
	using std::chrono::steady_clock;

	using std::chrono::zoned_time;
	using std::chrono::current_zone;

	//local sdl time
	template<typename LogT>
	void log_sdl_time(LogT &&Log = SDL_Log){
		//utc
		SDL_Time now;
		SDL_GetCurrentTime(&now);

		SDL_DateTime dt;

		//utc to local time
		SDL_TimeToDateTime(now,&dt,true);
		
		Log("%d/%d/%d %d:%d:%d",dt.year,dt.month,dt.day,dt.hour,dt.minute,dt.second);
	}

	//local chrono time
	template<typename LogT>
	void log_chrono_time(LogT &&Log = SDL_Log){
		//utc to local
		zoned_time lt{current_zone(),system_clock::now()};

		Log("%s",std::format("{:%Y/%m/%d %H:%M:%S}",lt.get_local_time()).c_str());
	}


	template<typename LogT>
	void log_sdl_date_time_locale_preferences(LogT &&Log = SDL_Log){
		SDL_DateFormat dateFormat;
		SDL_TimeFormat timeFormat;
		if(!SDL_GetDateTimeLocalePreferences(&dateFormat,&timeFormat))return;
		
		switch(dateFormat){
			case SDL_DATE_FORMAT_YYYYMMDD:
				Log("[log_sdl_date_time_locale_preferences] SDL_DATE_FORMAT_YYYYMMDD");
				break;

			case SDL_DATE_FORMAT_DDMMYYYY:
				Log("[log_sdl_date_time_locale_preferences] SDL_DATE_FORMAT_DDMMYYYY");
				break;

			case SDL_DATE_FORMAT_MMDDYYYY:
				Log("[log_sdl_date_time_locale_preferences] SDL_DATE_FORMAT_MMDDYYYY");
				break;

			default:
				Log("[log_sdl_date_time_locale_preferences] unknown: SDL_DateFormat");
				break;
		}

		switch(timeFormat){
			case SDL_TIME_FORMAT_24HR:
				Log("[log_sdl_date_time_locale_preferences] SDL_TIME_FORMAT_24HR");
				break;

			case SDL_TIME_FORMAT_12HR:
				Log("[log_sdl_date_time_locale_preferences] SDL_TIME_FORMAT_12HR");
				break;

			default:
				Log("[log_sdl_date_time_locale_preferences] unknown: SDL_TimeFormat");
				break;
		}

	}

}

#endif
