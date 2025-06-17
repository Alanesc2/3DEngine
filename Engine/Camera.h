#include <cmath>
#include <vector>
#include "Vector3.h"

using namespace std;

#ifndef CAMERA_H
#define CAMERA_H

class Camera {
public: Vector3 position, direction, up, right;
        float fov, aspectRatio, nearPlane, farPlane, yaw,
        pitch, movementSpeed, mouseSensitivity;

        Camera() {
            position = Vector3(0, 0, 0);
            fov = 45;
            aspectRatio = 16.0f;
            nearPlane = 0.1;
            farPlane = 100;
            yaw = 0.0f;
            pitch = 0.0f;
            movementSpeed = 2.5f;
            mouseSensitivity = 0.1f;

            // Initialize direction, up, and right based on initial yaw and pitch
            rotate(yaw, pitch);
        }

        void moveForward(float distance) {
            position = position + (direction * distance);
        }

        void moveRight(float distance) {
            position = position + (right * distance);
        }

        void moveUp(float distance) {
            position = position + (up * distance);
        }

        void rotate(float yaw, float pitch) {
            this->pitch = max(pitch, -89.0f);
            this->pitch = min(pitch, 89.0f);
            this->yaw = yaw;

            float pitchRad = this->pitch * (M_PI / 180);
            float yawRad = this->yaw * (M_PI / 180);

            direction.x = cos(pitchRad) * cos(yawRad);
            direction.y = sin(pitchRad);
            direction.z = cos(pitchRad) * sin(yawRad);
            direction = direction.normalize();

            // Recalculate Right and Up vectors robustly
            Vector3 worldUp(0.0f, 1.0f, 0.0f);
            
            // Check if looking nearly straight up or down to prevent degenerate right vector
            if (abs(direction.y) > 0.999f) { // If direction is almost parallel to worldUp
                // Use world's X-axis as a temporary right vector if looking straight up/down
                right = Vector3(1.0f, 0.0f, 0.0f);
            } else {
                right = direction.crossProduct(worldUp);
            }
            right = right.normalize();
            
            up = right.crossProduct(direction);
            up = up.normalize();
        }

private:


};

#endif