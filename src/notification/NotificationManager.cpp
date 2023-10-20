#include "NotificationManager.h"

#include <iostream>

namespace notification {

static constexpr std::chrono::nanoseconds context_timeout = std::chrono::nanoseconds(1000000000U);

std::unique_ptr<NotificationManager> NotificationManager::s_instance = nullptr;

NotificationManager::NotificationManager() : os::Thread()
{
    spawn();
}

NotificationManager::~NotificationManager() {}


NotificationManager & NotificationManager::getInstance()
{
    std::call_once(
        s_init_flag,
        [](){
            // constructor only ever called once
            // destructor called at program end since static variable
            s_instance.reset(new NotificationManager());
        }
    );
    return *s_instance;
}

void NotificationManager::initInThreadContext()
{
    std::cout << "init\n";
    notify_init("Notification App");
}

void NotificationManager::cleanupInThreadContext()
{
    notify_uninit();
}

bool NotificationManager::readyToStopMainLoop()
{
    std::cout << "size = " << m_notifs.size() << " && " << !isThreadRunning() << "\n";
    return m_notifs.size() < 1 && !isThreadRunning();
}

void NotificationManager::workerTask()
{
    runInLockedContext(
        [this]() { 
            std::cout << "child worker task\n";
            for (auto & [summary, notification] : m_notifs)
            {
                notification->show();
                std::cout << "Size before: " << m_notifs.size() << "\n";
                m_notifs.erase(summary);
                std::cout << "Size after: " << m_notifs.size() << "\n";
            }
        },
        std::chrono::milliseconds(1000)
    );
}

void NotificationManager::addNotification(std::string summary_text, std::chrono::nanoseconds timeout)
{
    std::cout << "adding notif \n";
    // const auto fn =
    //     [this, summary_text, timeout]() {
    //     };
    // std::cout << "added notif to list\n";
    // runInLockedContext(fn, context_timeout);
    std::unique_lock<std::mutex> lk = lockOnResource();
    m_notifs[summary_text] = std::make_unique<NotificationAdapter>(summary_text);
    m_notifs.at(summary_text)
            ->set_timeout(std::chrono::duration_cast<std::chrono::milliseconds>(timeout));
    lk.unlock();
    notifyResourceChange();
    std::cout << "done adding\n";
}

} // end of notification namespace
