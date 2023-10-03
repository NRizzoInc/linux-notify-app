project(notification-manager)

find_package(LibNotify REQUIRED)

SET(SOURCES
    notification_manager.cpp
)

SET(INCLUDES
    public/notification_manager.h
)

add_library(${PROJECT_NAME}
    ${SOURCES}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC public
    PRIVATE ${LIBNOTIFY_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${LIBNOTIFY_LIBRARIES}
)

