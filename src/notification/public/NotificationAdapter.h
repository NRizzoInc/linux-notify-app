#pragma once

#include <chrono>
#include <memory>
#include <optional>
#include <string>

#include <libnotify/notify.h>

namespace notification {

/// \brief Wrapper around the C-style libnotify-dev APIs
/// \todo Add Wrapper for windows notify APIs?
class NotificationAdapter {

public:
    NotificationAdapter(
        std::string summary,
        std::optional<std::string> body = std::nullopt,
        std::optional<std::string> icon = std::nullopt);
    ~NotificationAdapter();

    const NotificationAdapter & update(
        std::string summary,
        std::optional<std::string> body = std::nullopt,
        std::optional<std::string> icon = std::nullopt) const;
    const NotificationAdapter & set_timeout(const std::chrono::milliseconds timeout_ms) const;
    const NotificationAdapter & set_category(const std::string category) const;
    const NotificationAdapter & set_urgency(const NotifyUrgency urgency) const;
    const NotificationAdapter & set_image(GdkPixbuf * image_pix_buf) const;

    /// \brief Shows the notification to the screen. If notification is updated, must re-call this function
    bool show() const;
    bool close() const;
    bool is_alive() const;

    /// \todo: Add notification callback actions & clear

private:
    /// \brief Stores the pointer to the notification. Destruction is handled under-the-hood by the library.
    NotifyNotification * m_notif_ptr;

    NotificationAdapter() = delete;
    NotificationAdapter(const NotificationAdapter &) = delete;
    NotificationAdapter & operator=(const NotificationAdapter &) = delete;
    NotificationAdapter & operator=(const NotificationAdapter && arg) = delete;
};

}  // namespace notification
