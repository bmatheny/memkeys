#include <iostream>
#include <iomanip>
#include <string>
#include <pcrecpp.h>

#include "net/net.h"

extern "C" {
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/if_ether.h>
}

namespace mctop {

using namespace std;

// constructor
MemcacheCommand::MemcacheCommand(const Packet& _packet,
                                 const bpf_u_int32 captureAddress)
    : cmd_type(MC_UNKNOWN), sourceAddress(),
      commandName(), objectKey(), objectSize(0)
{
  static ssize_t ether_header_sz = sizeof(struct ether_header);
  static ssize_t ip_sz = sizeof(struct ip);
  static ssize_t tcphdr_sz = sizeof(struct tcphdr);

  const struct ether_header* ethernetHeader;
  const struct ip* ipHeader;
  const struct tcphdr* tcpHeader;

  const Packet::Header* pkthdr = &_packet.getHeader();
  const Packet::Data* packet = _packet.getData();

  bool possible_request = false;
  u_char *data;
  uint32_t dataLength = 0;

  // must be an IP packet
  ethernetHeader = (struct ether_header*)packet;
  if (ntohs(ethernetHeader->ether_type) != ETHERTYPE_IP) {
    return;
  }

  // must be TCP - TODO add support for UDP
  ipHeader = (struct ip*)(packet + ether_header_sz);
  if (ipHeader->ip_p != IPPROTO_TCP) {
    return;
  }
  setSourceAddress(&(ipHeader->ip_src));

  // The packet was destined for our capture address, this is a request
  // This bit of optimization lets us ignore a reasonably large percentage of
  // traffic
  if (ipHeader->ip_dst.s_addr == captureAddress) {
    possible_request = true;
  }

  tcpHeader = (struct tcphdr*)(packet + ether_header_sz + ip_sz);
  data = (u_char*)(packet + ether_header_sz + ip_sz + tcphdr_sz);
  dataLength = pkthdr->len - (ether_header_sz + ip_sz + tcphdr_sz);
  if (dataLength > pkthdr->caplen) {
    dataLength = pkthdr->caplen;
  }

  if (possible_request && parseRequest(data, dataLength)) {
    cmd_type = MC_REQUEST;
  } else if (!possible_request && parseResponse(data, dataLength)) {
    cmd_type = MC_RESPONSE;
  }
}

// protected
bool MemcacheCommand::parseRequest(u_char *data, int length)
{
  // don't care about requests right now
  return false;
}

bool MemcacheCommand::parseResponse(u_char *data, int length)
{
  static pcrecpp::RE re("VALUE (\\S+) \\d+ (\\d+)",
                        pcrecpp::RE_Options(PCRE_MULTILINE));
  bool found_response = false;
  string key;
  int size = -1;
  string input = "";
  for (int i = 0; i < length; i++) {
    int cid = (int)data[i];
    if (isprint(cid) || cid == 10 || cid == 13) {
      input += (char)data[i];
    }
  }
  re.PartialMatch(input, &key, &size);
  if (size >= 0) {
    objectSize = size;
    objectKey = key;
    found_response = true;
  }
  return found_response;
}

void MemcacheCommand::setSourceAddress(const void * src)
{
  char sourceIp[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, src, sourceIp, INET_ADDRSTRLEN);
  sourceAddress = sourceIp;
}
void MemcacheCommand::setCommandName(const std::string &name)
{
  commandName = name;
}

} // end namespace mctop
