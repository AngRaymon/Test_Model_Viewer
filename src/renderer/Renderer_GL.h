#ifndef RENDERER_GL
#define RENDERER_GL
#include "VAO.h"
#include "Model.h"
//#include <GLUT/glut.h>
#include <SDL.h>
//#include <SDL_opengl.h>

#define MAX_TEXTURES 1024
typedef unsigned int U32;
typedef int S32;
typedef char U8;
static const int SCREEN_WIDTH = 1280;
static const int SCREEN_HEIGHT = 768;
struct SGLVert {
	GLfloat pos[3];
	GLfloat color[4];
	GLfloat uv[2];
};
struct SGLTexture {
	GLuint t;
	U32 flags;
};

enum {
	ATTRIB_VERTEX,
	ATTRIB_COLOR,
	ATTRIB_UV,
	NUM_ATTRIBUTES
};

class Renderer_GL {
private:
	static Renderer_GL* m_pInstance;
	SGLTexture m_textures[MAX_TEXTURES];
	GLuint m_glVertexBuffer;
	GLuint m_glVertexArray;
	GLuint m_glIndexBuffer;
	GLuint m_glTexUniform;
	GLuint m_glTexUniformDefault;
	GLint m_glDefaultProgram;

	bool InitContext();
	bool InitGL();
	bool LoadShaders();
	std::vector<IVertexArrayObject*> m_VAOs;
public:
	static Renderer_GL* GetInstance();
	Renderer_GL();
	~Renderer_GL();
	bool Init();
	void Clean();
	void InitWindow(U32 unWidth, U32 unHeight);
	void DrawTriangles(SGLVert* verts, U32* indices, U32 numVerts, U32 numIndices);
	void DrawMesh(SGLVert* verts, U32* indices, U32 numVerts, U32 numIndices);
	void DrawModel(Model *m, bool drawMesh = false);
	void ClearShader();
	int AllocateTexture(U32 options);
	void UploadTexture(int t, U8* buffer, unsigned short width, unsigned short height, U32 pitch, U32 bufsize, U32 flags);
	void DestroyTexture(int t);
	void StartRenderFrame(U8 options);
	void EndRenderFrame(U8 options);
};
#endif