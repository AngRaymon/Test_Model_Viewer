#ifndef VIEW_MODEL
#define VIEW_MODEL
#include "VAO.h"

class Model {
private:
    ColoredVAO m_mesh;

public:
    Model();
    ~Model();
    virtual void LoadData();
    virtual void BindData();
    virtual unsigned int GetNumIndices();
};


class NModel : public Model {
private:
    NormalVAO m_mesh;
    int m_numIndices;

public:
    NModel();
    ~NModel();
    void LoadData();
    void LoadData(std::vector<NormalVAO::NormalVert> &data, std::vector<unsigned int> &indices);
    void BindData();
    unsigned int GetNumIndices();
};

#endif