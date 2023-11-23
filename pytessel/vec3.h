#ifndef _VEC3_H
#define _VEC3_H

class Vec3 {
public:
    float x = 0.0;
    float y = 0.0;
    float z = 0.0;

    Vec3() {}

    Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    friend Vec3 operator*(const mat33& lhs, const Vec3& rhs) {
        return Vec3(0,0,0);
    }

    friend Vec3 operator*(float v, const Vec3& rhs) {
        return Vec3(v * rhs.x, v * rhs.y, v * rhs.z);
    }

    friend Vec3 operator*(const Vec3& rhs, float v) {
        return Vec3(v * rhs.x, v * rhs.y, v * rhs.z);
    }

    Vec3 normalized() const {
        float l = std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
        return Vec3(this->x / l, this->y / l, this->z / l);
    }

    float dot(const Vec3& rhs) const {
        return this->x * rhs.x + this->y * rhs.y + this->z * rhs.z;
    }

    friend Vec3 operator+(const Vec3& lhs, const Vec3& rhs) {
        return Vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
    }

    friend Vec3 operator-(const Vec3& lhs, const Vec3& rhs) {
        return Vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
    }

    void operator-=(const Vec3& rhs) {
        this->x -= rhs.x;
        this->y -= rhs.y;
        this->z -= rhs.z;
    }

    friend Vec3 operator/(const Vec3& rhs, float v) {
        return Vec3(rhs.x / v, rhs.y / v, rhs.z / v);
    }

    Vec3 cross(const Vec3& rhs) const {
        return Vec3(
            this->y * rhs.z - this->z * rhs.y,
            this->z * rhs.x - this->x * rhs.z,
            this->x * rhs.y - this->y * rhs.x
        );
    }
};

#endif // _VEC3_H
