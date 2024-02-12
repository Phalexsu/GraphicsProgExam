#include "VertexArray.h"

// Constructor & Destructor
VertexArray::VertexArray() {
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
}
VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &m_vertexArrayID);
}

// Bind vertex array
void VertexArray::Bind() const {
	glBindVertexArray(m_vertexArrayID);
}
// Unbind vertex array
void VertexArray::Unbind() const {
	glBindVertexArray(0);
}


// Add vertex buffer. This method utilizes the BufferLayout internal to
// the vertex buffer to set up the vertex attributes. Notice that
// this function opens for the definition of several vertex buffers.
void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) {
	VertexBuffers.push_back(vertexBuffer);
	vertexBuffer->Bind();
	const auto layout = vertexBuffer->GetLayout();
	const auto& attributes = layout.GetAttributes();
	for (unsigned int i = 0; i < attributes.size(); i++)
	{
		const auto& attribute = attributes[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, ShaderDataTypeComponentCount(attribute.Type), ShaderDataTypeToOpenGLBaseType(attribute.Type), attribute.Normalized, layout.GetStride(), (const void*)attribute.Offset);
		
	}
	

}
// Set index buffer
void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) {
	IdxBuffer = indexBuffer;
	IdxBuffer->Bind();
}
