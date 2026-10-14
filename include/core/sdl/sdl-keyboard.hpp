#ifndef CORE_HEADER_SDL_KEYBOARD_GUARD
#define CORE_HEADER_SDL_KEYBOARD_GUARD

#include <SDL3/SDL.h>
#include <string>

//key detectd base on SDL_GetKeyboardState

namespace core{

	class keyboard{

		public:
			int numkeys = 0;
			const bool *state = nullptr;

			keyboard() noexcept{
				state = SDL_GetKeyboardState(&numkeys);
			}

			~keyboard() = default;

			bool is_pressed(SDL_Scancode scancode) noexcept{
				if(scancode >=0 && scancode < numkeys){
					return state[scancode];
				}else{
					return false;
				}
			}

			template<typename LogT>
			void log(LogT &&Log = SDL_Log) noexcept{
				int pressed = 0;
				std::string s{};
				if(state != nullptr){
					for (int i = 0; i < numkeys ; i++){
						if (state[i] == true){
							s+= "1";
							pressed++;
						}else{
							s+= "0";
						}

						if ((i+1) % 64 == 0) s+= '\n';
					}
				}

				Log("[log_sdl_keyboard_state]numkeys:%d pressed:%d\n%s",numkeys,pressed,s.c_str());
			}

			//3/5/[0] don't manage resource, so zero overhead

	};

}

#endif
