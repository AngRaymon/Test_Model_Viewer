#include "Renderer_GL.h"
#include "math.h"
#include "shaders/Shaders.h"

#define TXALLOCATED 1

//The window we'll be rendering to
SDL_Window* gWindow = NULL;
//OpenGL context
SDL_GLContext gContext;

bool CheckGLError(GLenum &error) {
	//Check for error
	error = glGetError();
	if( error != GL_NO_ERROR )
	{
		printf( "OpenGL Error! %s\n", gluErrorString( error ) );
		return false;
	}
	return true;
}

Renderer_GL* Renderer_GL::m_pInstance = NULL;
Renderer_GL::Renderer_GL() {
	m_glDefaultProgram = 0;
	for (int i = 0; i < MAX_TEXTURES; ++i) {
		m_textures[i].flags = 0;
		m_textures[i].t = 0;
	}
}

Renderer_GL::~Renderer_GL() {

}

Renderer_GL* Renderer_GL::GetInstance() {
	if (!m_pInstance) {
		m_pInstance = new Renderer_GL();
	}
	return m_pInstance;
}

bool Renderer_GL::InitContext() {
	//Initialization flag
	bool success = true;
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else {
		//Use OpenGL 2.1
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );

		//Create window
		gWindow = SDL_CreateWindow( "Model Viewer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
		if( gWindow == NULL ) {
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else {
			//Create context
			gContext = SDL_GL_CreateContext( gWindow );
			if( gContext == NULL ) {
				printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else {
				//Use Vsync
				if( SDL_GL_SetSwapInterval( 1 ) < 0 ) {
					printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
				}

				//Initialize OpenGL
				if( !InitGL() ) {
					printf( "Unable to initialize OpenGL!\n" );
					success = false;
				}
			}
		}
	}

	return success;
}

bool Renderer_GL::InitGL() {
	bool success = true;
	printf("GLInit Start \n");
	GLenum error = GL_NO_ERROR;
	//Initialize Projection Matrix
	glMatrixMode( GL_PROJECTION );
	//glLoadIdentity();
	//gluPerspective(90, 1.67, 0.1, 5);
	gluPerspective(90, 1.67, 0.1, 300.0);
	//glOrtho(-0.5f, 0.5f, -0.5f, 0.5f, 0.0f, 5.0f);
	success = CheckGLError(error);
	//Initialize Modelview Matrix
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	success = CheckGLError(error);
	//Initialize clear color
	glClearColor( 0.f, 0.f, 0.f, 1.f );
	success = CheckGLError(error);
	LoadShaders();
	success = CheckGLError(error);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	return success;
}

static NormalShader s_shader;
bool Renderer_GL::LoadShaders() {
	bool success = s_shader.LoadShaders();
}

bool Renderer_GL::Init() {
	InitContext();
	return true;
}

void Renderer_GL::InitWindow(U32 width, U32 height) {

}

void Renderer_GL::Clean() {
	//Destroy window	
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
}

void Renderer_GL::DrawTriangles(SGLVert* verts, U32* indices, U32 numVerts, U32 numIndices) {
	GLenum error;
	GLenum type = GL_TRIANGLES;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, numVerts * sizeof(SGLVert), verts, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(U32), indices, GL_DYNAMIC_DRAW);
	glDrawElements(type, numIndices, GL_UNSIGNED_INT, NULL);
}

void Renderer_GL::DrawMesh(SGLVert* verts, U32* indices, U32 numVerts, U32 numIndices) {
	glPolygonMode(GL_FRONT, GL_LINE);
	glPolygonMode(GL_BACK, GL_LINE);
	DrawTriangles(verts, indices, numVerts, numIndices);
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);
}

void Renderer_GL::DrawModel(Model* m, bool drawMesh){
	static float deg = 0;
	deg += 1;
	if (deg > 360) deg = 0;
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glRotated(deg, 0.0, 1.0, 0.0);
	if (drawMesh) {
		glPolygonMode(GL_FRONT, GL_LINE);
		glPolygonMode(GL_BACK, GL_LINE);
	}
	s_shader.Activate();
	m->BindData();
	glDisable(GL_BLEND);
	glDrawElements(GL_TRIANGLES, m->GetNumIndices(), GL_UNSIGNED_INT, NULL);
	s_shader.Deactivate();
	if (drawMesh) {
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_FILL);
	}
	glPopMatrix();
}

void Renderer_GL::ClearShader() {

}

int Renderer_GL::AllocateTexture(U32 flags) {
	int s;
	int retval = -1;
	for (s = 0; s < MAX_TEXTURES; ++s) {
		if (!m_textures[s].flags && TXALLOCATED) {
			m_textures[s].flags = flags | TXALLOCATED;
			GLuint tmp = 0;
			glGenTextures(1, &tmp);
			retval = s;
			break;
		}
	}
	return retval;
}

void Renderer_GL::UploadTexture(int t, U8* buffer, unsigned short width, unsigned short height, U32 pitch, U32 bufsize, U32 flags) {
	int texSize = width * height;
	glBindTexture(GL_TEXTURE_2D, t);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, buffer);
}

void Renderer_GL::DestroyTexture(int t) {
	GLuint tex = m_textures[t].t;
	glDeleteTextures(1, &tex);
	m_textures[t].flags=0;
}

void Renderer_GL::StartRenderFrame(U8 options) {
	static float Dist = 100.0;
	static float xpos = Dist;
	static float ypos = Dist;
	static float zpos = Dist;
	static float theta = 0;
	static float phi = 1.0;
	//xpos = Dist * 2*sin(phi) * cos(theta);
	//zpos = Dist * 2*sin(phi) * sin(theta);
	//ypos = Dist * 2*cos(phi);
	phi += 0.01;
	theta += 0.01;
	if (theta > (2*3.141592654)) theta = 0;
	if (phi > (2*3.141592654)) phi = 0;
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUseProgram(m_glDefaultProgram);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	gluLookAt(xpos, ypos + 80.0, zpos, 0.0, 80.0, 0.0, 0.0, 1.0, 0.0);
}

void Renderer_GL::EndRenderFrame(U8 options) {
	//Update screen
	GLenum error;
	CheckGLError(error);
	SDL_GL_SwapWindow( gWindow );
}