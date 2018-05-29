#ifndef _COLORED_VERT_SHADER_
#define _COLORED_VERT_SHADER_
#include "BaseShader.h"
class ColoredVertShader : Shader {
private:
    static const char* fragShaderSrc;
    static const char* vertShaderSrc;
    int m_textureUniform;
    int m_program;

public:
    ColoredVertShader();
    ~ColoredVertShader();
    bool LoadShaders() override;
    void Activate() override;
    void Deactivate() override;
};
#endif