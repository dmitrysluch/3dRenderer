#pragma once
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <exception>
#include "Mesh.h"
#include "UnlitSolidColor.h"
#include "DrawNormals.h"

namespace renderer {
class AssimpBindings {
   public:
    static shared_ptr<Mesh> LoadMeshFromFile(const string& file_name, bool loadMaterial = true) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(file_name, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
                                                                aiProcess_SortByPType | aiProcess_GenNormals |
                                                                aiProcess_RemoveRedundantMaterials | aiProcess_PreTransformVertices);

        if (!scene) {
            throw exception(importer.GetErrorString());
        }
        vector<SubMesh> submeshes(scene->mNumMeshes);
        for (int i = 0; i < submeshes.size(); ++i) {
            submeshes[i].vertices.resize(scene->mMeshes[i]->mNumVertices);
            transform(scene->mMeshes[i]->mVertices, scene->mMeshes[i]->mVertices + submeshes[i].vertices.size(),
                      submeshes[i].vertices.begin(), [](aiVector3D from) { return Vector3f(from.x, from.y, from.z); });
            if (scene->mMeshes[i]->HasNormals()) {
                submeshes[i].normals.resize(scene->mMeshes[i]->mNumVertices);
                transform(scene->mMeshes[i]->mNormals, scene->mMeshes[i]->mNormals + submeshes[i].normals.size(),
                          submeshes[i].normals.begin(), [](aiVector3D from) { return Vector3f(from.x, from.y, from.z); });
            }
            if (scene->mMeshes[i]->HasTextureCoords(0)) {
                submeshes[i].texcoords.resize(scene->mMeshes[i]->mNumVertices);
                transform(scene->mMeshes[i]->mTextureCoords[0],
                          scene->mMeshes[i]->mTextureCoords[0] + submeshes[i].texcoords.size(),
                          submeshes[i].texcoords.begin(), [](aiVector3D from) { return Vector2f(from.x, from.y); });
            }
            assert(scene->mMeshes[i]->HasFaces());
            submeshes[i].indices.resize(3 * scene->mMeshes[i]->mNumFaces);
            for (int j = 0; j < scene->mMeshes[i]->mNumFaces; ++j) {
                assert(scene->mMeshes[i]->mFaces[j].mNumIndices == 3);
                for (int k = 0; k < 3; ++k) {
                    submeshes[i].indices[j * 3 + k] = scene->mMeshes[i]->mFaces[j].mIndices[k];
                }
            }
        }
        if (!loadMaterial) {
            return make_shared<Mesh>(std::move(submeshes));
        }

    }
};
}  // namespace renderer