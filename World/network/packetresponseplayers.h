#ifndef PACKETRESPONSEPLAYERS_H
#define PACKETRESPONSEPLAYERS_H
#include "packet.h"
#include "glm/glm.hpp"
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/base_object.hpp>
//#include "core/playeremplacer.h"

class PacketResponsePlayers : public NumberedPacket<PacketResponsePlayers>
{
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & boost::serialization::base_object<Packet>(*this);
        ar & emp;
    }

public:
    PacketResponsePlayers();

    //std::vector<PlayerEmplacer> emp;
};

#endif // PACKETRESPONSEPLAYERS_H
