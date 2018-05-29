#ifndef Vertex_Array_Objects
#define Vertex_Array_Objects
#include <vector>

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#ifdef __APPLE__
#define glGenVertexArrays glGenVertexArraysAPPLE
#define glBindVertexArray glBindVertexArrayAPPLE
#define glDeleteVertexArrays glDeleteVertexArraysAPPLE
#endif

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

class IVertexArrayObject {
public:
    enum VertAttrib{
    	ATTRIB_VERTEX,
        ATTRIB_NORMAL,
    	ATTRIB_UV,
    	ATTRIB_COLOR,
        ATTRIB_JOINT_ID,
        ATTRIB_WEIGHTS,
    	NUM_ATTRIBUTES
    };
    virtual bool Init() = 0;
    virtual void Bind() = 0;
    virtual void Unbind() = 0;
    virtual int GetID() = 0;
    virtual void Destroy() = 0;

};

class ColoredVAO : IVertexArrayObject {
public:
    struct ColoredVert {
	    GLfloat pos[3];
	    GLfloat color[4];
	    GLfloat uv[2];
    };
    ColoredVAO();
    ~ColoredVAO();
    bool Init() override;
    void Bind() override;
    void Unbind() override;
    int GetID() override;
    void Destroy() override;
    void LoadData(float* verts, float* colors, float* uv, unsigned int* indices, int numverts, int numIndices);
    
private:
    unsigned int m_ID;
    unsigned int m_VertexBufferID;
    unsigned int m_IndexBufferID;
};

class NormalVAO: IVertexArrayObject {
public:
    struct NormalVert {
        GLfloat pos[3];
        GLfloat uv[2];
        GLfloat normal[3];
    };
    NormalVAO();
    ~NormalVAO();
    bool Init() override;
    void Bind() override;
    void Unbind() override;
    int GetID() override;
    void Destroy() override;
    void LoadData(float* verts, float* uv, float* normal, unsigned int* indices, int numverts, int numindices);
    void LoadData(NormalVert *verts, unsigned int *indices, int numVerts, int numIndices);

private:
    unsigned int m_ID;
    unsigned int m_VertexBufferID;
    unsigned int m_IndexBufferID;
};
#endif