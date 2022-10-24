#ifndef A2_LEDGE_DETECTOR_H
#define A2_LEDGE_DETECTOR_H

#include <Area.hpp>
#include <Spatial.hpp>
#include <vector>

#include "../common.h"

class LedgeDetector : public Spatial {
    GODOT_CLASS(LedgeDetector, Spatial);

   private:
    NodePath shape_path;
    int ray_casts;
    float min_height;

    Vector2 v_raycast_offset;
    Vector2 h_raycast_offset;
    float v_raycast_len;
    float h_raycast_len;

    Vector3 area_shape_extents;
    Vector2 area_offset;

    float max_hangable_angle;
    bool debug_hangable;

    static void prepare_direction(Vector3 *direction);
    static Vector3 make_offset_point(Vector3 direction, float h_offset,
                                     float v_offset);
    static Area *make_area(Vector3 extents);
    static Vector3 get_orthogonal_up(Vector3 vector);

   public:
    static void _register_methods();

    void _init();
    void _ready();

    bool is_on_ledge(Vector3 direction, Array exclude);
    vector<Vector3> find_ledges(Vector3 direction, Array exclude);
    Vector3 find_ledge_hangable(Vector3 direction, Array exclude);
};

#endif