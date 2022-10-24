
#include "chunk.h"

#include <ArrayMesh.hpp>
#include <GlobalConstants.hpp>
#include <PoolArrays.hpp>
#include <Ref.hpp>
#include <Shape.hpp>
#include <ResourceLoader.hpp>

using namespace godot;

std::vector<Chunk *> Chunk::allocated;
std::vector<Chunk *> Chunk::to_be_created;

void Chunk::_register_methods() {
    register_method("_process", &Chunk::_process);
    register_method("_ready", &Chunk::_ready);
}

Chunk::Chunk() {
    material = ResourceLoader::get_singleton()->load("res://assets/materials/ground.tres");
}

Chunk::~Chunk() {}

void Chunk::_init() {}

void Chunk::_ready() {}

void Chunk::_process(float delta) { }

void Chunk::create(int32_t x, int32_t y, OpenSimplexNoise *noise, bool thread_safe) {
    _x = x; _y = y;

    for(int32_t i = 0; i <= size; i++) {
        for(int32_t j = 0; j <= size; j++) {
            this->heightmap[i][j] = noise->get_noise_2d((real_t) x + i, (real_t) y + j) * height;
        }
    }
    if(!thread_safe) {
        Chunk::to_be_created.push_back(this);
        return;
    }
    this->set_translation(Vector3((real_t) _x, 0.0f, (real_t) _y));
    if(this->mesh_instance) {
        this->mesh_instance->call_deferred("queue_free");
    }

    mesh_instance = MeshInstance::_new();
    Ref<Mesh> mesh{ArrayMesh::_new()};

    tool->begin(Mesh::PRIMITIVE_TRIANGLES);

    this->mesh();

    tool->generate_normals();
    tool->commit(mesh);
    mesh_instance->set_mesh(mesh);
    this->add_child(mesh_instance);
    mesh_instance->set_material_override(material);
    mesh_instance->create_trimesh_collision();
}

void Chunk::mesh() {
    for(int i = 0; i < size; i += resolution_divider) {
        for(int j = 0; j < size; j += resolution_divider) {
            auto pool = PoolVector3Array();
            pool.push_back(Vector3((real_t) i, heightmap[i][j], (real_t) j));
            pool.push_back(Vector3((real_t) i+resolution_divider, heightmap[i+resolution_divider][j], (real_t) j));
            pool.push_back(Vector3((real_t) i, heightmap[i][j+resolution_divider], (real_t) j+resolution_divider));
            tool->add_triangle_fan(pool);
            pool = PoolVector3Array();
            pool.push_back(Vector3((real_t) i, heightmap[i][j+resolution_divider], (real_t) j+resolution_divider));
            pool.push_back(Vector3((real_t) i+resolution_divider, heightmap[i+resolution_divider][j], (real_t) j));
            pool.push_back(Vector3((real_t) i+resolution_divider, heightmap[i+resolution_divider][j+resolution_divider], (real_t) j+resolution_divider));
            tool->add_triangle_fan(pool);
        }
    }
}

Chunk *Chunk::chunkAt(int32_t x, int32_t y, OpenSimplexNoise *noise) {
    if(x == this->_x && y == this->_y) { return this; }
    if(x < this->_x || (x == this->_x && y < this->_y)) {
        if(this->left) {
            return left->chunkAt(x, y, noise);
        }
        auto c = Chunk::allocated.back(); Chunk::allocated.pop_back();
        c->create(x, y, noise, false);
        this->left = c;
        return c;
    }
    if(this->right) {
        return right->chunkAt(x, y, noise);
    }
    auto c = Chunk::allocated.back(); Chunk::allocated.pop_back();
    c->create(x, y, noise, false);
    this->right = c;
    return c;
}

Chunk *Chunk::rotateToRoot(int32_t x, int32_t y, OpenSimplexNoise *noise) {
    if(chunkAt(x, y, noise) == this) { return this; }
    if(x < this->_x || (x == this->_x && y < this->_y)) {
        auto ret = this->left->rotateToRoot(x, y, noise);
        this->left = ret->right;
        ret->right = this;
        return ret;
    }
    auto ret = this->right->rotateToRoot(x, y, noise);
    this->right = ret->left;
    ret->left = this;
    return ret;
}
