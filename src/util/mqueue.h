#ifndef _UTIL_MQUEUE_H
#define _UTIL_MQUEUE_H

// Implementation taken from
// http://www.soa-world.de/echelon/uploaded_files/lockfreequeue.cpp

#include <string>

namespace mctop {

template <typename T>
class mqueue
{
 public:
  mqueue();
  ~mqueue();

  void produce(const T& t);
  bool consume(T& result);

 private:
  struct Node
  {
    Node(T val) : value(val), next(NULL) { }
    T value;
    Node* next;
  };
  Node* first;                                            // for producer only
  Node* divider;                                          // shared
  Node* last;                                             // shared
};

} // end namespace mctop

#endif
