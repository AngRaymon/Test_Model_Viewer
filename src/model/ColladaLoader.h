#ifndef COLLADA_LOADER
#define COLLADA_LOADER
#include "Model.h"
#include "tinyxml2.h"
#include <map>

class ColladaLoader {
public:
    ColladaLoader();
    ~ColladaLoader();
    void LoadData(std::string ColladaFile, int maxWeights);
    std::vector<NormalVAO::NormalVert>& GetVerts();
    std::vector<unsigned int>& GetIndices();

private:
    struct MeshData {
        bool *vertLoaded;
        float *normals;
        int *indices;
        char flags;
        int numVerts, numNormals;
        MeshData() : vertLoaded(NULL), normals(NULL), indices(NULL), flags(0) {}
    };
    enum Semantics {
        VERTEX = 0,
        NORMAL,
        TEXCOORD,
        COLOR,
        POSITION
    };
    enum LoadFlags : char {
        NONE = 0,
        VERTS_LOADED = 1 << 0,
        NORMALS_LOADED = 1 << 1,
        TEXCOORD_LOADED = 1 << 2,
        COLORS_LOADED = 1 << 3
    };
    std::vector<NormalVAO::NormalVert> m_verts;
    std::vector<unsigned int> m_indices;
    static std::map<std::string, int> m_Semantics;
    bool LoadMeshData(MeshData &data, tinyxml2::XMLDocument &xmlDoc);
    bool LoadVerts(MeshData &data, tinyxml2::XMLElement *input, tinyxml2::XMLElement *mesh);
    bool LoadNormals(MeshData &data, tinyxml2::XMLElement *input, tinyxml2::XMLElement *mesh);
    bool ProcessVerts(MeshData &data, tinyxml2::XMLElement *polyList, std::map<ColladaLoader::Semantics, int> &offsets, int numInputs);
    tinyxml2::XMLElement *FindChildWithAttribute(tinyxml2::XMLElement *base, const char* element, const char* attr, const char* value);
};
#endif