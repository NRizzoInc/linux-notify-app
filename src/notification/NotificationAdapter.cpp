#include "NotificationAdapter.h"

namespace notification {

NotificationAdapter::NotificationAdapter(
    std::string summary,
    std::optional<std::string> body,
    std::optional<std::string> icon)
    : m_notif_ptr(notify_notification_new(
        summary.c_str(),
        body ? (*body).c_str() : 0,
        icon ? (*icon).c_str() : 0))
{}

NotificationAdapter::~NotificationAdapter() {}

const NotificationAdapter & NotificationAdapter::update(
    std::string summary,
    std::optional<std::string> body,
    std::optional<std::string> icon) const
{
    notify_notification_update(
        m_notif_ptr,
        summary.c_str(),
        body ? (*body).c_str() : 0,
        icon ? (*icon).c_str() : 0);
    return *this;
}

const NotificationAdapter & NotificationAdapter::set_timeout(
    const std::chrono::milliseconds timeout_ms) const
{
    notify_notification_set_timeout(m_notif_ptr, timeout_ms.count());
    return *this;
}

const NotificationAdapter & NotificationAdapter::set_category(
    const std::string category) const
{
    notify_notification_set_category(m_notif_ptr, category.c_str());
    return *this;
}

const NotificationAdapter & NotificationAdapter::set_urgency(
    const NotifyUrgency urgency) const
{
    notify_notification_set_urgency(m_notif_ptr, urgency);
    return *this;
}

const NotificationAdapter & NotificationAdapter::set_image(
    GdkPixbuf * image_pix_buf) const
{
    notify_notification_set_image_from_pixbuf(m_notif_ptr, image_pix_buf);
    return *this;
}

bool NotificationAdapter::show() const
{
    return notify_notification_show(m_notif_ptr, nullptr);
}

bool NotificationAdapter::close() const
{
    return notify_notification_close(m_notif_ptr, nullptr);
}

bool NotificationAdapter::is_alive() const
{
    #pragma message("NotificationAdapter::is_alive is just a stub")
    return true;
}


}  // namespace notification
