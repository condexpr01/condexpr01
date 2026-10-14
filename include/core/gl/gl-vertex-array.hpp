#ifndef CORE_HEADER_GL_VERTEX_ARRAY_GRUAD
#define CORE_HEADER_GL_VERTEX_ARRAY_GRUAD

#include "glad/gl.hpp"

namespace core{

	//bobj
	class buffer_object{
		private:
			GLuint bobj_id = 0;
			GLenum target = 0;

		//methods
		public:
			GLuint id() noexcept{return bobj_id;}
			GLuint get_target() noexcept{return target;}

			void bind() noexcept{
				glBindBuffer(target,bobj_id);
			}

			void unbind() noexcept{
				glBindBuffer(target,0);
			}

			void data(GLsizeiptr size,const void* data,GLenum usage) noexcept{
				bind();
				glBufferData(target,size,data,usage);
				unbind();
			}

			void take(buffer_object &bo) noexcept{
				if (this != &bo){
					bobj_id = bo.bobj_id;
					target = bo.target;

					bo.bobj_id = 0;
					bo.target = 0;
				}
			}

		//RAII
		public:
			buffer_object() = delete;

			buffer_object(GLenum target){
				(*this).target = target;
				glGenBuffers(1, &bobj_id);
			}

			buffer_object(buffer_object &bo) = delete;
			buffer_object &operator=(buffer_object &other) = delete;

			buffer_object(buffer_object &&bo) noexcept{
				take(bo);
			}

			buffer_object &operator=(buffer_object &&bo) noexcept{
				take(bo);
				return *this;
			}

			~buffer_object() noexcept{
				if(bobj_id){
					glDeleteBuffers(1, &bobj_id);
					bobj_id = 0;
				}
			}

	};


	class vertex_array{
		private:
			GLuint va_id = 0;

		//methods
		public:
			GLuint id() noexcept{return va_id;}

			void take(vertex_array &va) noexcept{
				va_id = va.va_id;
				va.va_id = 0;
			}

			void bind()noexcept{
				glBindVertexArray(va_id);
			}

			void unbind()noexcept{
				glBindVertexArray(0);
			}

			//index:      index of the vertex attribute to be modified
			//size:       number of components per vertex attribute
			//type:       data type of each component
			//normalized: whether fixed-point data values should be normalized
			//stride:     byte offset between consecutive vertex attributes
			//pointer:    offset of the first component of the first vertex attribute in the buffer
			void attrib_pointer(GLuint index, GLint size, GLenum type,
					GLboolean normalized, GLsizei stride,
					const void *pointer) noexcept{

				bind();
				glVertexAttribPointer(index,size,type,normalized,stride,pointer);
				unbind();

			}

			void enable_vertex_attrib_array(GLuint index) noexcept{
				bind();
				glEnableVertexAttribArray(index);
				unbind();
			}

			void disable_vertex_attrib_array(GLuint index) noexcept{
				bind();
				glDisableVertexAttribArray(index);
				unbind();
			}

			void draw_arrays(GLenum mode,GLint first, GLsizei size) noexcept{
				bind();
				if(size > 0)glDrawArrays(mode,first,size);
				unbind();
			}

			void draw_elements(GLenum mode, GLsizei count, GLenum type, const void *indices) noexcept{
				bind();
				if(count > 0)glDrawElements(mode,count,type,(const void*)indices);
				unbind();
			}

		//RAII
		public:
			vertex_array() noexcept{
				glGenVertexArrays(1,&va_id);
			}

			vertex_array(vertex_array  &va) = delete;
			vertex_array& operator=(vertex_array &va) = delete;

			vertex_array(vertex_array &&va) noexcept{
				take(va);
			}

			vertex_array& operator=(vertex_array &&va) noexcept{
				take(va);
				return *this;
			}

			~vertex_array() noexcept{
				if(va_id) {
					glDeleteVertexArrays(1,&va_id);
					va_id = 0;
				}
			}

	};

}

#endif
