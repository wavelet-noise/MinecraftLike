// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "Camera.h"


#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(void)
{
  mFov = 45.0f;
  mAspect = 1.0f;
  mNear = 0.01f;
  mFar = 2000.0f;
  mProjection = glm::perspective(mFov, mAspect, mNear, mFar);

  mQuat = glm::quat_cast(glm::lookAt
    (
      glm::vec3(0.0f, 0.0f, 0.0f), // eye
      glm::vec3(0.0f, 1.0f, 0.0f), // center
      glm::vec3(0.0f, 0.0f, 1.0f)  // up
      ));
}


Camera::~Camera(void)
{
}

const glm::mat4 &Camera::GetViewProject() const
{
  return mViewProjection;
}

const glm::mat4 &Camera::GetView() const
{
  return mView;
}

const glm::mat4 &Camera::GetProject() const
{
  return mProjection;
}

const glm::mat3 &Camera::GetDirection() const
{
  return mDirection;
}

void Camera::SetPos(const glm::vec3 &pos)
{
  changed = true;
  mPos = pos;
}

const glm::vec3 & Camera::GetPos() const
{
  return mPos;
}

void Camera::Resize(const glm::uvec2 &size)
{
  changed = true;
  mAspect = static_cast<float>(size.x) / static_cast<float>(size.y);
  mProjection = glm::perspective(mFov, mAspect, mNear, mFar);
}

void Camera::Rotate(const glm::vec3 &degrees)
{
  changed = true;
  mDir += degrees;
}

void Camera::SetRot(const glm::quat &quat)
{
  changed = true;
  mQuat = quat;
  mDir = {};
  Update();
}

void Camera::Move(const glm::vec3 &dist)
{
  changed = true;
  mPos += glm::vec3(dist.x, dist.z, -dist.y) * mQuat;
}

void Camera::Update()
{
  if (!changed)
    return;
  const auto &pitch = glm::angleAxis(mDir.x, glm::vec3(1, 0, 0));
  const auto &yaw = glm::angleAxis(mDir.z, glm::vec3(0, 0, 1));
  const auto &roll = glm::angleAxis(mDir.y, glm::vec3(0, 1, 0));
  mDir = {};

  mQuat = pitch * mQuat * yaw;
  mQuat = glm::normalize(mQuat);

  mView = glm::translate(glm::mat4_cast(mQuat), -mPos);
  mViewProjection = mProjection * mView;
  mDirection = glm::mat3_cast(mQuat);

  CalculateFrustum();
  changed = false;
}

glm::vec3 Camera::GetRay(const glm::vec2 &pos)
{
  glm::vec3 near = glm::unProject(glm::vec3(pos.x, 600.0f - pos.y, 0.0f), mView, mProjection, glm::vec4(0, 0, 600.0, 600));
  glm::vec3 far = glm::unProject(glm::vec3(pos.x, 600.0f - pos.y, 1.0f), mView, mProjection, glm::vec4(0, 0, 600.0, 600));
  return glm::normalize(far - near);
}

void NormalizePlane(float frustum[6][4], int side) {
  float magnitude = (float)sqrt((frustum[side][0] * frustum[side][0]) +
    (frustum[side][1] * frustum[side][1]) +
    (frustum[side][2] * frustum[side][2]));
  frustum[side][0] /= magnitude;
  frustum[side][1] /= magnitude;
  frustum[side][2] /= magnitude;
  frustum[side][3] /= magnitude;
}

