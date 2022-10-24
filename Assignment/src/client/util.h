#ifndef A2_UTIL_H
#define A2_UTIL_H

#include <Node.hpp>

#include "./common.h"

class Util {
   private:
    Util();

   public:
    static Vector3 vector3_sign(Vector3 vector);
    static bool is_only_digits(String s);
    static void delete_children(Node *node);
    static String get_time_str(int seconds);
};

#endif