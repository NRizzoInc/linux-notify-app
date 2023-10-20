#include <chrono>
#include <iostream>

#include <NotificationManager.h>

using NotificationManager = notification::NotificationManager;

int main() {
    NotificationManager & notif_handle = notification::NotificationManager::getInstance();
    notif_handle.addNotification("Hello World #1!", std::chrono::nanoseconds(500'000'000U));
    // notif_handle.addNotification("Hello World #2!", std::chrono::nanoseconds(500'000'000U));

    return EXIT_SUCCESS;
}
