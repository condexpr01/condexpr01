
#ifndef CORE_HEADER_GL_RENDERBUFFER_GRUAD
#define CORE_HEADER_GL_RENDERBUFFER_GRUAD

#include "glad/gl.hpp"

namespace core{
	class renderbuffer{
		private:
			GLuint robj;
			GLenum target;

		//methods
		public:
			GLuint id() noexcept{return robj;}
			GLenum get_target() noexcept{return target;}

			void bind()noexcept  {glBindRenderbuffer(target,robj);}
			void unbind()noexcept{glBindRenderbuffer(target,0);}

			void take(renderbuffer &rb) noexcept{
				if ((*this).robj != rb.robj){
					(*this).robj  =rb.robj;
					(*this).target =rb.target;

					rb.robj  = 0;
					rb.target = 0;
				}
			}

			void renderbuffer_storage(GLenum internalformat, GLsizei width, GLsizei height){
				bind();
				glRenderbufferStorage(target,internalformat,width,height);
				unbind();
			}

		//RAII
		public:
			renderbuffer() noexcept{
				glGenRenderbuffers(1,&robj);
				(*this).target = GL_RENDERBUFFER;
			}

			~renderbuffer() noexcept{
				if (robj){glDeleteRenderbuffers(1,&robj);}
			}

			renderbuffer(renderbuffer &rb) = delete;
			renderbuffer& operator=(renderbuffer &rb) = delete;

			renderbuffer(renderbuffer &&rb) noexcept{take(rb);}
			renderbuffer &operator=(renderbuffer &&rb) noexcept{
				take(rb);
				return *this;
			}
			
	};
}

#endif
