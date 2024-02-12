#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* vertices, GLsizei size){
	glGenBuffers(1, &VertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}
VertexBuffer::~VertexBuffer(){
	glDeleteBuffers(1, &VertexBufferID);
}

// Bind the vertex buffer
void VertexBuffer::Bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
}

// Unbind the vertex buffer
void VertexBuffer::Unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


// Fill out a specific segment of the buffer given by an offset and a size.
void VertexBuffer::BufferSubData(GLintptr offset, GLsizeiptr size, const void* data) const {
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}