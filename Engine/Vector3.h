#include <cmath>
#include <vector>

using namespace std;

#ifndef VECTOR3_H
#define VECTOR3_H

class Vector3 {
public: float x, y, z;
        Vector3() {
            x = 0;
            y = 0;
            z = 0;
        }
        
        Vector3(float xCord, float yCord, float zCord) {
            x = xCord;
            y = yCord;
            z = zCord;
        };

        Vector3 operator+(const Vector3& other) const {
            Vector3 obj1;
            obj1.x = x + other.x;
            obj1.y = y + other.y;
            obj1.z = z + other.z;

            return obj1;
        }

        Vector3 operator-(const Vector3& other) const {
            Vector3 obj1;
            obj1.x = x - other.x;
            obj1.y = y - other.y;
            obj1.z = z - other.z;

            return obj1;
        }

        Vector3 operator*(float scalar) const {
            Vector3 obj1;
            obj1.x = x * scalar;
            obj1.y = y * scalar;
            obj1.z = z * scalar;

            return obj1;
        }

        bool operator==(const Vector3& other) const {
            if (x == other.x && y == other.y && z == other.z) return true;

            else return false;
        }

        bool operator!=(const Vector3& other) const {
            return !this->operator==(other);
        }

        float dotProduct(const Vector3& other) const {
            return x*other.x + y*other.y + z*other.z;
        }

        Vector3 crossProduct(const Vector3& other) const {
            Vector3 obj1;
            obj1.x = y*other.z - z*other.y;
            obj1.y = z*other.x - x*other.z;
            obj1.z = x*other.y - y*other.x;

            return obj1;
        }

        float length() const {
            return sqrt((x * x) + (y * y) + (z * z));
        }

        Vector3 normalize() const {
            Vector3 obj1;
            float len = this->length();

            if (len > 0) {
                obj1.x = x / len;
                obj1.y = y / len;
                obj1.z = z / len;
            }

            return obj1;
        }

        float distance(const Vector3& other) const{
            return (float) (*this - other).length();
        }
};

#endif