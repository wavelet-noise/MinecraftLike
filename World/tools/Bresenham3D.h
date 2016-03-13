// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#pragma once
#ifndef Bresenham3D_h__
#define Bresenham3D_h__

#include <glm/glm.hpp>
#include <vector>
#include <functional>

std::tuple<glm::ivec3, glm::vec3> PickFirst(const glm::vec3 &origin, const glm::vec3 &direction, float radius, std::function<bool(const glm::ivec3 &)> collider);
std::tuple<glm::ivec3, glm::vec3> PickPrefirst(const glm::vec3 &origin, const glm::vec3 &direction, float radius, std::function<bool(const glm::ivec3 &)> collider);

#endif // Bresenham3D_h__
