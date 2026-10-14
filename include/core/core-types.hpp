#ifndef CORE_HEADER_TYPES_GUARD
#define CORE_HEADER_TYPES_GUARD

#include "glad/gl.hpp"

namespace core {

	template <typename posT>
	struct position{posT x,y,z;};

	template <typename posT>
	struct tex_pos_t{posT u,v,w;};

	template <typename colorT>
	struct color{colorT r,g,b,a;};

	//pos normal tangent uv rgba
	template <typename posT, typename colorT>
	struct vertex{
		struct position<posT> pos;
		struct position<posT> normal;
		struct position<posT> tangent;
		struct tex_pos_t<posT> tex_pos;
		struct color<colorT> color;
	};

	#define GLVERTEX_posT   GL_FLOAT
	#define GLVERTEX_colorT GL_UNSIGNED_BYTE
	using glposition = core::position<GLfloat>;
	using glvertex = core::vertex<GLfloat,GLubyte>;
}
#endif
