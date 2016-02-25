#include "NetworkManager.h"
#include "packetrequestplayers.h"

void NetworkManager::OnAccept(const std::string & host, uint16_t port)
{
  LOG(info) << "acception " << host << ":" << port;

  Recv();
}

void NetworkManager::OnConnect(const std::string & host, uint16_t port)
{
  LOG(info) << "connection " << host << ":" << port;

  Recv();
}

void NetworkManager::OnSend(const std::vector<uint8_t>& buffer)
{
}

size_t getLength(const std::vector<uint8_t> &buffer)
{
  size_t l = (buffer[3] << 24) | (buffer[2] << 16) | (buffer[1] << 8) | buffer[0] + 6;

  return l;
}

size_t getType(const std::vector<uint8_t> &buffer)
{
  size_t t = (buffer[5] << 8) | buffer[4];

  return t;
}

void NetworkManager::OnRecv(std::vector<uint8_t>& buffer)
{
  static std::vector<uint8_t> ubf;
  static std::vector<uint8_t> tail;
  static size_t length_header = 0;
  try
  {
    if (ubf.empty())
    {
      ubf.insert(ubf.end(), buffer.begin(), buffer.end());
      length_header = getLength(ubf);

      if (length_header == 0)
        LOG(info) << "hz chto, stream skipped";
    }
    else
    {
      ubf.insert(ubf.end(), buffer.begin(), buffer.end());
    }

    if (ubf.size() >= length_header) //enough accepted
    {
      if (ubf.size() > length_header) //more than one accepted
      {
        tail.insert(tail.end(), ubf.begin() + length_header, ubf.end()); //move tail to next
        ubf.erase(ubf.begin() + length_header, ubf.end());
      }

      if (ubf.size() == length_header && length_header != 0) //exact needed packet
      {
        size_t type = getType(ubf);
        auto packet = PacketFactory_::Get().Create(type);

        boost::interprocess::bufferstream bs(reinterpret_cast<char*>(&ubf[0]), ubf.size());
        bs.ignore(6);

        boost::archive::text_iarchive ia(bs);

        ia >> *packet;

        const auto &a = attaches.find(packet->GetId());
        if (a != attaches.end())
        {
          a->second(packet);
        }
        else
        {
          // ?
        }

        ubf.clear();

        if (!tail.empty()) //push tail to next
        {
          ubf.insert(ubf.end(), tail.begin(), tail.end());
          length_header = getLength(ubf);
          if (length_header == 0)
            LOG(info) << "hz chto, stream skipped";
          tail.clear();
        }
      }
      else
      {
        tail.clear();
        ubf.clear();
        LOG(error) << "broken stream =(";
      }
    }
  }
  catch (...)
  {
    ubf.clear();
  }

  Recv();
}

inline void NetworkManager::OnError(const boost::system::error_code & error)
{
  LOG(info) << "error code " << error << " : " << error.message();
}

NetworkManager::NetworkManager(std::shared_ptr<Hive> hive)
  : Connection(hive)
{
}

NetworkManager::~NetworkManager()
{
}
