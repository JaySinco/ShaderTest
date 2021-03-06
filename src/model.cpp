#include "model.h"
#include "utils.h"
#include <limits>
#include <filesystem>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace gl
{
void Model::draw(Shader &shader, Camera &camera) const
{
    glm::mat4 modelMatrix = this->getModelMatrix();
    glm::mat4 viewMatrix = camera.getViewMatrix();
    glm::mat4 projectionMatrix = camera.getProjectionMatrix();
    glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;

    shader.set("uf_ModelMatrix", modelMatrix);
    shader.set("uf_ViewMatrix", viewMatrix);
    shader.set("uf_ProjectionMatrix", projectionMatrix);
    shader.set("uf_ModelViewMatrix", modelViewMatrix);
    shader.set("uf_ModelViewProjectionMatrix", projectionMatrix * modelViewMatrix);
    shader.set("uf_NormalMatrix", glm::inverseTranspose(glm::mat3(modelViewMatrix)));

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

void Model::spin(float ddegree, float axis_x, float axis_y, float axis_z)
{
    this->rotate =
        glm::rotate(glm::mat4(1.0f), glm::radians(ddegree), glm::vec3(axis_x, axis_y, axis_z)) *
        this->rotate;
}

void Model::zoom(float dx, float dy, float dz)
{
    this->scale = glm::scale(glm::mat4(1.0f), glm::vec3(dx, dy, dz)) * this->scale;
}

glm::mat4 Model::getModelMatrix() const
{
    return this->translate * this->rotate * this->scale * this->init;
}

struct BoundingRect
{
    glm::vec3 lower;
    glm::vec3 high;
};

static std::shared_ptr<Mesh> convert(aiMesh *mesh, const aiScene *scene, BoundingRect &rect)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned> indices;
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
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
    for (unsigned i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for (unsigned j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
            rect.lower.x = std::min(rect.lower.x, mesh->mVertices[face.mIndices[j]].x);
            rect.high.x = std::max(rect.high.x, mesh->mVertices[face.mIndices[j]].x);
            rect.lower.y = std::min(rect.lower.y, mesh->mVertices[face.mIndices[j]].y);
            rect.high.y = std::max(rect.high.y, mesh->mVertices[face.mIndices[j]].y);
            rect.lower.z = std::min(rect.lower.z, mesh->mVertices[face.mIndices[j]].z);
            rect.high.z = std::max(rect.high.z, mesh->mVertices[face.mIndices[j]].z);
        }
    }
    auto pMesh = std::make_shared<Mesh>();
    pMesh->load(vertices, indices);
    LOG(INFO) << "-- " << mesh->mName.C_Str() << ", faces=" << mesh->mNumFaces;
    return pMesh;
}

static void iterNode(aiNode *node, const aiScene *scene, std::vector<std::shared_ptr<Mesh>> &meshes,
                     BoundingRect &rect)
{
    for (unsigned i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(convert(mesh, scene, rect));
    }
    for (unsigned i = 0; i < node->mNumChildren; i++) {
        iterNode(node->mChildren[i], scene, meshes, rect);
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
    LOG(INFO) << "loading \"" << utils::ws2s(std::filesystem::path(modelFile).filename()) << "\"";
    BoundingRect rect;
    rect.lower = glm::vec3(std::numeric_limits<float>::max());
    rect.high = glm::vec3(std::numeric_limits<float>::min());
    this->meshes.clear();
    iterNode(scene->mRootNode, scene, this->meshes, rect);
    glm::vec3 center = (rect.high + rect.lower) / 2.0f;
    glm::vec3 size = rect.high - rect.lower;
    float factor = 2.0f / std::max({size.x, size.y, size.z});
    this->init =
        glm::scale(glm::mat4(1.0f), glm::vec3(factor)) * glm::translate(glm::mat4(1.0f), -center);
    this->reset();
    return true;
}

}  // namespace gl
