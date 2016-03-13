// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "Bresenham3D.h"
#include <algorithm>
#include <tools\CoordSystem.h>

int mod(int value, int modulus) {
  return (value % modulus + modulus) % modulus;
}

float intbound(float s, float ds) {
  if (ds < 0)
    return intbound(-s, -ds);
  else 
  {
    s = mod(s, 1);
    return (1 - s) / ds;
  }
}

int signum(float x) {
  return x > 0 ? 1 : x < 0 ? -1 : 0;
}


std::tuple<glm::ivec3, glm::vec3> PickFirst(const glm::vec3 &origin, const glm::vec3 &direction, float radius, std::function<bool(const glm::ivec3 &)> collider) {
  // From "A Fast Voxel Traversal Algorithm for Ray Tracing"
  // by John Amanatides and Andrew Woo, 1987
  // <http://www.cse.yorku.ca/~amana/research/grid.pdf>
  // <http://citeseer.ist.psu.edu/viewdoc/summary?doi=10.1.1.42.3443>
  auto temp = cs::WtoWB(origin);
  int x = temp[0];
  int y = temp[1];
  int z = temp[2];
  auto dx = direction[0];
  auto dy = direction[1];
  auto dz = direction[2];
  auto stepX = signum(dx);
  auto stepY = signum(dy);
  auto stepZ = signum(dz);
  auto tMaxX = intbound(origin[0], dx);
  auto tMaxY = intbound(origin[1], dy);
  auto tMaxZ = intbound(origin[2], dz);
  auto tDeltaX = stepX / dx;
  auto tDeltaY = stepY / dy;
  auto tDeltaZ = stepZ / dz;
  auto face = glm::ivec3();
  std::vector<glm::ivec3> list;

  if (dx == 0 && dy == 0 && dz == 0)
    throw std::logic_error("Raycast in zero direction!");

  radius /= sqrt(dx*dx + dy*dy + dz*dz);

  while (true) {

    if (collider({ x, y, z }))
      break;

    if (tMaxX < tMaxY) {
      if (tMaxX < tMaxZ) {
        if (tMaxX > radius) break;
        x += stepX;
        tMaxX += tDeltaX;
        face[0] = -stepX;
        face[1] = 0;
        face[2] = 0;
      }
      else {
        if (tMaxZ > radius) break;
        z += stepZ;
        tMaxZ += tDeltaZ;
        face[0] = 0;
        face[1] = 0;
        face[2] = -stepZ;
      }
    }
    else {
      if (tMaxY < tMaxZ) {
        if (tMaxY > radius) break;
        y += stepY;
        tMaxY += tDeltaY;
        face[0] = 0;
        face[1] = -stepY;
        face[2] = 0;
      }
      else {
        if (tMaxZ > radius) break;
        z += stepZ;
        tMaxZ += tDeltaZ;
        face[0] = 0;
        face[1] = 0;
        face[2] = -stepZ;
      }
    }
  }

  return std::make_tuple(glm::ivec3(x,y,z), face);
}

std::tuple<glm::ivec3, glm::vec3> PickPrefirst(const glm::vec3 &origin, const glm::vec3 &direction, float radius, std::function<bool(const glm::ivec3 &)> collider) {
  // From "A Fast Voxel Traversal Algorithm for Ray Tracing"
  // by John Amanatides and Andrew Woo, 1987
  // <http://www.cse.yorku.ca/~amana/research/grid.pdf>
  // <http://citeseer.ist.psu.edu/viewdoc/summary?doi=10.1.1.42.3443>
  auto temp = cs::WtoWB(origin);
  int x = temp[0], last_x = x;
  int y = temp[1], last_y = y;
  int z = temp[2], last_z = z;
  auto dx = direction[0];
  auto dy = direction[1];
  auto dz = direction[2];
  auto stepX = signum(dx);
  auto stepY = signum(dy);
  auto stepZ = signum(dz);
  auto tMaxX = intbound(origin[0], dx);
  auto tMaxY = intbound(origin[1], dy);
  auto tMaxZ = intbound(origin[2], dz);
  auto tDeltaX = stepX / dx;
  auto tDeltaY = stepY / dy;
  auto tDeltaZ = stepZ / dz;
  auto face = glm::ivec3(), last_face = face;
  std::vector<glm::ivec3> list;

  if (dx == 0 && dy == 0 && dz == 0)
    throw std::logic_error("Raycast in zero direction!");

  radius /= sqrt(dx*dx + dy*dy + dz*dz);

  while (true) {

    if (collider({ x, y, z }))
      break;
    last_x = x; last_y = y; last_z = z;
    last_face = face;

    if (tMaxX < tMaxY) {
      if (tMaxX < tMaxZ) {
        if (tMaxX > radius) return{};
        x += stepX;
        tMaxX += tDeltaX;
        face[0] = -stepX;
        face[1] = 0;
        face[2] = 0;
      }
      else {
        if (tMaxZ > radius) return{};
        z += stepZ;
        tMaxZ += tDeltaZ;
        face[0] = 0;
        face[1] = 0;
        face[2] = -stepZ;
      }
    }
    else {
      if (tMaxY < tMaxZ) {
        if (tMaxY > radius) return{};
        y += stepY;
        tMaxY += tDeltaY;
        face[0] = 0;
        face[1] = -stepY;
        face[2] = 0;
      }
      else {
        if (tMaxZ > radius) return{};
        z += stepZ;
        tMaxZ += tDeltaZ;
        face[0] = 0;
        face[1] = 0;
        face[2] = -stepZ;
      }
    }
  }

  return std::make_tuple(glm::ivec3(last_x, last_y, last_z), last_face);
}

