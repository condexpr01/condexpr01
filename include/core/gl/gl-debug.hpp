#ifndef CORE_HEADER_GL_DEBUG_GUARD
#define CORE_HEADER_GL_DEBUG_GUARD

#include "glad/gl.hpp"
#include <SDL3/SDL.h>

namespace core{

	template <auto Log = SDL_Log>
	void debug_error() noexcept{
		GLenum error = glGetError();
		if(error!=GL_NO_ERROR){
			Log("[core::debug_error]error: %d",error);
		}
	}

}
#endif
