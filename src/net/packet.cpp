#include "net/packet.h"

namespace mckeys {
using namespace std;

Packet::Packet() : header(), data(NULL), _timestamp(0)
{
  memset(&header, 0, sizeof(header));
}
Packet::Packet(const Header& header, const Data* data)
  : header(header),
    data(copyData(const_cast<Data*>(data), header.caplen)),
    _timestamp(UtilTime::currentTimeMillis())
{}

// Copy constructor
Packet::Packet(const Packet& other)
{
  copy(other);
}
// Assignment operator
Packet& Packet::operator=(const Packet& packet) {
  if (this != &packet) {
    destroy();
    copy(packet);
  }
  return (*this);
}
// Destructor
Packet::~Packet() {
  destroy();
}

int32_t Packet::id() const {
  return header.ts.tv_sec + header.ts.tv_usec + length() + capLength();
}

///////////////////////////////////////////////////////////////////////////////
//                      Protected Methods                                    //
///////////////////////////////////////////////////////////////////////////////

/**
 * This horrible bit of hack has something to do with memory alignment. Using
 * memcpy without all the cast business results in a segfault due to
 * uninitialized memory.
 * @warning This is likely not portable
 * @protected
 */
Packet::Data* Packet::copyData(Data* in, const bpf_u_int32 len) {
  if (len == 0) {
    return NULL;
  }
  void * d = reinterpret_cast<void*>(in);
  void * nd = malloc(len);
  memset(nd, 0, len);
  memcpy(nd, const_cast<void const*>(d), len);
  return reinterpret_cast<Data*>(nd);
}

///////////////////////////////////////////////////////////////////////////////
//                      Private Methods                                      //
///////////////////////////////////////////////////////////////////////////////

/**
 * Cast data to a void.
 * @private
 */
inline void* Packet::__void_data(Data* d) {
  return reinterpret_cast<void*>(d);
}

/**
 * Copy the contents of one packet to this packet, respecting the malloc()
 * required for a proper copy.
 * @param Packet other the packet to copy data from
 * @result Nothing, mutates the current packet
 * @private
 */
void Packet::copy(const Packet& other) {
  header = other.header;
  if (other.data == NULL) {
    data = NULL;
  } else {
    data = copyData(other.data, other.capLength());
  }
  _timestamp = other._timestamp;
}

/**
 * Destroy the current packet. Free the memory associated with data if needed.
 * @private
 */
void Packet::destroy() {
  if (data != NULL) {
    free(__void_data(data));
  }
}

} // end namespace
