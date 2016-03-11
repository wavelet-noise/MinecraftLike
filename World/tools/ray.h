#pragma once
#include <glm\glm.hpp>
namespace glm {
/*!
* \brief Описывает луч
*/
struct ray {

  ray() {}
  ray(glm::vec3 p, glm::vec3 d);
  ray(const glm::ray &r);
  ray &operator =(const glm::ray &r) {
    if (this != &r) {
      mOrigin = r.mOrigin; mDir = r.mDir; mDir = normalize(mDir);
      inv = glm::vec3(1 / mDir.x, 1 / mDir.y, 1 / mDir.z);
      sign[0] = inv.x < 0;
      sign[1] = inv.y < 0;
      sign[2] = inv.z < 0;
    }  return *this;
  }
  ~ray();

  glm::vec3 origin();
  glm::vec3 dir();

private:
  /*!
  * \brief Нормализует направление и предрасчитывает обратное направление и массив sign[]
  */
  void precompute();

  friend glm::ray normalize(const glm::ray &x);
  friend bool intersect(const glm::ray &r, float t0, float t1, const glm::vec3 &min, const glm::vec3 &max);
  glm::vec3 mOrigin; /*< Точка начала луча*/
  glm::vec3 mDir; /*< Направление луча*/
  glm::vec3 inv; /*< Инвертированное направление луча*/
  int sign[3]; /*< dir[i] < 0 (используется для оптипизации)*/
};

/*!
* \brief Приведение длины вектора направления к единице
* \param x Луч
* \return Нормализованный луч
*/
glm::ray normalize(const glm::ray &x);

/*!
* \brief Проверяет пересечение луча с ограничивающим паралелемипедом в определенном участе луча
*
* \param r Луч
* \param t0 Ближняя граница отсечения по лучу
* \param t1 Дальняя граница отсечения по лучу
* \param min Минимальная точка ограничивающего паралелепипеда
* \param max Максимальная точка ограничивающего паралелепипеда
* \return true, в случае пересечение и false в обратном случае
*
* @note t1 > t0, max[i] > min[i]
*/
bool intersect(const glm::ray &r, float t0, float t1, const glm::vec3 &min, const glm::vec3 &max);

struct plane {
public:
  plane();
  plane(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
  plane(const glm::vec3& normal, const glm::vec3& point);
  plane(const glm::vec3& normal, const float constant);

  plane(float _a, float _b, float _c, float _d);

  static plane normalize(const plane &pl);

  float     distance(const glm::vec3 &point) const;
  glm::vec3 closest_point(const glm::vec3 &point) const;
  glm::vec3 normal() const;

  float a, b, c, d;
  void normalize_this();
};

float distance(const plane &pl, const glm::vec3 &point);
}