#pragma once
#include "network.h"

#include "packetrequestplayers.h"
#include "packetrequestsector.h"

#include "packetresponseplayers.h"
#include "packetresponsesector.h"

namespace {
  class NetworkManager : public Connection
  {
  private:
    void OnAccept(const std::string & host, uint16_t port)
    {
      LOG(info) << "acception " << host << ":" << port;

      Recv();
    }

    void OnConnect(const std::string & host, uint16_t port)
    {
      LOG(info) << "connection " << host << ":" << port;

      Recv();
    }

    void OnSend(const std::vector< uint8_t > & buffer)
    {
    }

    void OnRecv(std::vector< uint8_t > & buffer)
    {
      Recv();

      PacketHolder ph;

      if (buffer.size() <= 6) //not packet
        return;

      try {
        ph.Deserialize(buffer);
      }
      catch (...)
      {
        return;
      }

      if (ph.packet->id == 2)
      {
        PacketRequestSector &prs = *std::static_pointer_cast<PacketRequestSector>(ph.packet);

        std::shared_ptr<Sector> s = RemoteServer::instance().GetSector(prs.pos);
        if (!s)
          return;

        PacketHolder ph;
        ph.Init<PacketResponseSector>(s);

        Send(ph);
      }

      if (ph.packet->id == 1)
      {
        PacketRequestPlayers &prp = *std::static_pointer_cast<PacketRequestPlayers>(ph.packet);
        pemp[prp.id] = PlayerEmplacer(prp.our_pos, prp.our_phi, prp.id);

        PacketHolder ph;
        std::vector<PlayerEmplacer> tempp;
        for (const auto &p : pemp)
        {
          tempp.push_back(p.second);
        }

        ph.Init<PacketResponsePlayers>(tempp);
        Send(ph);
      }
    }

    void OnTimer(const boost::posix_time::time_duration & delta)
    {
      //LOG(info) << "[" << __FUNCTION__ << "] " << delta;
    }

    void OnError(const boost::system::error_code & error)
    {
      LOG(info) << "error code " << error << " : " << error.message();
    }

    std::map<size_t, std::function<void(std::shared_ptr<Packet>)>> attaches;

  public:
    NetworkManager(std::shared_ptr< Hive > hive)
      : Connection(hive)
    {
    }

    template<typename _Ty>
    void Attach(std::function<void(std::shared_ptr<Packet>)> callback)
    {
      size_t cid = (reinterpret_cast<_Ty*>(nullptr))->GetId();
      if (attaches.find(cid) != attaches.end())
        LOG(trace) << typeid(_Ty).name << " callback registering";
      else
        LOG(trace) << typeid(_Ty).name << " callback override";
      attaches[cid] = callback;
    }

    ~NetworkManager()
    {
    }
  };
}