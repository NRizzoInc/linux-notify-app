#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

/// TODO: Add windows implementations

namespace os {

enum class ThreadState
{
    WAITING_TO_START,
    INITIALIZING,
    RUNNING,
    TERMINATING,
    STOPPED,
};

/// \brief Adapter pattern style class to inheret from when
/// designing classes meant to be run as a thread
class Thread : public std::thread {
public:
    Thread();
    virtual ~Thread();

    /// \brief Starts the thread and loads any required context from initInThreadContext
    /// \note Needs to be triggered by caller/child class
    void spawn();

    /// \return ThreadState
    // Typically returns TERMINATING when object has reached its destructor,
    // but is waiting on readyToStopMainLoop() to return true.
    ThreadState getThreadState() const;

    /// \brief Informs caller if thread is still going
    /// @return True if thread is still active/running
    bool isThreadRunning() const;

    /// \brief It will trigger a lock/mutex to prevent race conditions while operating on the underlying resource
    /// \post Caller should call notifyResourceChange when done
    /// \todo Wrap this in another class that will auto-trigger cv notify_all on destructor call
    std::unique_lock<std::mutex> lockOnResource();

    /// \brief Alert the underlying resource manager that the resource has been released
    /// \note Should be called after lockOnResource
    void notifyResourceChange();

protected:
    /// \brief Funtion to overload to perform some operations before the thread starts
    virtual void initInThreadContext();

    /// \brief Funtion to overload to perform some operations when cleaning up the thread
    virtual void cleanupInThreadContext();

    /// \brief Funtion to overload to perform some operations that requires a resource lock
    virtual void runInLockedContext(std::function<void()> fn, std::chrono::nanoseconds timeout);

    /// \brief Should be overloaded and return true when thread main loop should terminate
    /// \returns True when thread should stop. False if thread should keep going
    virtual bool readyToStopMainLoop();

    /// \brief Function that should be overloaded by inhereting class
    /// It will be repeatedly called until program ends
    /// \note Inherently provides a locked/mutex context to access resources.
    //          Do not need to call lockOnResource()
    virtual void workerTask();

private:
    /// \brief changes the thread's state
    /// \param new_state The state to change to
    void changeThreadState(const ThreadState new_state);

    // loop where inhereted class's worker function is called
    void workerMainLoop();

    /// \brief Joins on the thread and waits for it be done
    void waitForThreadToFinish();

    /// \brief Waits until the thread reaches the designated state.
    /// Use carefully to make sure there are no deadlocks
    void waitForThreadState(const ThreadState state);

    std::atomic<ThreadState> m_thread_state;
    std::mutex m_resource_mutex;
    std::mutex m_state_mutex;
    std::condition_variable m_resource_cv;
    std::condition_variable m_state_cv;
    std::thread m_thread;

    Thread(const Thread&) = delete;
    Thread& operator=(const Thread&) = delete;
    Thread& operator=(const Thread&& arg) = delete;
};


} // namespace os
