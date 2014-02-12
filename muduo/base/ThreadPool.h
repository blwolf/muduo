// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_THREADPOOL_H
#define MUDUO_BASE_THREADPOOL_H

#include <muduo/base/Condition.h>
#include <muduo/base/Mutex.h>
#include <muduo/base/Thread.h>
#include <muduo/base/Types.h>

#include <boost/noncopyable.hpp>

#include <deque>
#include <vector>

namespace muduo
{

class ThreadPool : boost::noncopyable
{
 public:
  typedef std::function<void ()> Task;

  explicit ThreadPool(const string& name = string("ThreadPool"));
  ~ThreadPool();

  // Must be called before start().
  void setMaxQueueSize(int maxSize) { maxQueueSize_ = maxSize; }

  void start(int numThreads);
  void stop();

  // Could block if maxQueueSize > 0
  void run(const Task& f);
#ifdef __GXX_EXPERIMENTAL_CXX0X__
  void run(Task&& f);
#endif

 private:
  bool isFull() const;
  void runInThread();
  Task take();

  MutexLock mutex_;
  Condition notEmpty_;
  Condition notFull_;
  string name_;
  std::vector<std::unique_ptr<muduo::Thread>> threads_;
  std::deque<Task> queue_;
  size_t maxQueueSize_;
  bool running_;
};

}

#endif
