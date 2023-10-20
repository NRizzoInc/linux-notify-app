project(notification)

find_package(LibNotify REQUIRED)

SET(SOURCES
    NotificationManager.cpp
    NotificationAdapter.cpp
)

SET(INCLUDES
    public/NotificationManager.h
    public/NotificationAdapter.h
)

add_library(${PROJECT_NAME}
    ${SOURCES}
    ${INCLUDES}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC public
    PUBLIC ${LIBNOTIFY_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    ${LIBNOTIFY_LIBRARIES}
    os_adapter
)
