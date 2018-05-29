#include "ColladaLoader.h"
#include <string>

std::map<std::string, int> ColladaLoader::m_Semantics = {
    {"VERTEX", 0},
    {"NORMAL", 1},
    {"TEXCOORD", 2},
    {"COLOR", 3},
    {"POSITION", 4}
};

ColladaLoader::ColladaLoader() {

}

ColladaLoader::~ColladaLoader() {
    
}

void ColladaLoader::LoadData(std::string ColladaFile, int maxWeights) {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(ColladaFile.c_str());
    MeshData mesh;
    LoadMeshData(mesh, doc);
}

bool ColladaLoader::LoadMeshData(MeshData &data, tinyxml2::XMLDocument &xmlDoc) {
    tinyxml2::XMLElement *rootElement = xmlDoc.FirstChildElement("COLLADA");
    tinyxml2::XMLElement *geometriesEle = rootElement->FirstChildElement("library_geometries");
    tinyxml2::XMLElement *geometry = geometriesEle->FirstChildElement("geometry");
    tinyxml2::XMLElement *mesh = geometry->FirstChildElement("mesh");
    tinyxml2::XMLElement *polyList = mesh->FirstChildElement("polylist");
    if (!polyList) polyList = mesh->FirstChildElement("triangles");
    if (polyList) {
        tinyxml2::XMLElement *input = polyList->FirstChildElement("input");
        int numInputs = 0;
        std::map<ColladaLoader::Semantics, int> offsets;
        while (input) {
            std::string semantic = input->Attribute("semantic");
            if (m_Semantics.find(semantic) != m_Semantics.end()) {
                switch(m_Semantics[semantic]) {
                    case Semantics::VERTEX:
                        LoadVerts(data, input, mesh);
                        offsets[Semantics::VERTEX] = atoi(input->Attribute("offset"));
                        break;
                    case Semantics::NORMAL:
                        LoadNormals(data, input, mesh);
                        offsets[Semantics::NORMAL] = atoi(input->Attribute("offset"));
                        break;
                    case Semantics::TEXCOORD:

                        break;
                    case Semantics::COLOR:

                        break;
                    default:
                        break;
                }
            }
            ++numInputs;
            input = input->NextSiblingElement("input");
        }
        ProcessVerts(data, polyList, offsets, numInputs);
    }
}

bool ColladaLoader::LoadVerts(MeshData &data, tinyxml2::XMLElement *input, tinyxml2::XMLElement* mesh) {
    if (!(data.flags && LoadFlags::VERTS_LOADED)) {
        std::string vertexSource = input->Attribute("source");
        if (vertexSource[0] == '#') {
            vertexSource = vertexSource.substr(1);
        }
        //find vertices element
        tinyxml2::XMLElement *vertices = FindChildWithAttribute(mesh, "vertices", "id", vertexSource.c_str());
        if (!vertices) return false;
        //find input with POSITION
        tinyxml2::XMLElement *vertInput = FindChildWithAttribute(vertices, "input", "semantic", "POSITION");
        if (!vertInput) return false;
        //find Positions
        std::string positionSource = vertInput->Attribute("source");
        if (positionSource[0] == '#') {
            positionSource = positionSource.substr(1);
        }
        tinyxml2::XMLElement *posSrcEle = FindChildWithAttribute(mesh, "source", "id", positionSource.c_str());
        if (!posSrcEle) return false;
        //Assume here x, y, z float coordinates
        tinyxml2::XMLElement *floatArray = posSrcEle->FirstChildElement("float_array");
        if (!floatArray) return false;
        int flCount = atoi(floatArray->Attribute("count"));
        data.numVerts = flCount / 3;
        data.vertLoaded = new bool[data.numVerts];
        memset(data.vertLoaded, 0, data.numVerts);
        m_verts.resize(data.numVerts);
        int c = 0;
        char* flArrayText = strdup(floatArray->GetText());
        char* tok = strtok(flArrayText, " ");
        while (c < data.numVerts && tok != NULL) {
            m_verts[c].pos[0]= atof(tok);
            tok = strtok(NULL, " ");
            m_verts[c].pos[1]= atof(tok);
            tok = strtok(NULL, " ");
            m_verts[c++].pos[2]= atof(tok);
            tok = strtok(NULL, " ");
        }
        //if (c != flCount) {
        //    delete data.vertLoaded;
        //    return false;
        //}
        data.flags &= LoadFlags::VERTS_LOADED;
        return true;
    }
    return true;
}

bool ColladaLoader::LoadNormals(MeshData &data, tinyxml2::XMLElement *input, tinyxml2::XMLElement *mesh) {
    if (!(data.flags && LoadFlags::NORMALS_LOADED)) {
        std::string normalSource = input->Attribute("source");
        if (normalSource[0] == '#') {
            normalSource = normalSource.substr(1);
        }
        tinyxml2::XMLElement *normalSrcEle = FindChildWithAttribute(mesh, "source", "id", normalSource.c_str());
        if (!normalSrcEle) return false;
        tinyxml2::XMLElement *floatArray = normalSrcEle->FirstChildElement("float_array");
        if (!floatArray) return false;
        int flCount = atoi(floatArray->Attribute("count"));
        data.numNormals = flCount / 3;
        data.normals = new float[flCount];
        int c = 0;
        char* flArrayText = strdup(floatArray->GetText());
        char* tok = strtok(flArrayText, " ");
        while (c < flCount && tok != NULL) {
            data.normals[c++] = atof(tok);
            tok = strtok(NULL, " ");
        }
        //if (c != flCount) {
        //    delete data.normals;
        //    return false;
        //}
    }
}

bool ColladaLoader::ProcessVerts(MeshData &data, tinyxml2::XMLElement *polyList, std::map<ColladaLoader::Semantics, int> &offsets, int numInputs) {
    char * polyListStr = strdup(polyList->FirstChildElement("p")->GetText());
    char* tok = strtok(polyListStr, " ");
    int index = 0;
    while (tok != NULL) {
        int curOffset = 0;
        int positionIndex, normalIndex;
        int *inputs = new int[numInputs];
        for (int i = 0; i < numInputs; ++i) {
            inputs[i] = atoi(tok);
            tok = strtok(NULL, " ");
        }
        positionIndex = inputs[offsets[Semantics::VERTEX]];
        normalIndex = inputs[offsets[Semantics::NORMAL]];
        m_verts[positionIndex].normal[0] = data.normals[normalIndex * 3];
        m_verts[positionIndex].normal[1] = data.normals[normalIndex * 3 + 1];
        m_verts[positionIndex].normal[2] = data.normals[normalIndex * 3 + 2];
        m_indices.push_back(positionIndex);
    }
}

tinyxml2::XMLElement *ColladaLoader::FindChildWithAttribute(tinyxml2::XMLElement *base, const char* element, const char* attr, const char* value) {
    tinyxml2::XMLElement *child = base->FirstChildElement(element);
    while (child) {
        if (strcmp(child->Attribute(attr), value) == 0) {
            break;
        }
        child = child->NextSiblingElement(element);
    }
    return child;
}


std::vector<NormalVAO::NormalVert>& ColladaLoader::GetVerts() {
    return m_verts;
}

std::vector<unsigned int>& ColladaLoader::GetIndices() {
    return m_indices;
}