void Camera::CalculateFrustum() {
  const glm::mat4 &MV = mView;
  const glm::mat4 &P = mProjection;

  m_clipMatrix[0] = (MV[0][0] * P[0][0]) + (MV[0][1] * P[1][0]) + (MV[0][2] * P[2][0]) + (MV[0][3] * P[3][0]);
  m_clipMatrix[1] = (MV[0][0] * P[0][1]) + (MV[0][1] * P[1][1]) + (MV[0][2] * P[2][1]) + (MV[0][3] * P[3][1]);
  m_clipMatrix[2] = (MV[0][0] * P[0][2]) + (MV[0][1] * P[1][2]) + (MV[0][2] * P[2][2]) + (MV[0][3] * P[3][2]);
  m_clipMatrix[3] = (MV[0][0] * P[0][3]) + (MV[0][1] * P[1][3]) + (MV[0][2] * P[2][3]) + (MV[0][3] * P[3][3]);

  m_clipMatrix[4] = (MV[1][0] * P[0][0]) + (MV[1][1] * P[1][0]) + (MV[1][2] * P[2][0]) + (MV[1][3] * P[3][0]);
  m_clipMatrix[5] = (MV[1][0] * P[0][1]) + (MV[1][1] * P[1][1]) + (MV[1][2] * P[2][1]) + (MV[1][3] * P[3][1]);
  m_clipMatrix[6] = (MV[1][0] * P[0][2]) + (MV[1][1] * P[1][2]) + (MV[1][2] * P[2][2]) + (MV[1][3] * P[3][2]);
  m_clipMatrix[7] = (MV[1][0] * P[0][3]) + (MV[1][1] * P[1][3]) + (MV[1][2] * P[2][3]) + (MV[1][3] * P[3][3]);

  m_clipMatrix[8] = (MV[2][0] * P[0][0]) + (MV[2][1] * P[1][0]) + (MV[2][2] * P[2][0]) + (MV[2][3] * P[3][0]);
  m_clipMatrix[9] = (MV[2][0] * P[0][1]) + (MV[2][1] * P[1][1]) + (MV[2][2] * P[2][1]) + (MV[2][3] * P[3][1]);
  m_clipMatrix[10] = (MV[2][0] * P[0][2]) + (MV[2][1] * P[1][2]) + (MV[2][2] * P[2][2]) + (MV[2][3] * P[3][2]);
  m_clipMatrix[11] = (MV[2][0] * P[0][3]) + (MV[2][1] * P[1][3]) + (MV[2][2] * P[2][3]) + (MV[2][3] * P[3][3]);

  m_clipMatrix[12] = (MV[3][0] * P[0][0]) + (MV[3][1] * P[1][0]) + (MV[3][2] * P[2][0]) + (MV[3][3] * P[3][0]);
  m_clipMatrix[13] = (MV[3][0] * P[0][1]) + (MV[3][1] * P[1][1]) + (MV[3][2] * P[2][1]) + (MV[3][3] * P[3][1]);
  m_clipMatrix[14] = (MV[3][0] * P[0][2]) + (MV[3][1] * P[1][2]) + (MV[3][2] * P[2][2]) + (MV[3][3] * P[3][2]);
  m_clipMatrix[15] = (MV[3][0] * P[0][3]) + (MV[3][1] * P[1][3]) + (MV[3][2] * P[2][3]) + (MV[3][3] * P[3][3]);

  m_frustum[0][0] = m_clipMatrix[3] - m_clipMatrix[0];
  m_frustum[0][1] = m_clipMatrix[7] - m_clipMatrix[4];
  m_frustum[0][2] = m_clipMatrix[11] - m_clipMatrix[8];
  m_frustum[0][3] = m_clipMatrix[15] - m_clipMatrix[12];
  NormalizePlane(m_frustum, 0);

  m_frustum[1][0] = m_clipMatrix[3] + m_clipMatrix[0];
  m_frustum[1][1] = m_clipMatrix[7] + m_clipMatrix[4];
  m_frustum[1][2] = m_clipMatrix[11] + m_clipMatrix[8];
  m_frustum[1][3] = m_clipMatrix[15] + m_clipMatrix[12];
  NormalizePlane(m_frustum, 1);

  m_frustum[2][0] = m_clipMatrix[3] + m_clipMatrix[1];
  m_frustum[2][1] = m_clipMatrix[7] + m_clipMatrix[5];
  m_frustum[2][2] = m_clipMatrix[11] + m_clipMatrix[9];
  m_frustum[2][3] = m_clipMatrix[15] + m_clipMatrix[13];
  NormalizePlane(m_frustum, 2);

  m_frustum[3][0] = m_clipMatrix[3] - m_clipMatrix[1];
  m_frustum[3][1] = m_clipMatrix[7] - m_clipMatrix[5];
  m_frustum[3][2] = m_clipMatrix[11] - m_clipMatrix[9];
  m_frustum[3][3] = m_clipMatrix[15] - m_clipMatrix[13];
  NormalizePlane(m_frustum, 3);

  m_frustum[4][0] = m_clipMatrix[3] - m_clipMatrix[2];
  m_frustum[4][1] = m_clipMatrix[7] - m_clipMatrix[6];
  m_frustum[4][2] = m_clipMatrix[11] - m_clipMatrix[10];
  m_frustum[4][3] = m_clipMatrix[15] - m_clipMatrix[14];
  NormalizePlane(m_frustum, 4);

  m_frustum[5][0] = m_clipMatrix[3] + m_clipMatrix[2];
  m_frustum[5][1] = m_clipMatrix[7] + m_clipMatrix[6];
  m_frustum[5][2] = m_clipMatrix[11] + m_clipMatrix[10];
  m_frustum[5][3] = m_clipMatrix[15] + m_clipMatrix[14];
  NormalizePlane(m_frustum, 5);
}

