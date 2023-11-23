#ifndef _VEC3_H
#define _VEC3_H

class Vec3 {
public:
    float x = 0.0;
    float y = 0.0;
    float z = 0.0;

    Vec3();

    Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

};

#endif // _VEC3_H
