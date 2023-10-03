#pragma once

#include <memory>
#include <mutex>

namespace notification {


class NotificationManager {
public:
    ~NotificationManager();
    static NotificationManager & getInstance();
    void doWork();


private:
    static std::unique_ptr<NotificationManager> s_instance;
    static inline std::once_flag s_init_flag;


    NotificationManager() = default;
    NotificationManager(const NotificationManager&)= delete;
    NotificationManager& operator=(const NotificationManager&)= delete;
    NotificationManager& operator=(const NotificationManager&& arg) = delete;
};

} // end of notification namespace
