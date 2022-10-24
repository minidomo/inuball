#include "./util.h"

#include <Ref.hpp>
#include <RegEx.hpp>
#include <RegExMatch.hpp>
#include <memory>

Vector3 Util::vector3_sign(Vector3 vector) {
    Vector3 ret(vector);
    ret.x = ret.x < 0 ? -1.0f : 1.0f;
    ret.y = ret.y < 0 ? -1.0f : 1.0f;
    ret.z = ret.z < 0 ? -1.0f : 1.0f;
    return ret;
}

bool Util::is_only_digits(String s) {
    RegEx *regex = RegEx::_new();
    regex->compile("^\\d+$");
    return regex->search(s).is_valid();
}

void Util::delete_children(Node *node) {
    Array children = node->get_children();
    for (int i = 0; i < children.size(); i++) {
        node->remove_child(children[i]);
    }
}

String Util::get_time_str(int seconds) {
    std::unique_ptr<char[]> buf(new char[6]);
    std::snprintf(buf.get(), 6, "%02d:%02d", seconds / 60, seconds % 60);
    return String(buf.get());
}
