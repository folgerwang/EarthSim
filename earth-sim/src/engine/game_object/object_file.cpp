#include <string>
#include <sstream>
#include <unordered_map>
#include "object_file.h"
#include "engine/engine_helper.h"

namespace engine {
namespace {
uint32_t getNewIndex(
    std::unordered_map<glm::uvec3, uint32_t>& index_map,
    std::vector<glm::vec3>& new_vertices,
    std::vector<glm::vec3>& new_normals,
    std::vector<glm::vec2>& new_uvs,
    const std::vector<glm::vec3>& vertices,
    const std::vector<glm::vec3>& normals,
    const std::vector<glm::vec2>& uvs,
    const glm::uvec3& idx) {
    uint32_t result = 0;
    if (index_map.find(idx) != index_map.end()) {
        result = index_map[idx];
    }
    else {
        auto i = static_cast<uint32_t>(new_vertices.size());
        index_map[idx] = i;
        result = i;
        new_vertices.push_back(vertices[idx.x-1]);
        if (idx.y <= normals.size()) {
            new_normals.push_back(normals[idx.y-1]);
        }
        if (idx.z <= uvs.size()) {
            new_uvs.push_back(uvs[idx.z-1]);
        }
    }

    return result;
}

void createUnifiedMesh(
    std::vector<glm::vec3>& new_vertices,
    std::vector<glm::vec3>& new_normals,
    std::vector<glm::vec2>& new_uvs,
    std::vector<glm::uvec3>& new_faces,
    const std::vector<glm::vec3>& vertices,
    const std::vector<glm::vec3>& normals,
    const std::vector<glm::vec2>& uvs,
    const std::vector<glm::uvec3>& v_faces,
    const std::vector<glm::uvec3>& vn_faces,
    const std::vector<glm::uvec3>& vt_faces) {

    std::unordered_map<glm::uvec3, uint32_t> index_map;

    for (auto face_idx = 0; face_idx < v_faces.size(); face_idx++) {
        glm::uvec3 new_face;
        const auto& v_face = v_faces[face_idx];
        const auto& vn_face = vn_faces[face_idx];
        const auto& vt_face = vt_faces[face_idx];

        auto i0 = glm::uvec3(v_face.x, vn_face.x, vt_face.x);
        auto i1 = glm::uvec3(v_face.y, vn_face.y, vt_face.y);
        auto i2 = glm::uvec3(v_face.z, vn_face.z, vt_face.z);
        new_face.x = getNewIndex(index_map, new_vertices, new_normals, new_uvs, vertices, normals, uvs, i0);
        new_face.y = getNewIndex(index_map, new_vertices, new_normals, new_uvs, vertices, normals, uvs, i1);
        new_face.z = getNewIndex(index_map, new_vertices, new_normals, new_uvs, vertices, normals, uvs, i2);
        new_faces.push_back(new_face);
    }
}
}

namespace game_object {

ObjectMesh::ObjectMesh(const renderer::DeviceInfo& device_info, const std::string& object_name) : device_info_(device_info){
    uint64_t buffer_size = 0;
    auto buffer_data = engine::helper::readFile(object_name, buffer_size);
    std::string obj_string((char*)buffer_data.data(), buffer_size);
    std::stringstream obj_stream(std::move(obj_string));
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    std::vector<glm::uvec3> v_faces;
    std::vector<glm::uvec3> vn_faces;
    std::vector<glm::uvec3> vt_faces;

    bool start_process_faces = false;
    bool has_normal = false;
    bool has_texture = false;
    while (!obj_stream.eof()) {
        std::string line;
        getline(obj_stream, line);

        std::stringstream line_stream(line);

        std::string tag;
        line_stream >> tag;
        if (tag == "#" || tag == "") {
            continue;
        }

        if (tag == "v") {
            float x, y, z;
            line_stream >> x >> y >> z;
            vertices.push_back(glm::vec3(x, y, z));
        }

        if (tag == "vn") {
            float x, y, z;
            line_stream >> x >> y >> z;
            normals.push_back(glm::vec3(x, y, z));
            has_normal = true;
        }

        if (tag == "vt") {
            float x, y;
            line_stream >> x >> y;
            uvs.push_back(glm::vec2(x, y));
            has_texture = true;
        }

        if (tag == "f") {
            start_process_faces = true;
            std::string is0, is1, is2;
            line_stream >> is0 >> is1 >> is2;
            glm::uvec3 i0, i1, i2;
            sscanf_s(is0.data(), "%u/%u/%u", &i0.x, &i1.x, &i2.x);
            sscanf_s(is1.data(), "%u/%u/%u", &i0.y, &i1.y, &i2.y);
            sscanf_s(is2.data(), "%u/%u/%u", &i0.z, &i1.z, &i2.z);

            v_faces.push_back(i0);
            vt_faces.push_back(i1);
            vn_faces.push_back(i2);
        }

        // end of process one object.
        if (start_process_faces && tag != "f") {
            std::vector<glm::vec3> new_vertices;
            std::vector<glm::vec3> new_normals;
            std::vector<glm::vec2> new_uvs;
            std::vector<glm::uvec3> new_faces;

            createUnifiedMesh(
                new_vertices,
                new_normals,
                new_uvs,
                new_faces,
                vertices,
                normals,
                uvs,
                v_faces,
                vn_faces,
                vt_faces);

            vertices.clear();
            normals.clear();
            uvs.clear();
            v_faces.clear();
            vn_faces.clear();
            vt_faces.clear();
        }
    }
}

} // game_object
} // engine