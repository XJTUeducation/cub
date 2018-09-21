#ifndef HA2478A83_2111_4DE2_824C_BE3A4943ED52
#define HA2478A83_2111_4DE2_824C_BE3A4943ED52

#include <cub/cub.h>

#include <atomic>
#include <mutex>
#include <condition_variable>

CUB_NS_BEGIN

struct BlockingCounter {
  BlockingCounter(int initial_count)
    : state_(initial_count << 1), notified_(false) {
  }

  void DecrementCount() {
    unsigned int v = state_.fetch_sub(2, std::memory_order_acq_rel) - 2;
    if (v != 1) {
      return;  // either count has not dropped to 0, or waiter is not waiting
    }
    std::mutex_lock l(mu_);
    notified_ = true;
    cond_var_.notify_all();
  }

  inline void Wait() {
    unsigned int v = state_.fetch_or(1, std::memory_order_acq_rel);
    if ((v >> 1) == 0)
      return;
    mutex_lock l(mu_);
    while (!notified_) {
      cond_var_.wait(l);
    }
  }
  // Wait for the specified time, return false iff the count has not dropped to
  // zero before the timeout expired.
  inline bool WaitFor(std::chrono::milliseconds ms) {
    unsigned int v = state_.fetch_or(1, std::memory_order_acq_rel);
    if ((v >> 1) == 0)
      return true;
    mutex_lock l(mu_);
    while (!notified_) {
      const std::cv_status status = cond_var_.wait_for(l, ms);
      if (status == std::cv_status::timeout) {
        return false;
      }
    }
    return true;
  }

private:
  std::mutex mu_;
  std::condition_variable cond_var_;
  std::atomic<int> state_;  // low bit is waiter flag
  bool notified_;
};

CUB_NS_END

#endif
