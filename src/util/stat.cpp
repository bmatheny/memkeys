#include "util/stat.h"

#include <cmath>

namespace mckeys {

using namespace std;
using std::isinf;

Stat::Stat()
    : key(), _created(0), _size(0), _count(0)
{}
Stat::Stat(const string &key, const uint32_t size)
    : key(key),
      _created(UtilTime::currentTimeMillis()),
      _size(size),
      _count(1)
{}
Stat::~Stat() {}
// std::atomic member variables require that your class implement a copy
// constructor and the assignment operator, if you want to use the Stat class
// with most STL collections
Stat::Stat(const Stat &stat)
  : key(stat.getKey()),
    _created(stat.getCreated()),
    _size(stat.getSize()),
    _count(stat.getCount())
{}
// TODO implement copy/destroy
Stat& Stat::operator=(const Stat& rhs)
{
  if (this != &rhs) {
    key = rhs.getKey();
    _created = rhs.getCreated();
    setSize(rhs.getSize());
    _count.store(rhs.getCount());
  }
  return *this;
}

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
  double rate = (getCount() / (double)elapsed_t);
  if (isinf(rate)) {
    return 1.0;
  } else {
    return rate;
  }
}

void Stat::increment() {
  _count += 1;
}

} // end namespace
