
#ifndef A2_CHUNK_H
#define A2_CHUNK_H

#include <Godot.hpp>
#include <Material.hpp>
#include <MeshInstance.hpp>
#include <OpenSimplexNoise.hpp>
#include <Ref.hpp>
#include <StaticBody.hpp>
#include <SurfaceTool.hpp>
#include <vector>

#include "../signals/looking_at_receiver.h"

using namespace godot;

class Chunk : public StaticBody {
    GODOT_CLASS(Chunk, StaticBody)

   private:
    static constexpr int32_t size = 1 << 7;
    static constexpr real_t height = 12.0f;
    static constexpr int32_t resolution_divider = 1 << 3;

    static std::vector<Chunk *> allocated;
    static std::vector<Chunk *> to_be_created;

    int32_t _x = 0, _y = 0;

    real_t heightmap[size + 1][size + 1] = {0};

    MeshInstance *mesh_instance = nullptr;
    SurfaceTool *tool = SurfaceTool::_new();

    Ref<Material> material;

    Chunk *left = nullptr, *right = nullptr;
    Chunk *chunkAt(int32_t x, int32_t y, OpenSimplexNoise *noise);
    Chunk *rotateToRoot(int32_t x, int32_t y, OpenSimplexNoise *noise);

    void create(int32_t x, int32_t y, OpenSimplexNoise *noise,
                bool thread_safe = true);
    void mesh();

   public:
    static void _register_methods();

    Chunk();
    ~Chunk();

    void _init();

    void _ready();

    void _process(float delta);

    friend class CustomWorld;
};

#endif
