// Implementation taken from
// http://www.soa-world.de/echelon/uploaded_files/lockfreequeue.cpp
// http://www.soa-world.de/echelon/2011/02/a-simple-lock-free-queue-in-c.html
//
// Provides a lock free queue for single producer, single consumer applications
#include "util/mqueue.h"

namespace mctop {

template <typename T>
mqueue<T>::mqueue()
{
  first = new Node(T());
  divider = first;
  last = first;
}

template <typename T>
mqueue<T>::~mqueue()
{
  while(first != NULL)                                      // release the list
  {
    Node* tmp = first;
    first = tmp->next;
    delete tmp;
  }
}

template <typename T>
void mqueue<T>::produce(const T& t)
{
  last->next = new Node(t);                               // add the new item
  asm volatile("" ::: "memory");                          // prevend compiler reordering
  // gcc atomic, cast to void to prevent "value computed is not used"
  (void)__sync_lock_test_and_set(&last, last->next);
  while(first != divider)                                 // trim unused nodes
  {
    Node* tmp = first;
    first = first->next;
    delete tmp;
  }
}

template <typename T>
bool mqueue<T>::consume(T& result)
{
  if(divider != last)                                     // if queue is nonempty
  {
    result = divider->next->value;                        // C: copy it back
    asm volatile("" ::: "memory");                        // prevent compiler reordering
    // gcc atomic, cast to void to prevent "value computed is not used"
    (void)__sync_lock_test_and_set(&divider, divider->next);
    return true;                                          // and report success
  }
  return false;                                           // else report empty
}

} // end namespace mctop
