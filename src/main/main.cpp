// #include <libnotify/notify.h>
#include <iostream>
#include <version.h>

int main() {
    std::cout << "GIT_SHA1=" << GIT_SHA1 << "\n";
    std::cout << "GIT_BRANCH=" << GIT_BRANCH << "\n";
    // notify_init("Sample");
    // NotifyNotification* n = notify_notification_new ("Hello world", 
    //                              "some message text... bla bla",
    //                               0);
    // notify_notification_set_timeout(n, 10000); // 10 seconds

    // if (!notify_notification_show(n, 0)) 
    // {
    //     std::cerr << "show has failed" << std::endl;
    //     return -1;
    // }
    return EXIT_SUCCESS;
}
