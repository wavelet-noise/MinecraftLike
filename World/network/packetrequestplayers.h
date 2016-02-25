#ifndef PACKETREQUESTPLAYERS_H
#define PACKETREQUESTPLAYERS_H
#include "packet.h"
#include "glm/glm.hpp"
#include <boost/serialization/serialization.hpp>
#include <Core\TemplateFactory.h>

class PacketRequestPlayers : public NumberedPacket<PacketRequestPlayers>
{
  friend class boost::serialization::access;
  virtual void serialize(boost::archive::text_iarchive & ar, const unsigned int)
  {
    ar & our_pos;
    ar & our_phi;
  }

public:
  glm::vec3 our_pos;
  float our_phi;
};

REGISTER_PACKET_(PacketRequestPlayers)

#endif // PACKETREQUESTPLAYERS_H
