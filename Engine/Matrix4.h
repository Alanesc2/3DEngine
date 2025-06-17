#include <cmath>
#include <vector>
#include "Vector3.h"

using namespace std;

#ifndef MATRIX4_H
#define MATRIX4_H

class Matrix4 {
public: float arr[16] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
        Matrix4() {   
        }

        Matrix4(float values[16]) {
            for (int i = 0; i < 16; i++) {
                arr[i] = values[i];
            }
        }

        Matrix4 matrixMult(const Matrix4& other) const {
            Matrix4 obj1;

            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    float sum = 0;
                    for (int k = 0; k < 4; k++) {
                        sum += (arr[i*4 + k] * other.arr[k*4 + j]);
                    }
                    obj1.arr[i*4 + j] = sum;
                }
            }

            return obj1;
        }

        Vector3 vectorTrans(const Vector3& other) const {
            Vector3 obj1;

            obj1.x = arr[0] * other.x + arr[1] * other.y + arr[2] * other.z + arr[3];
            obj1.y = arr[4] * other.x + arr[5] * other.y + arr[6] * other.z + arr[7];
            obj1.z = arr[8] * other.x + arr[9] * other.y + arr[10] * other.z + arr[11];
            
            return obj1;
        }

        static Matrix4 translation(float x, float y, float z) {
            Matrix4 result;
            result.arr[3] = x; 
            result.arr[7] = y;  
            result.arr[11] = z; 
            return result;
        }

        static Matrix4 xRotation(float angle) {
            Matrix4 result;
            result.arr[5] = cos(angle);
            result.arr[6] = -sin(angle);
            result.arr[9] = sin(angle);
            result.arr[10] = cos(angle);
            
            return result;
        }

        static Matrix4 yRotation(float angle) {
            Matrix4 result;
            result.arr[0] = cos(angle);
            result.arr[2] = sin(angle);
            result.arr[8] = -sin(angle);
            result.arr[10] = cos(angle);
            
            return result;
        }

        static Matrix4 zRotation(float angle) {
            Matrix4 result;
            result.arr[0] = cos(angle);
            result.arr[1] = -sin(angle);
            result.arr[4] = sin(angle);
            result.arr[5] = cos(angle);
            
            return result;
        }

        static Matrix4 scale(float sx, float sy, float sz) {
            Matrix4 result;
            result.arr[0] = sx;
            result.arr[5] = sy;
            result.arr[10] = sz;
            
            return result;
        }

        static Matrix4 perspective(float fov, float aspect, float near, float far) {
            Matrix4 result;
            float tanHalfFov = tan(fov / 2.0f);
            
            result.arr[0] = 1.0f / (aspect * tanHalfFov);
            result.arr[5] = 1.0f / tanHalfFov;
            result.arr[10] = -(far + near) / (far - near);
            result.arr[11] = -1.0f;
            result.arr[14] = -(2.0f * far * near) / (far - near);
            result.arr[15] = 0.0f;
            
            return result;
        }

        static Matrix4 viewMatrix(Vector3 position, Vector3 target, Vector3 up) {
            Matrix4 result;
            Vector3 forward = (target - position).normalize();
            Vector3 right = forward.crossProduct(up);
            up = right.crossProduct(forward);

            result.arr[0] = right.x;
            result.arr[1] = right.y;
            result.arr[2] = right.z;
            result.arr[3] = -right.dotProduct(position);
            result.arr[4] = up.x;
            result.arr[5] = up.y;
            result.arr[6] = up.z;
            result.arr[7] = up.dotProduct(position);
            result.arr[8] = -forward.x;
            result.arr[9] = -forward.y;
            result.arr[10] = -forward.z;
            result.arr[11] = forward.dotProduct(position);

            return result;
        }
};

#endif