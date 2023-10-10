#include "NotificationManager.h"

#include <iostream>

namespace notification {

std::unique_ptr<NotificationManager> NotificationManager::s_instance = nullptr;

NotificationManager::NotificationManager()
    : m_stop(false), m_recv_notif_thread([this]() { workerLoop(); })
{
    notify_init("Notification App");
}

NotificationManager::~NotificationManager()
{
    m_new_notif_cv.notify_all();
    m_stop = true;
    if (m_recv_notif_thread.joinable())
    {
        m_recv_notif_thread.join();
    }
    notify_uninit();
}


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

void NotificationManager::addNotification(std::string summary_text, std::chrono::milliseconds timeout_ms)
{
    std::scoped_lock lk(m_notif_mutex);
    m_notifs[summary_text] = std::make_unique<NotificationAdapter>(summary_text);
    m_notifs.at(summary_text)->set_timeout(timeout_ms);
    m_new_notif_cv.notify_all();
}

void NotificationManager::workerLoop()
{
    while (!m_stop || m_notifs.size() > 0)
    {
        waitForNotif();
    }

}

void NotificationManager::waitForNotif()
{
    std::unique_lock lk(m_notif_mutex);
    const bool is_done_waiting = m_new_notif_cv.wait_for(
        lk,
        std::chrono::milliseconds(500),
        [this] {
            return m_stop.load();
        }
    );

    if (is_done_waiting)
    {
        handleNotif();
    }
}

void NotificationManager::handleNotif()
{
    for (auto & [summary, notification] : m_notifs)
    {
        notification->show();
        m_notifs.erase(summary);
    }
}

} // end of notification namespace
