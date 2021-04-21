#include "pcqueue.h"

#include <thread>
#include <vector>

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

void SingleInSingleOut() {
  PCQueue<int> queue(15);
  std::thread writer([&queue]() {
      for (int i = 0; i < 100; ++i) {
        queue.Produce(i);
      }
  });
  for (int i = 0; i < 100; ++i) {
    UTIL_ABORT_IF(i != queue.Consume(), Exception, "Mismatched PCQueue output for " << i);
  }
  writer.join();
}

void MultipleWriters() {
  const unsigned kCount = 2000;
  const unsigned kNumThreads = 4;
  PCQueue<unsigned> queue(13);
  auto writer = [&queue, kCount]() {
    for (unsigned i = 0; i < kCount; ++i) {
      queue.Produce(i);
    }
  };
  std::vector<std::thread> threads;
  for (unsigned i = 0; i < kNumThreads; ++i) {
    threads.emplace_back(writer);
  }
  unsigned seen[kCount] = {0};
  for (unsigned i = 0; i < kCount * kNumThreads; ++i) {
    unsigned got = queue.Consume();
    UTIL_ABORT_IF(got >= kCount, Exception, "Index " << got << " too high.");
    seen[got]++;
    // Since each thread generates in order, counts should be monotonically non-increasing.
    UTIL_ABORT_IF(got && seen[got] > seen[got - 1], Exception, "Queue is not keeping order.");
  }
  for (unsigned i = 0; i < kCount; ++i) {
    UTIL_ABORT_IF(seen[i] != kNumThreads, Exception, "Expected to see values from exactly " << kNumThreads << " threads but got " << seen[i] << " of " << i);
  }
  for (std::thread &t : threads) {
    t.join();
  }
}

} // namespace util

int main() {
  util::Count();
  util::SingleInSingleOut();
  util::MultipleWriters();
  std::cout << "Success" << std::endl;
  return 0;
}
