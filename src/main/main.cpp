#include <iostream>

#include <notification_manager.h>
#include <version.h>

using NotificationManager = notification::NotificationManager;

int main() {
    std::cout << "GIT_SHA1=" << GIT_SHA1 << "\n";
    std::cout << "GIT_BRANCH=" << GIT_BRANCH << "\n";

    NotificationManager & notif_handle = notification::NotificationManager::getInstance();
    notif_handle.doWork();

    return EXIT_SUCCESS;
}
