#ifndef CORE_HEADER_GL_SHADER_GUARD
#define CORE_HEADER_GL_SHADER_GUARD

#include "glad/gl.hpp"

namespace core{

	class shader{

		//error status
		private:
			GLboolean status = GL_FALSE;
			GLchar *reason = nullptr;
			GLuint shader_id = 0;

		//get error status methods
		public:
			GLboolean is_ok() noexcept{return status;}
			GLchar*    what() noexcept{return reason;}

		public:
			GLuint id() noexcept{return shader_id;};

			void take(shader &s) noexcept{
				shader_id = s.shader_id;
				reason = s.reason;
				status = s.status;

				s.shader_id = 0;
				s.reason = nullptr;
				s.status = GL_FALSE;
			}

			void release() noexcept{
				if (reason){
					delete[] reason;
					reason = nullptr;
				}

				if (shader_id){
					glDeleteShader(shader_id);
					shader_id = 0;
				}

				if (status)status = GL_FALSE;
			}

		public:
			shader(GLenum type, const char* src) {
				shader_id = glCreateShader(type);
				glShaderSource(shader_id,1,&src,nullptr);
				glCompileShader(shader_id);

				GLint compile_status;
				glGetShaderiv(shader_id,GL_COMPILE_STATUS,&compile_status);
				status = static_cast<GLboolean>(compile_status);

				if (!compile_status){
					GLsizei log_length = 0;
					glGetShaderiv(shader_id,GL_INFO_LOG_LENGTH,&log_length);

					reason = new GLchar[log_length]; //maybe throw causing exception
					glGetShaderInfoLog(shader_id,log_length,nullptr,reason);
				}

			}

			//防止重复析构
			shader(shader  &s) = delete;
			shader& operator=(shader &s) = delete;

			shader(shader &&s) noexcept{take(s);}
			shader& operator=(shader &&s) noexcept{
				if(this != &s){
					release();
					take(s);
				}

				return *this;
			}


			//需要保证生命周期在gl context内, 不然glDeleteShader会错
			~shader(){release();}

	};

}

#endif
