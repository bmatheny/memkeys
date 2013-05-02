#ifndef _NET_PACKET_H
#define _NET_PACKET_H

#include "net/pcap.h"

extern "C" {
#include <string.h>
#include <stdlib.h>
}

namespace mckeys {

class Packet {
 public:

  typedef u_char Data;
  typedef pcap_pkthdr Header;

  Packet();
  Packet(const Header& header, const Data* data);
  Packet(const Packet& packet); // copy constructor
  Packet& operator=(const Packet& packet); // assignment operator
  virtual ~Packet();

  const Data* getData() const { return data; }
  const Header& getHeader() const { return header; }

  uint32_t capLength() const { return getHeader().caplen; }
  int32_t id() const;
  uint32_t length() const { return getHeader().len; }
  uint64_t timestamp() const { return _timestamp; }

 protected:
  Data* copyData(Data* data, const bpf_u_int32 len);

 private:
  inline void* __void_data(Data* d);
  void copy(const Packet& other);
  void destroy();

  Header header;
  Data* data;
  uint64_t _timestamp;

};

} // end namespace

#endif
