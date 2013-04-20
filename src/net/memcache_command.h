#ifndef _NET_MEMCACHE_CMD_H
#define _NET_MEMCACHE_CMD_H

#include <string>

extern "C" {
#include <pcap.h>
}

namespace mctop {

enum memcache_command_t {
  MC_UNKNOWN, MC_REQUEST, MC_RESPONSE
};

class MemcacheCommand
{
 public:
  MemcacheCommand(const struct pcap_pkthdr* pkthdr, const u_char* packet,
                  const bpf_u_int32 captureAddress);

  bool isCommand() const
    { return (isRequest() || isResponse()); }
  bool isRequest() const
    { return (cmd_type == MC_REQUEST); }
  bool isResponse() const
    { return (cmd_type == MC_RESPONSE); }

  std::string getCommandName() const
    { return commandName; }
  std::string getObjectKey() const
    { return objectKey; }
  uint32_t getObjectSize() const
  { return objectSize; }
  std::string getSourceAddress() const
    { return sourceAddress; }

 protected:
  void setSourceAddress(const void * src);
  void setCommandName(const std::string &name);

  bool parseRequest(u_char *data, int dataLength);
  bool parseResponse(u_char *data, int dataLength);

  memcache_command_t cmd_type;
  std::string payload;
  std::string sourceAddress;
  std::string commandName;
  std::string objectKey;
  int objectSize;

};

} // end namespace mctop

#endif
