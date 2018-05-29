#include "VAO.h"

ColoredVAO::ColoredVAO() {
    m_ID = 0;
    m_VertexBufferID = 0;
    m_IndexBufferID = 0;
}

ColoredVAO::~ColoredVAO() {

}

bool ColoredVAO::Init() {
	glGenVertexArrays(1, &m_ID);
	glBindVertexArray(m_ID);
	glGenBuffers(1, &m_VertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
	glGenBuffers(1, &m_IndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);

	glEnableVertexAttribArray(ATTRIB_VERTEX);
	glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof(ColoredVert), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(ATTRIB_COLOR);
	glVertexAttribPointer(ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(ColoredVert), BUFFER_OFFSET(12));
	glEnableVertexAttribArray(ATTRIB_UV);
	glVertexAttribPointer(ATTRIB_UV, 2, GL_FLOAT, GL_FALSE, sizeof(ColoredVert), BUFFER_OFFSET(28));
    Unbind();
}

void ColoredVAO::LoadData(float* verts, float* colors, float* uv, unsigned int* indices, int numverts, int numIndices) {
	Bind();
	ColoredVert* cverts = new ColoredVert[numverts];
	for (int i = 0; i < numverts; ++i) {
		cverts[i].pos[0] = verts[3*i];
		cverts[i].pos[1] = verts[3*i+1];
		cverts[i].pos[2] = verts[3*i+2];
		cverts[i].color[0] = colors[4*i];
		cverts[i].color[1] = colors[4*i+1];
		cverts[i].color[2] = colors[4*i+2];
		cverts[i].color[3] = colors[4*i+3];
		cverts[i].uv[0] = 0;
		cverts[i].uv[1] = 0;
	}
	GLenum error;
	GLenum type = GL_TRIANGLES;
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, numverts * sizeof(ColoredVert), cverts, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	Unbind();
}

void ColoredVAO::Bind() {
	glBindVertexArray(m_ID);
}

void ColoredVAO::Unbind() {
	glBindVertexArray(0);
}

int ColoredVAO::GetID() {
	return m_ID;
}

void ColoredVAO::Destroy() {

}

NormalVAO::NormalVAO() {
	
}

NormalVAO::~NormalVAO() {

}

bool NormalVAO::Init() {
	glGenVertexArrays(1, &m_ID);
	glBindVertexArray(m_ID);
	glGenBuffers(1, &m_VertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
	glGenBuffers(1, &m_IndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);

	glEnableVertexAttribArray(ATTRIB_VERTEX);
	glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof(NormalVert), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(ATTRIB_UV);
	glVertexAttribPointer(ATTRIB_UV, 2, GL_FLOAT, GL_FALSE, sizeof(NormalVert), BUFFER_OFFSET(12));
	glEnableVertexAttribArray(ATTRIB_NORMAL);
	glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(NormalVert), BUFFER_OFFSET(20));
    Unbind();
}

void NormalVAO::Bind() {
	glBindVertexArray(m_ID);
}

void NormalVAO::Unbind() {
	glBindVertexArray(0);
}

int NormalVAO::GetID() {

}

void NormalVAO::Destroy() {

}

void NormalVAO::LoadData(float* verts, float* uv, float* normal, unsigned int* indices, int numverts, int numindices) {
	Bind();
	NormalVert* cverts = new NormalVert[numverts];
	for (int i = 0; i < numverts; ++i) {
		cverts[i].pos[0] = verts[3*i];
		cverts[i].pos[1] = verts[3*i+1];
		cverts[i].pos[2] = verts[3*i+2];
		cverts[i].uv[0] = 0;
		cverts[i].uv[1] = 0;
		cverts[i].normal[0] = normal[3*i];
		cverts[i].normal[1] = normal[3*i+1];
		cverts[i].normal[2] = normal[3*i+2];
	}
	GLenum error;
	GLenum type = GL_TRIANGLES;
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, numverts * sizeof(NormalVert), cverts, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numindices * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	Unbind();
}
static bool CheckGLError(GLenum &error) {
	//Check for error
	error = glGetError();
	if( error != GL_NO_ERROR )
	{
		printf( "OpenGL Error! %s\n", gluErrorString( error ) );
		return false;
	}
	return true;
}
void NormalVAO::LoadData(NormalVert *verts, unsigned int *indices, int numVerts, int numIndices) {
	Bind();
	GLenum error;
	GLenum type = GL_TRIANGLES;
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, numVerts * sizeof(NormalVert), verts, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	Unbind();
	CheckGLError(error);
}