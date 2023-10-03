#include "notification_manager.h"

#include <libnotify/notify.h>

#include <memory>
#include <mutex>

namespace notification {

std::unique_ptr<NotificationManager> NotificationManager::s_instance = nullptr;

NotificationManager::~NotificationManager() {}


NotificationManager & NotificationManager::getInstance()
{
    std::call_once(
        s_init_flag,
        [](){
            s_instance.reset(new NotificationManager());
        }
    );
    return *s_instance;
}


void NotificationManager::doWork() {
    
}


/*
    notify_init("Sample");
    NotifyNotification* n = notify_notification_new ("Hello world",
                                 "some message text... bla bla",
                                  0);
    notify_notification_set_timeout(n, 10000); // 10 seconds

    if (!notify_notification_show(n, 0))
    {
        std::cerr << "show has failed" << std::endl;
        notify_uninit();
        return -1;
    }
    notify_uninit();
*/

} // end of notification namespace
