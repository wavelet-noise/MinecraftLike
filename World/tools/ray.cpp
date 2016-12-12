#include "ray.h"
namespace glm {

  ray normalize(const ray &x)
  {
    return ray(x.mOrigin, normalize(x.mDir));
  }

  ray::ray(glm::vec3 p, glm::vec3 d) : mOrigin(p), mDir(d) {
    mDir = normalize(mDir);
    inv = glm::vec3(1 / mDir.x, 1 / mDir.y, 1 / mDir.z);
    sign[0] = inv.x < 0;
    sign[1] = inv.y < 0;
    sign[2] = inv.z < 0;
  }

  ray::ray(const glm::ray & r) : mOrigin(r.mOrigin), mDir(r.mDir) {
    mDir = normalize(mDir);
    inv = glm::vec3(1 / mDir.x, 1 / mDir.y, 1 / mDir.z);
    sign[0] = inv.x < 0;
    sign[1] = inv.y < 0;
    sign[2] = inv.z < 0;
  }

  ray::~ray()
  {
  }

  glm::vec3 ray::origin()
  {
    return mOrigin;
  }

  glm::vec3 ray::dir()
  {
    return mDir;
  }

  void ray::precompute()
  {
    mDir = normalize(mDir);
    inv = vec3(1 / mDir.x, 1 / mDir.y, 1 / mDir.z);
    sign[0] = inv.x < 0;
    sign[1] = inv.y < 0;
    sign[2] = inv.z < 0;
  }

  bool intersect(const ray &r, float t0, float t1, const vec3 &min, const vec3 &max)
  {
    vec3 bounds[2] = { min, max };
    float tmin, tmax, tymin, tymax, tzmin, tzmax;

    tmin = (bounds[r.sign[0]].x - r.mOrigin.x) * r.inv.x;
    tmax = (bounds[1 - r.sign[0]].x - r.mOrigin.x) * r.inv.x;
    tymin = (bounds[r.sign[1]].y - r.mOrigin.y) * r.inv.y;
    tymax = (bounds[1 - r.sign[1]].y - r.mOrigin.y) * r.inv.y;

    if ((tmin > tymax) || (tymin > tmax))
      return false;

    if (tymin > tmin)
      tmin = tymin;
    if (tymax < tmax)
      tmax = tymax;

    tzmin = (bounds[r.sign[2]].z - r.mOrigin.z) * r.inv.z;
    tzmax = (bounds[1 - r.sign[2]].z - r.mOrigin.z) * r.inv.z;

    if ((tmin > tzmax) || (tzmin > tmax))
      return false;

    if (tzmin > tmin)
      tmin = tzmin;
    if (tzmax < tmax)
      tmax = tzmax;

    return ((tmin < t1) && (tmax > t0));
  }

  plane::plane() : a(0), b(0), c(0), d(0)
  {

  }

  plane::plane(const vec3 &center, const vec3 &vec1, const vec3 &vec2)
  {
    vec3 normal = glm::cross(vec1, vec2);

    vec3 n = glm::normalize(normal);
    a = n.x;
    b = n.y;
    c = n.z;
    d = -dot(center, n);
  }

  plane::plane(const vec3& normal, const glm::vec3& point)
  {
    vec3 n = glm::normalize(normal);
    a = n.x;
    b = n.y;
    c = n.z;
    d = -glm::dot(point, n);
  }

  plane::plane(float _a, float _b, float _c, float _d) : a(_a), b(_b), c(_c), d(_d)
  {
  }

  plane plane::normalize(const plane &pl)
  {
    plane res;
    float dist = glm::sqrt(pl.a * pl.a + pl.b * pl.b + pl.c * pl.c);
    res.a = pl.a / dist;
    res.b = pl.b / dist;
    res.c = pl.c / dist;
    res.d = pl.d / dist;
    return res;
  }

  void plane::normalize_this()
  {
    plane t = normalize(*this);

    a = t.a;
    b = t.b;
    c = t.c;
    d = t.d;
  }

  vec3 plane::closest_point(const vec3 &point) const
  {
    return (point - normal() * distance(point));
  }

  float plane::distance(const vec3 &point) const
  {
    return (a * point.x + b * point.y + c * point.z + d);
  }

  vec3 plane::normal() const
  {
    return vec3(a, b, c);
  }

  float distance(const plane &pl, const vec3 &point)
  {
    return (pl.a * point.x + pl.b * point.y + pl.c * point.z + pl.d);
  }
}