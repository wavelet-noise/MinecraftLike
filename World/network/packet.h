#ifndef PACKET_H
#define PACKET_H
#include <vector>
#include <inttypes.h>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/export.hpp>
#include <memory>
#include <glm/glm.hpp>

namespace boost
{
  namespace serialization
  {
    template<class Archive>
    void serialize(Archive & ar, glm::ivec2& vec, const unsigned int)
    {
      ar & boost::serialization::make_nvp("x", vec.x);
      ar & boost::serialization::make_nvp("y", vec.y);
    }

    template<class Archive>
    void serialize(Archive & ar, glm::ivec3& vec, const unsigned int)
    {
      ar & boost::serialization::make_nvp("x", vec.x);
      ar & boost::serialization::make_nvp("y", vec.y);
      ar & boost::serialization::make_nvp("z", vec.z);
    }

    template<class Archive>
    void serialize(Archive & ar, glm::vec3& vec, const unsigned int)
    {
      ar & boost::serialization::make_nvp("x", vec.x);
      ar & boost::serialization::make_nvp("y", vec.y);
      ar & boost::serialization::make_nvp("z", vec.z);
    }

    template<class Archive>
    void serialize(Archive & ar, glm::vec2& vec, const unsigned int)
    {
      ar & boost::serialization::make_nvp("x", vec.x);
      ar & boost::serialization::make_nvp("y", vec.y);
    }
  }
}

class Packet : boost::noncopyable
{
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive &ar, const unsigned int)
  {
  }

public:
  virtual ~Packet() {}
  Packet(size_t __id = 0)
  {

  }

  virtual size_t GetId() = 0;
};

namespace
{
  static size_t Nextid()
  {
    static size_t next_id(0);
    return next_id++;
  }

  template <typename T_>
  static size_t Idfor()
  {
    static size_t result(Nextid());
    return result;
  }
}

template<typename T> struct NumberedPacket : Packet {
  int getId() {
    return Idfor<T>();
  }
};

#endif // PACKET_H
