#include "ledge_detector.h"

#include <math.h>

#include <BoxShape.hpp>
#include <CapsuleShape.hpp>
#include <CollisionShape.hpp>
#include <CubeMesh.hpp>
#include <Dictionary.hpp>
#include <Math.hpp>
#include <MeshInstance.hpp>
#include <PhysicsDirectSpaceState.hpp>
#include <PhysicsShapeQueryParameters.hpp>
#include <Ref.hpp>
#include <Shape.hpp>
#include <SpatialMaterial.hpp>
#include <Transform.hpp>
#include <World.hpp>

#include "../debug.h"

void LedgeDetector::_register_methods() {
    register_method("_ready", &LedgeDetector::_ready);
    register_property<LedgeDetector, float>("min_height",
                                            &LedgeDetector::min_height, 1);
    register_property<LedgeDetector, int>("ray_casts",
                                          &LedgeDetector::ray_casts, 8);
    register_property<LedgeDetector, NodePath>(
        "shape_path", &LedgeDetector::shape_path, NodePath());
    register_property<LedgeDetector, float>(
        "max_hangable_angle", &LedgeDetector::max_hangable_angle, 30);
    register_property<LedgeDetector, bool>(
        "debug_hangable", &LedgeDetector::debug_hangable, false);
}

void LedgeDetector::_init() {
    min_height = 1;
    ray_casts = 8;
    max_hangable_angle = 30;
    debug_hangable = false;
    shape_path = NodePath();
}

void LedgeDetector::_ready() {
    dev_assert(!shape_path.is_empty());

    CollisionShape* shape =
        Object::cast_to<CollisionShape>(get_node(shape_path));
    dev_assert(shape != nullptr);

    Ref<Shape> ref = shape->get_shape();
    CapsuleShape* capsule_shape = Object::cast_to<CapsuleShape>(ref.ptr());
    dev_assert(capsule_shape != nullptr);

    float radius = capsule_shape->get_radius();
    float total_shape_height = radius * 2 + capsule_shape->get_height();

    // vertical ray cast diagram
    // https://utexas.box.com/s/fsjac602cv83t5zxo5vmtrdfe09hgiry
    // vertical ray cast is for detecting ledges

    // add a little more to make sure it's outside of the object
    v_raycast_offset.x = radius + .01f;

    // start at top of object because of possible obstructions right in front of
    // the object
    v_raycast_offset.y = total_shape_height / 2;

    // min height is supposed to be the min height to classify a ledge. since we
    // start at top of the object we add the shape's total height and the min
    // height to get the total length of the vector
    v_raycast_len = min_height + total_shape_height;

    // horizontal ray cast diagram
    // https://utexas.box.com/s/usyw8j2hrayqa67xs9336zygkbbeajdc
    // horizontal ray cast is for determining the location of the ledge and
    // obtaining it's normal and a position on the ledge. this information is
    // used for the area check below

    // add a little more to make sure it's outside of the object
    h_raycast_offset.x = radius + .01f;

    // start below the object and half of ledge min height to get more accurate
    // ledge normals
    h_raycast_offset.y = -(total_shape_height + min_height) / 2;

    // take into account that the player can walk slightly off the ledge and
    // still be able to ledge hang
    h_raycast_len = radius * 2;

    // area visual diagram
    // https://utexas.box.com/s/e1deeop2ulmk7y4lw1i95w4ubes4tzcd
    // used for seeing if there are any obstructions that would prevent a player
    // from hanging on a ledge
    area_shape_extents.x = radius;
    area_shape_extents.y = v_raycast_len / 2;
    area_shape_extents.z = radius;

    area_offset.x = radius + .01f;
    area_offset.y = total_shape_height / 2;
}

/**
 * Checks if this object is on a ledge
 * @param direction the direction of where the first ray cast should be relative
 * to origin
 * @param exclude the objects to exclude when checking collision
 * @return true if this object is on a ledge, false otherwise
 */
bool LedgeDetector::is_on_ledge(Vector3 direction, Array exclude) {
    return find_ledges(direction, exclude).size() > 0;
}

/**
 * Finds the locations of ledges from the object's origin
 * @param direction the direction of where the first ray cast should be relative
 * to origin
 * @param exclude the objects to exclude when checking collision
 * @return a list of the ledges' x and z offsets from the object's origin
 */
vector<Vector3> LedgeDetector::find_ledges(Vector3 direction, Array exclude) {
    vector<Vector3> ret;

    if (direction.x == 0 && direction.z == 0) return ret;

    prepare_direction(&direction);

    Vector3 origin = get_global_transform().get_origin();

    // these two should be called in _physics_process only
    World* world = get_world().ptr();
    PhysicsDirectSpaceState* direct_state = world->get_direct_space_state();

    float rotation_delta = Math::deg2rad(360.0f / ray_casts);

    Vector3 cur_offset =
        make_offset_point(direction, v_raycast_offset.x, v_raycast_offset.y);
    Vector3 raycast_vec = Vector3::DOWN * v_raycast_len;

    for (int i = 0; i < ray_casts; i++) {
        Vector3 cur_origin = origin + cur_offset;

        Dictionary collision = direct_state->intersect_ray(
            cur_origin, cur_origin + raycast_vec, exclude);

        // empty means ledge found
        if (collision.empty()) {
            cur_origin.y = 0;
            ret.push_back(cur_origin);
        }

        cur_offset.rotate(Vector3::UP, rotation_delta);
    }

    return ret;
}

