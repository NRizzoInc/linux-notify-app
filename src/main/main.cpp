#include <chrono>
#include <iostream>

#include <NotificationManager.h>

using NotificationManager = notification::NotificationManager;

int main() {
    NotificationManager & notif_handle = notification::NotificationManager::getInstance();
    notif_handle.addNotification("Hello World #1!", std::chrono::milliseconds(5));
    notif_handle.addNotification("Hello World #2!", std::chrono::milliseconds(5));

    return EXIT_SUCCESS;
}
