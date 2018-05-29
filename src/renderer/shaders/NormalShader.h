#ifndef _NORMAL_SHADER_
#define _NORMAL_SHADER_
#include "BaseShader.h"

class NormalShader : Shader {
private:
    static const char* fragShaderSrc;
    static const char* vertShaderSrc;
    int m_textureUniform;
    int m_program;

public:
    NormalShader();
    ~NormalShader();
    bool LoadShaders() override;
    void Activate() override;
    void Deactivate() override;
};

#endif