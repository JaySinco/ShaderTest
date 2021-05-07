#include "model.h"
#include "utils.h"
#include <glm/ext/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace gl
{
Model::Model() { this->reset(); }

void Model::draw() const
{
    for (const auto &mesh: this->meshes) {
        mesh->draw();
    }
}

void Model::reset()
{
    this->translate = glm::mat4(1.0f);
    this->rotate = glm::mat4(1.0f);
    this->scale = glm::mat4(1.0f);
}

void Model::move(float dx, float dy, float dz)
{
    this->translate = glm::translate(glm::mat4(1.0f), glm::vec3(dx, dy, dz)) * this->translate;
}

void Model::moveTo(float x, float y, float z)
{
    this->translate = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
}

void Model::spin(float ddegree, float axis_x, float axis_y, float axis_z)
{
    this->rotate =
        glm::rotate(glm::mat4(1.0f), glm::radians(ddegree), glm::vec3(axis_x, axis_y, axis_z)) *
        this->rotate;
}

void Model::spinTo(float degree, float axis_x, float axis_y, float axis_z)
{
    this->rotate =
        glm::rotate(glm::mat4(1.0f), glm::radians(degree), glm::vec3(axis_x, axis_y, axis_z));
}

void Model::zoom(float dx, float dy, float dz)
{
    this->scale = glm::scale(glm::mat4(1.0f), glm::vec3(dx, dy, dz)) * this->scale;
}

void Model::zoomTo(float x, float y, float z)
{
    this->scale = glm::scale(glm::mat4(1.0f), glm::vec3(x, y, z));
}

glm::mat4 Model::getModelMatrix() const { return this->translate * this->rotate * this->scale; }

static std::shared_ptr<Mesh> convert(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned> indices;
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vt;
        vt.pos = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        if (mesh->HasNormals()) {
            vt.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        }
        if (mesh->mTextureCoords[0]) {
            vt.texCoord = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
        vertices.push_back(vt);
    }
    for (unsigned i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned j = 0; j < face.mNumIndices; j++) indices.push_back(face.mIndices[j]);
    }
    auto pMesh = std::make_shared<Mesh>();
    pMesh->load(vertices, indices);
    return pMesh;
}

static void iterNode(aiNode *node, const aiScene *scene, std::vector<std::shared_ptr<Mesh>> &meshes)
{
    for (unsigned i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(convert(mesh, scene));
    }
    for (unsigned i = 0; i < node->mNumChildren; i++) {
        iterNode(node->mChildren[i], scene, meshes);
    }
}

bool Model::load(const std::wstring &modelFile)
{
    Assimp::Importer importer;
    std::string path = utils::ws2s(modelFile);
    const aiScene *scene = importer.ReadFile(
        path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        LOG(ERROR) << "failed to import model: " << importer.GetErrorString();
        return false;
    }
    iterNode(scene->mRootNode, scene, this->meshes);
    return true;
}

}  // namespace gl
