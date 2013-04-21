#include "util/stat.h"

namespace mctop {

using namespace std;

Stat::Stat(const string &key, const uint32_t size)
    : key(key),
      _created(Stat::now()),
      _size(size),
      _count(1)
{}
Stat::~Stat() {}
// Copy constructor needed for StatCollection
Stat::Stat(const Stat &stat)
  : key(stat.getKey()),
    _created(stat.getCreated()),
    _size(stat.getSize()),
    _count(stat.getCount())
{}

uint64_t Stat::getCreated() const {
  return _created;
}
const string Stat::getKey() const {
  return key;
}
uint32_t Stat::getSize() const {
  return _size.load();
}
void Stat::setSize(const uint32_t size) {
  _size.store(size);
}

double Stat::bandwidth(const uint64_t elapsed_t) const {
  return (getSize() * requestRate(elapsed_t) * 8) / 1000.0;
}

double Stat::requestRate(const uint64_t elapsed_t) const {
  return getCount() / (double)elapsed_t;
}

void Stat::increment() {
  _count++;
}

} // end namespace mctop