bool Camera::BoxWithinFrustum(const glm::vec4 &min, const glm::vec4 &max) const {
  for (int i = 0; i < 6; i++) {
    if ((m_frustum[i][0] * min.x + m_frustum[i][1] * min.y + m_frustum[i][2] * min.z + m_frustum[i][3] <= 0.0F) &&
      (m_frustum[i][0] * max.x + m_frustum[i][1] * min.y + m_frustum[i][2] * min.z + m_frustum[i][3] <= 0.0F) &&
      (m_frustum[i][0] * min.x + m_frustum[i][1] * max.y + m_frustum[i][2] * min.z + m_frustum[i][3] <= 0.0F) &&
      (m_frustum[i][0] * max.x + m_frustum[i][1] * max.y + m_frustum[i][2] * min.z + m_frustum[i][3] <= 0.0F) &&
      (m_frustum[i][0] * min.x + m_frustum[i][1] * min.y + m_frustum[i][2] * max.z + m_frustum[i][3] <= 0.0F) &&
      (m_frustum[i][0] * max.x + m_frustum[i][1] * min.y + m_frustum[i][2] * max.z + m_frustum[i][3] <= 0.0F) &&
      (m_frustum[i][0] * min.x + m_frustum[i][1] * max.y + m_frustum[i][2] * max.z + m_frustum[i][3] <= 0.0F) &&
      (m_frustum[i][0] * max.x + m_frustum[i][1] * max.y + m_frustum[i][2] * max.z + m_frustum[i][3] <= 0.0F))
    {
      return false;
    }
  }
  return true;
}

bool Camera::BoxWithinFrustum(const glm::vec3 &min, const glm::vec3 &max) const {
  for (int i = 0; i < 6; i++) {
    if ((m_frustum[i][0] * min.x + m_frustum[i][1] * min.y + m_frustum[i][2] * min.z + m_frustum[i][3] <= 0.0F) &&
      (m_frustum[i][0] * max.x + m_frustum[i][1] * min.y + m_frustum[i][2] * min.z + m_frustum[i][3] <= 0.0F) &&
      (m_frustum[i][0] * min.x + m_frustum[i][1] * max.y + m_frustum[i][2] * min.z + m_frustum[i][3] <= 0.0F) &&
      (m_frustum[i][0] * max.x + m_frustum[i][1] * max.y + m_frustum[i][2] * min.z + m_frustum[i][3] <= 0.0F) &&
      (m_frustum[i][0] * min.x + m_frustum[i][1] * min.y + m_frustum[i][2] * max.z + m_frustum[i][3] <= 0.0F) &&
      (m_frustum[i][0] * max.x + m_frustum[i][1] * min.y + m_frustum[i][2] * max.z + m_frustum[i][3] <= 0.0F) &&
      (m_frustum[i][0] * min.x + m_frustum[i][1] * max.y + m_frustum[i][2] * max.z + m_frustum[i][3] <= 0.0F) &&
      (m_frustum[i][0] * max.x + m_frustum[i][1] * max.y + m_frustum[i][2] * max.z + m_frustum[i][3] <= 0.0F))
    {
      return false;
    }
  }
  return true;
}