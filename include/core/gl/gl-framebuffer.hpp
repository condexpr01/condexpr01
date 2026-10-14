
#ifndef CORE_HEADER_GL_FRAMEBUFFER_GRUAD
#define CORE_HEADER_GL_FRAMEBUFFER_GRUAD

#include "glad/gl.hpp"

namespace core{
	class framebuffer{
		private:
			GLuint fbobj;
			GLenum target;

		//error status
		private:
			GLboolean status = GL_FALSE;
			const GLchar *reason = nullptr;

		//get error status methods
		public:
			GLboolean is_ok() noexcept{
				bind();
				GLenum fb_status = glCheckFramebufferStatus(target);
				unbind();

				if (fb_status == GL_FRAMEBUFFER_COMPLETE){
					status = GL_TRUE;
				}else{
					status = GL_FALSE;
					reason = "[core::frambuffer]incomplete framebuffer";
				}

				return status;
			}

			const GLchar* what() noexcept{return reason;}


		//methods
		public:
			GLuint id() noexcept{return fbobj;}
			GLenum get_target() noexcept{return target;}

			void bind()noexcept {glBindFramebuffer(target,fbobj);}
			void unbind()noexcept {glBindFramebuffer(target,0);}

			void take(framebuffer &fb) noexcept{
				if ((*this).fbobj != fb.fbobj){
					(*this).fbobj  =fb.fbobj;
					(*this).target =fb.target;
					(*this).status =fb.status;
					(*this).reason =fb.reason;

					fb.fbobj  = 0;
					fb.target = 0;
					fb.status = GL_FALSE;
					fb.reason = nullptr;
				}
			}

			void frambuffer_texture(GLenum attachment, GLuint texture, GLuint level) noexcept{
				bind();
				glFramebufferTexture(target,attachment,texture,level);
				unbind();
			}

			void frambuffer_renderbuffer(GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) noexcept{
				bind();
				glFramebufferRenderbuffer(target,attachment,renderbuffertarget,renderbuffer);
				unbind();
			}

		//RAII
		public:
			framebuffer() = delete;

			framebuffer(GLenum target) noexcept{
				glGenFramebuffers(1,&fbobj);
				(*this).target = target;
			}

			~framebuffer() noexcept{
				if (fbobj){glDeleteFramebuffers(1,&fbobj);}
			}

			framebuffer(framebuffer &fb) = delete;
			framebuffer& operator=(framebuffer &fb) = delete;

			framebuffer(framebuffer &&fb) noexcept{take(fb);}
			framebuffer &operator=(framebuffer &&fb) noexcept{
				take(fb);
				return *this;
			}

	};
}

#endif
