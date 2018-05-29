#ifndef _PHONG_SHADER_
#define _PHONG_SHADER_
#include "BaseShader.h"

class PhongShader : Shader {
private:
    static const char* fragShaderSrc;
    static const char* vertShaderSrc;
    int m_textureUniform;
    int m_program;

public:
    PhongShader();
    ~PhongShader();
    bool LoadShaders() override;
    void Activate() override;
    void Deactivate() override;
};

#endif