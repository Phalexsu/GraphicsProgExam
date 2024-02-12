#ifndef RENDERCOMMANDS_H_
#define RENDERCOMMANDS_H_

#include "glad/glad.h"
#include "VertexArray.h"


namespace RenderCommands
{
	inline void Clear(GLuint mode = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) { glClear(mode); };
	inline void SetPolygonMode(GLenum face, GLenum mode) { glPolygonMode(face, mode); }
	inline void DrawIndex(const std::shared_ptr<VertexArray>& vao, GLenum primitive) { glDrawElements(primitive, vao->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr); }
}


#endif
