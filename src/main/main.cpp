#include <iostream>

#include <notification_manager.h>

using NotificationManager = notification::NotificationManager;

int main() {
    NotificationManager & notif_handle = notification::NotificationManager::getInstance();
    notif_handle.doWork();

    return EXIT_SUCCESS;
}
