project(notification)

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(LibNotify REQUIRED)
find_package(Threads REQUIRED)

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
)

target_include_directories(${PROJECT_NAME}
    PUBLIC public
    PUBLIC ${LIBNOTIFY_INCLUDE_DIRS}
)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${LIBNOTIFY_LIBRARIES}
        PRIVATE Threads::Threads
    )
endif()

