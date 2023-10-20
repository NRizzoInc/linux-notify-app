#include "Thread.h"

#include <iostream>

namespace os {

Thread::Thread() : m_thread_state(ThreadState::WAITING_TO_START)
{

}

Thread::~Thread()
{
    waitForThreadToFinish();
}

void Thread::waitForThreadToFinish()
{
    const ThreadState state = getThreadState();
    if (state == ThreadState::INITIALIZING || state == ThreadState::RUNNING)
    {
        std::cout << "TERMINATING\n";
        changeThreadState(ThreadState::TERMINATING);
        m_state_cv.notify_all();
        if (m_thread.joinable())
        {
            m_thread.join();
        }

        // once start terminating thread, wait until it is stopped
        // race condition with multiple inheretors/threads stopping
        if (std::this_thread::get_id() != m_thread.get_id())
        {
            waitForThreadState(ThreadState::STOPPED);
        }
    }
    else if (state != ThreadState::TERMINATING && state != ThreadState::STOPPED)
    {
        changeThreadState(ThreadState::STOPPED);
    }
}


void Thread::waitForThreadState(const ThreadState state)
{
    std::unique_lock lk(m_state_mutex);
    m_state_cv.wait(lk, [this, state]() {
        return m_thread_state == state || m_thread_state == ThreadState::STOPPED;
    });
}


void Thread::spawn()
{
    std::cout << "spawning\n";
    if (getThreadState() != ThreadState::WAITING_TO_START)
    {
        waitForThreadToFinish();

        // if user wants to re-spawn, thread should return to initial state
        changeThreadState(ThreadState::WAITING_TO_START);
    }
    m_thread = std::thread(&Thread::workerMainLoop, this);
    changeThreadState(ThreadState::INITIALIZING);
    std::cout << "INITIALIZING\n";
}

void Thread::changeThreadState(const ThreadState new_state)
{
    std::scoped_lock lk(m_state_mutex);
    if (m_thread_state.load() != new_state)
    {
        m_thread_state.store(new_state);
        m_state_cv.notify_all();
    }
}

ThreadState Thread::getThreadState() const
{
    return m_thread_state.load();
}

bool Thread::isThreadRunning() const
{
    return m_thread_state == ThreadState::RUNNING;
}

void Thread::initInThreadContext()
{
    // left up to inheretor to fill out
}

void Thread::cleanupInThreadContext()
{
    // left up to inheretor to fill out
}

std::unique_lock<std::mutex> Thread::lockOnResource()
{
    return std::unique_lock(m_resource_mutex);
}

void Thread::notifyResourceChange()
{
    m_resource_cv.notify_all();
}


void Thread::runInLockedContext(std::function<void()> fn, std::chrono::nanoseconds timeout)
{
    std::unique_lock lk(m_resource_mutex);
    const bool is_done_waiting = m_resource_cv.wait_for(
        lk,
        timeout,
        [this] {
            return !readyToStopMainLoop();
        }
    );

    std::cout << "is_done_waiting fn = " << is_done_waiting << std::endl;
    if (is_done_waiting)
    {
        fn();
        m_resource_cv.notify_all();
    }
}

void Thread::workerTask()
{
    // Up to inheretor to fill out this function
    // Called in a loop within workerMainLoop
}

bool Thread::readyToStopMainLoop()
{
    // for now just based on atomic bool, but should be overloaded to mean more
    return getThreadState() != ThreadState::RUNNING;
}

void Thread::workerMainLoop()
{
    // init when thread is ready and running
    waitForThreadState(ThreadState::INITIALIZING);
    initInThreadContext();
    changeThreadState(ThreadState::RUNNING);

    while (!readyToStopMainLoop())
    {
        std::cout << "waiting for worker task\n";
        workerTask();
        // runInLockedContext([this]() {  }, std::chrono::milliseconds(1000));
    }

    cleanupInThreadContext();
    changeThreadState(ThreadState::TERMINATING);
}

} // namespace os

