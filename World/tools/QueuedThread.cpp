#include "QueuedThread.h"



QueuedThread::QueuedThread()
{
  mThread = std::make_unique<decltype(mThread)::element_type>([this]
  {
    Start();
    while (true)
    {
      Process();

      std::lock_guard<std::mutex> lock(mMutex);
      if (mClosed)
      {
        break;
      }
      else
      {
        while (!mQueue.empty())
        {
          mQueue.front()();
          mQueue.pop();
        }
      }
    }
    Stop();
  });
}


QueuedThread::~QueuedThread()
{
  {
    std::lock_guard<std::mutex> lock(mMutex);
    mClosed = true;
  }
  mThread->join();
}
