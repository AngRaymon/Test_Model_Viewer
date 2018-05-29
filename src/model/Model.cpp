#include "Model.h"
#include "VAO.h"

Model::Model() {

}

Model::~Model() {

}

void Model::LoadData() {
    m_mesh.Init();
    float verts[24] = {
        -0.5, -0.5, -0.5,
        0.5, -0.5, -0.5,
        0.5, 0.5, -0.5,
        -0.5, 0.5, -0.5,
        -0.5, -0.5, 0.5,
        0.5, -0.5, 0.5,
        0.5, 0.5, 0.5,
        -0.5, 0.5, 0.5,
    };
    float colors[32] = {
        0.0, 0.0, 1.0, 0.5,
        0.0, 1.0, 0.0, 0.5,
        0.0, 1.0, 1.0, 0.5,
        1.0, 0.0, 0.0, 0.5,
        1.0, 0.0, 1.0, 0.5,
        1.0, 1.0, 0.0, 0.5,
        1.0, 1.0, 1.0, 0.5,
        0.5, 0.5, 0.5, 0.5,
    };
    float uv[24];
    memset(uv, 0, 24 * sizeof(float));
    unsigned int indices[36] = {0, 1, 2, 2, 3, 0, 1, 5, 6, 6, 2, 1, 0, 4, 7, 7, 3, 0, 4, 5, 6, 6, 7, 4, 3, 2, 6, 6, 7, 3, 0, 1, 5, 5, 4, 0};
    m_mesh.LoadData(verts, colors, uv, indices, 24, 36);
}

unsigned int Model::GetNumIndices() {
    return 36;
}

void Model::BindData() {
    m_mesh.Bind();
}

NModel::NModel() {

}

NModel::~NModel() {

}

void NModel::LoadData() {
    m_mesh.Init();
    float verts[24] = {
        -0.5, -0.5, -0.5,
        0.5, -0.5, -0.5,
        0.5, 0.5, -0.5,
        -0.5, 0.5, -0.5,
        -0.5, -0.5, 0.5,
        0.5, -0.5, 0.5,
        0.5, 0.5, 0.5,
        -0.5, 0.5, 0.5,
    };
    float uv[24];
    memset(uv, 0, 24 * sizeof(float));
    float normals[24] = {
        -1.0, -1.0, -1.0,
        1.0, -1.0, -1.0,
        1.0, 1.0, -1.0,
        -1.0, 1.0, -1.0,
        -1.0, -1.0, 1.0,
        1.0, -1.0, 1.0,
        1.0, 1.0, 1.0,
        -1.0, 1.0, 1.0,
    };
    unsigned int indices[36] = {0, 1, 2, 2, 3, 0, 1, 5, 6, 6, 2, 1, 0, 4, 7, 7, 3, 0, 4, 5, 6, 6, 7, 4, 3, 2, 6, 6, 7, 3, 0, 1, 5, 5, 4, 0};
    m_mesh.LoadData(verts, uv, normals, indices, 24, 36);
}

void NModel::LoadData(std::vector<NormalVAO::NormalVert> &data, std::vector<unsigned int> &indices) {
    m_mesh.Init();
    m_mesh.LoadData(&data[0], &indices[0], data.size(), indices.size());
    m_numIndices = indices.size();
}

unsigned int NModel::GetNumIndices() {
    return m_numIndices;
}


void NModel::BindData() {
    m_mesh.Bind();
}