#ifndef CORE_HEADER_GL_PROGRAM_GUARD
#define CORE_HEADER_GL_PROGRAM_GUARD

#include "glad/gl.hpp"
#include <utility>

namespace core{

	class program{
		//error status
		private:
			GLboolean status = GL_FALSE;
			GLchar *reason = nullptr;
			GLuint program_id = 0;

		//get error status methods
		public:
			GLboolean is_ok() noexcept{return status;}
			GLchar*    what() noexcept{return reason;}

		public:
			GLuint id() noexcept{return program_id;};

			void link() {
				glLinkProgram(program_id);

				GLint link_status = 0;
				glGetProgramiv(program_id,GL_LINK_STATUS,&link_status);
				status = static_cast<GLboolean>(link_status);

				if(!link_status){
					if(reason){//prevent mem leak when multilink
						delete[] reason;
						reason = nullptr;
					}

					glGetProgramiv(program_id,GL_INFO_LOG_LENGTH,&link_status);

					reason = new GLchar[link_status];//maybe throw causing exception
					glGetProgramInfoLog(program_id,link_status,nullptr,reason);
				}
			}

			void take(program&& p) noexcept{
				program_id = p.program_id;
				status = p.status;
				reason = p.reason;

				p.program_id = 0;
				p.status = GL_FALSE;
				p.reason = nullptr;
			}

			void attach_shader(GLuint shader) noexcept{
				glAttachShader(program_id,shader);
			}

			void release() noexcept{
				if (program_id){glDeleteProgram(program_id);}

				program_id = 0;
				status = GL_FALSE;

				delete[] reason;
				reason = nullptr;
			}

			void use() noexcept{
				if(program_id && status)glUseProgram(program_id);
			}

			GLint uniform_location(const GLchar* name) noexcept{
				return glGetUniformLocation(program_id,name);
			}

		public:
			program() noexcept{
				program_id = glCreateProgram();
				status = GL_FALSE;
				reason = nullptr;
			}

			program(program&  p) = delete;
			program(program&& p) noexcept {
				take(std::move(p));
			}

			program& operator=(program&  p) = delete;
			program& operator=(program&& p)noexcept{
				if (this != &p){
					release();
					take(std::move(p));
				}

				return (*this);
			}

			~program() noexcept{release();}

	};

}

#endif
