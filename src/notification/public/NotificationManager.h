#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <map>

#include "NotificationAdapter.h"
#include "Thread.h"

namespace notification {

class NotificationManager : public os::Thread {
public:
    ~NotificationManager();
    static NotificationManager& getInstance();

    void addNotification(std::string summary_text, std::chrono::nanoseconds timeout);

protected:
    /// \brief Funtion to overload to perform some operations before the thread starts
    virtual void initInThreadContext() override;

    /// \brief Funtion to overload to perform some operations when cleaning up the thread
    virtual void cleanupInThreadContext() override;

    /// \brief Should be overloaded and return true when thread main loop should terminate
    /// \returns True when thread should stop. False if thread should keep going
    virtual bool readyToStopMainLoop() override;

    /// \brief Iterates over notifications list, showing and removing them from the map
    /// \note Is runs in workerMainLoop() repeatedly until program ends
    virtual void workerTask() override;

private:
    NotificationManager();

    static std::unique_ptr<NotificationManager> s_instance;
    static inline std::once_flag s_init_flag;
    // key-value is a unique_ptr to the notification because the object does not allow copy-ctors
    // using map to maintain order of notifications
    std::map<std::string, std::unique_ptr<NotificationAdapter>> m_notifs;

    NotificationManager(const NotificationManager&) = delete;
    NotificationManager& operator=(const NotificationManager&) = delete;
    NotificationManager& operator=(const NotificationManager&& arg) = delete;
};

}  // namespace notification
