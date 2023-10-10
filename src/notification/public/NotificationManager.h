#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <map>

#include "NotificationAdapter.h"

namespace notification {

class NotificationManager {
public:
    ~NotificationManager();
    static NotificationManager& getInstance();

    void addNotification(std::string summary_text, std::chrono::milliseconds timeout_ms);

private:
    NotificationManager();

    // Runs main worker functions in a loop
    void workerLoop();
    // Worker function that waits for a new notification
    void waitForNotif();
    // loops through notifications, showing and removing them from the map
    void handleNotif();

    static std::unique_ptr<NotificationManager> s_instance;
    static inline std::once_flag s_init_flag;
    // key-value is a unique_ptr to the notification because the object does not allow copy-ctors
    // using map to maintain order of notifications
    std::map<std::string, std::unique_ptr<NotificationAdapter>> m_notifs;
    std::atomic_bool m_stop;
    std::mutex m_notif_mutex;
    std::condition_variable m_new_notif_cv;
    std::thread m_recv_notif_thread;

    NotificationManager(const NotificationManager&) = delete;
    NotificationManager& operator=(const NotificationManager&) = delete;
    NotificationManager& operator=(const NotificationManager&& arg) = delete;
};

}  // namespace notification
