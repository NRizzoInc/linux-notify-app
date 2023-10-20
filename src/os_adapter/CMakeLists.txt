project(os_adapter)

SET(SOURCES
    Thread.cpp
)

SET(INCLUDES
    public/Thread.h
)

add_library(${PROJECT_NAME}
    ${SOURCES}
    ${INCLUDES}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC public
)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)

    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${LIBNOTIFY_LIBRARIES}
        PRIVATE Threads::Threads
    )
endif()

