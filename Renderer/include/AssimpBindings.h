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
    static vector<SubMesh> LoadMeshFromFile(const string& file_name) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(file_name, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
                                                                aiProcess_SortByPType | aiProcess_GenNormals |
                                                                aiProcess_RemoveRedundantMaterials);

        if (!scene) {
            throw exception(importer.GetErrorString());
        }
        vector<SubMesh> result(scene->mNumMeshes);
        for (int i = 0; i < result.size(); ++i) {
            result[i].vertices.resize(scene->mMeshes[i]->mNumVertices);
            transform(scene->mMeshes[i]->mVertices, scene->mMeshes[i]->mVertices + result[i].vertices.size(),
                      result[i].vertices.begin(), [](aiVector3D from) { return Vector3f(from.x, from.y, from.z); });
            if (scene->mMeshes[i]->HasNormals()) {
                result[i].normals.resize(scene->mMeshes[i]->mNumVertices);
                transform(scene->mMeshes[i]->mNormals, scene->mMeshes[i]->mNormals + result[i].normals.size(),
                          result[i].normals.begin(), [](aiVector3D from) { return Vector3f(from.x, from.y, from.z); });
            }
            if (scene->mMeshes[i]->HasTextureCoords(0)) {
                result[i].texcoords.resize(scene->mMeshes[i]->mNumVertices);
                transform(scene->mMeshes[i]->mTextureCoords[0],
                          scene->mMeshes[i]->mTextureCoords[0] + result[i].texcoords.size(),
                          result[i].texcoords.begin(), [](aiVector3D from) { return Vector2f(from.x, from.y); });
            }
            assert(scene->mMeshes[i]->HasFaces());
            result[i].indices.resize(3 * scene->mMeshes[i]->mNumFaces);
            for (int j = 0; j < scene->mMeshes[i]->mNumFaces; ++j) {
                assert(scene->mMeshes[i]->mFaces[j].mNumIndices == 3);
                for (int k = 0; k < 3; ++k) {
                    result[i].indices[j * 3 + k] = scene->mMeshes[i]->mFaces[j].mIndices[k];
                }
            }
            result[i].material = make_unique<DrawNormals>();
        }
        return result;
    }
};
}  // namespace renderer