#ifndef _BASE_SHADER_
#define _BASE_SHADER_

class Shader {
public:
    virtual bool LoadShaders() = 0;
    virtual void Activate() = 0;
    virtual void Deactivate() = 0;
};
#endif