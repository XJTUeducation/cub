#include <cub/sched/Executor.h>
#include <cub/algo/loop.h>

CUB_NS_BEGIN

Executor::Executor(size_t threadNum) : stop(false) {
  FOREACH(i, threadNum) {
    workers.emplace_back([this] {
      threadRun();
    });
  }
}

Executor::~Executor() {
  SYNCHRONIZED(tasksMutex){
    stop = true;
  }
  condition.notify_all();

  for(std::thread &worker : workers) {
    worker.join();
  }
}

void Executor::threadRun() {
  FOREVER {
    std::function<void()> task;

    SYNCHRONIZED(tasksMutex) {
      condition.wait(LOCKER(tasksMutex).getLocker(), [this] {
        return stop || !tasks.empty();
      });

      if(stop && tasks.empty())
        return;
      task = std::move(tasks.front());
      tasks.pop();
    }

    task();
  }
}

CUB_NS_END
