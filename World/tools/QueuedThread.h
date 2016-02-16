#pragma once
#ifndef QueuedThread_h__
#define QueuedThread_h__


#include <functional>
#include <queue>
#include <mutex>
#include <memory>

class QueuedThread
{
public:
  using FunctorType = std::function<void()>;

  /// Потокобезопасный конструктор.
  QueuedThread();
  /// Потокобезопасный деструктор.
  ~QueuedThread();

  /// Отложенное выполнение функции в потоке. 
  /// Потокобезопасная операция.
  /// Все взаимодействие с потоком должно происходить 
  /// с помощью данного метода.
  template<class F, class ...Args>
  FunctorType PushFunc(F func, Args... args)
  {
    std::lock_guard<std::mutex> lock(mMutex);
    return [=] { return func(args...); };
  }

protected:
  /// Вызывается единожды при старте потока.
  virtual void Start() = 0;

  /// Постоянно вызывается в потоке.
  virtual void Process() = 0;

  /// Вызывается единожды при разрушении потока.
  virtual void Stop() = 0;


private:
  std::queue<FunctorType> mQueue;
  std::unique_ptr<std::thread> mThread;
  std::mutex mMutex;

  bool mClosed = false;
};



#endif // QueuedThread_h__