/**
 * Checks if there is a hangable ledge in the given direction
 * @param direction the direction of where the first ray cast should be relative
 * to origin
 * @param exclude the objects to exclude when checking collision
 * @return the location of where the object would be placed, or the infinity
 * vector if none found
 */
Vector3 LedgeDetector::find_ledge_hangable(Vector3 direction, Array exclude) {
    if (direction.x == 0 && direction.z == 0) return Vector3::INF;

    prepare_direction(&direction);

    Vector3 origin = get_global_transform().get_origin();

    // these two should be called in _physics_process only
    World* world = get_world().ptr();
    PhysicsDirectSpaceState* direct_state = world->get_direct_space_state();

    // check for ledge
    Vector3 v_offset_point =
        make_offset_point(direction, v_raycast_offset.x, v_raycast_offset.y);
    Vector3 v_raycast_origin = origin + v_offset_point;
    Vector3 v_raycast_vec = Vector3::DOWN * v_raycast_len;

    Dictionary ray_collision = direct_state->intersect_ray(
        v_raycast_origin, v_raycast_origin + v_raycast_vec, exclude);

    // not empty means that there is no ledge
    if (!ray_collision.empty()) return Vector3::INF;

    // get ledge normal and position
    Vector3 h_offset_point =
        make_offset_point(direction, h_raycast_offset.x, h_raycast_offset.y);
    Vector3 h_raycast_origin = origin + h_offset_point;
    Vector3 h_raycast_vec = -direction * h_raycast_len;

    ray_collision = direct_state->intersect_ray(
        h_raycast_origin, h_raycast_origin + h_raycast_vec, exclude);

    if (ray_collision.empty()) return Vector3::INF;

    // hangable ledges must be within the max hangable angle
    Vector3 normal = ray_collision["normal"];

    float cur_angle = Math::rad2deg(normal.y);
    if (abs(cur_angle) > max_hangable_angle) return Vector3::INF;

    // make sure that there's no obstructions against the player

    // set origin relative to ledge normal and position
    Vector3 position = ray_collision["position"];
    Vector3 normal_up = get_orthogonal_up(normal);
    Vector3 area_origin =
        position + normal * area_offset.x + normal_up * area_offset.y;

    // rotate such that it's parallel to ledge's normal
    Transform transform;
    transform.set_look_at(area_origin, area_origin + normal, Vector3::UP);

    if (debug_hangable) {
        Area* area = make_area(area_shape_extents);
        add_child(area);
        area->set_as_toplevel(true);
        area->set_transform(transform);
    }

    BoxShape* box_shape = BoxShape::_new();
    box_shape->set_extents(area_shape_extents);
    Ref<Resource> shape(box_shape);

    PhysicsShapeQueryParameters* params = PhysicsShapeQueryParameters::_new();
    params->set_shape(shape);
    params->set_transform(transform);
    params->set_exclude(exclude);

    Ref<PhysicsShapeQueryParameters> query(params);
    Dictionary area_collision = direct_state->get_rest_info(query);

    return area_collision.empty() ? area_origin : Vector3::INF;
}

/**
 * Modifies the given vector to be used as horizontal direction for ledge ray
 * casts
 * @param direction the vector to be modified
 */
void LedgeDetector::prepare_direction(Vector3* direction) {
    direction->y = 0;
    direction->normalize();
}

/**
 * Creates a 3D point as a vector in the given direction with the given
 * horizontal and vertical offset
 * @param direction the horizontal direction the vector will go in
 * @param h_offset the horizontal offset
 * @param v_offset the vertical offset
 * @return a 3D point in the given direction with the given horizontal and
 * vertical offset
 */
Vector3 LedgeDetector::make_offset_point(Vector3 direction, float h_offset,
                                         float v_offset) {
    Vector3 ret = direction * h_offset;
    ret.y = v_offset;
    return ret;
}

/**
 * Creates a light pink, transparent area object with the given extents. Used
 * for debugging.
 */
Area* LedgeDetector::make_area(Vector3 extents) {
    Area* area = Area::_new();

    CollisionShape* collision_shape = CollisionShape::_new();
    BoxShape* box_shape = BoxShape::_new();
    box_shape->set_extents(extents);
    collision_shape->set_shape(Ref<Shape>(box_shape));

    MeshInstance* mesh_instance = MeshInstance::_new();
    CubeMesh* cube_mesh = CubeMesh::_new();
    cube_mesh->set_size(extents * 2);
    mesh_instance->set_mesh(Ref<Mesh>(cube_mesh));

    SpatialMaterial* spatial_material = SpatialMaterial::_new();
    spatial_material->set_feature(SpatialMaterial::FEATURE_TRANSPARENT, true);

    // light pink
    spatial_material->set_albedo(Color(1, .71f, .76f, .5f));
    mesh_instance->set_material_override(Ref<Material>(spatial_material));

    area->add_child(collision_shape);
    area->add_child(mesh_instance);

    return area;
}

/**
 * Rotates the given vector 90 degrees such that it points upwards
 * @return the given vector rotated 90 degrees such that it points upwards
 */
Vector3 LedgeDetector::get_orthogonal_up(Vector3 v) {
    Vector3 axis = v.rotated(Vector3::UP, Math::deg2rad(90.0f));
    Vector3 res = v.rotated(axis, Math::deg2rad(-90.0f));
    return res;
}