#include <iostream>
#include <iomanip>
#include <string>

#include "net/memcache_command.h"

extern "C" {
#include <pcap.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/if_ether.h>
}

namespace mctop {

using namespace std;

// constructor
MemcacheCommand::MemcacheCommand(const struct pcap_pkthdr* pkthdr, const u_char* packet)
    : cmd_type(MC_UNKNOWN),
      payload(""),
      sourceAddress("")
{
  const struct ether_header* ethernetHeader;
  static ssize_t ether_header_sz = sizeof(struct ether_header);
  const struct ip* ipHeader;
  static ssize_t ip_sz = sizeof(struct ip);
  const struct tcphdr* tcpHeader;
  static ssize_t tcphdr_sz = sizeof(struct tcphdr);
  u_char *data;
  int dataLength = 0;
  string dataStr = "";

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
  cout << ipHeader->ip_src.s_addr << endl;
  setSourceAddress(&(ipHeader->ip_src));

  tcpHeader = (struct tcphdr*)(packet + ether_header_sz + ip_sz);
  data = (u_char*)(packet + ether_header_sz + ip_sz + tcphdr_sz);
  dataLength = pkthdr->len - (ether_header_sz + ip_sz + tcphdr_sz);

  cout << getSourceAddress() << endl;
  for (int i = 0; i < dataLength; i++) {
    uint32_t cv = data[i];
    //cout << setw(15) << i << setw(5) << cv << " ";
    if ((cv >= 32 && cv <= 126) || cv == 10 || cv == 11 || cv == 13) {
      /*
      if (cv == 10) {
        cout << "\\n";
      } else if (cv == 11) {
        cout << "\\v";
      } else if (cv == 13) {
        cout << "\\r";
      } else {
        cout << (char)cv;
      } */
      dataStr += (char)cv;
    } else {
      //cout << "<>";
      dataStr += ".";
    }
    //cout << endl;
  }
}

// protected
void MemcacheCommand::setSourceAddress(const void * src)
{
  char sourceIp[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, src, sourceIp, INET_ADDRSTRLEN);
  sourceAddress = sourceIp;
}

} // end namespace mctop
