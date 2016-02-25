#pragma once
#include "network.h"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/interprocess/streams/bufferstream.hpp>
#include <boost\noncopyable.hpp>
#include <Core\TemplateFactory.h>

class Packet;

#define REGISTER_PACKET_(type) REGISTER_ELEMENT(type, PacketFactory_::Get(), Packet::Idfor<type>())

struct PacketFactory_
{
  using FactoryType_ = TemplateFactory<int, Packet>;
  static FactoryType_ &Get()
  {
    static FactoryType_ object;
    return object;
  }
};

class NetworkManager : public Connection
{
private:
  void OnAccept(const std::string & host, uint16_t port);

  void OnConnect(const std::string & host, uint16_t port);

  void OnSend(const std::vector< uint8_t > & buffer);

  void OnRecv(std::vector<uint8_t> &buffer);

  void OnTimer(const boost::posix_time::time_duration & delta)
  {
    //LOG(info) << "[" << __FUNCTION__ << "] " << delta;
  }

  void OnError(const boost::system::error_code & error);

  std::map<size_t, std::function<void(std::shared_ptr<Packet>)>> attaches;

public:
  NetworkManager(std::shared_ptr< Hive > hive);

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

  ~NetworkManager();
};