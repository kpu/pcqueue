#include "pcqueue.h"

namespace util {

void Count() {
  PCQueue<int> queue(10);
  for (int i = 0; i < 10; ++i) {
    queue.Produce(i);
  }
  for (int i = 0; i < 10; ++i) {
    UTIL_ABORT_IF(i != queue.Consume(), Exception, "Mismatched PCQueue output for " << i);
  }
  for (int i = 0; i < 10; ++i) {
    queue.Produce(i);
  }
  for (int i = 0; i < 10; ++i) {
    UTIL_ABORT_IF(i != queue.Consume(), Exception, "Mismatched PCQueue output for " << i);
  }
}

} // namespace util

int main() {
  util::Count();
  std::cerr << "Success" << std::endl;
}
