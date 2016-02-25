#ifndef PACKET_H
#define PACKET_H
#include <vector>
#include <inttypes.h>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/export.hpp>
#include <memory>
#include <glm/glm.hpp>
#include <boost/interprocess/streams/bufferstream.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost\noncopyable.hpp>

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

namespace {
  size_t Nextid()
  {
    static size_t next_id(0);
    return next_id++;
  }
}

class Packet
{
public:
  virtual ~Packet() {}

  template <typename T_>
  inline static size_t Idfor()
  {
    static size_t result(Nextid());
    return result;
  }

  friend class boost::serialization::access;

  virtual size_t GetId() {
    return -1;
  }

  virtual void serialize(boost::archive::text_iarchive & ar, const unsigned int)
  {
  }
};

template<typename T> struct NumberedPacket : Packet {
  virtual size_t GetId() {
    return Idfor<T>();
  }
};

#endif // PACKET_H
