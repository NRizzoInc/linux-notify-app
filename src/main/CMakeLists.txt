project(notify-app)

find_package(LibNotify REQUIRED)


SET(SOURCES
    main.cpp
)

SET(INCLUDES

)

add_executable(${PROJECT_NAME}
    ${SOURCES}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${INCLUDES}
    PRIVATE ${LIBNOTIFY_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    notify-app-version
    ${LIBNOTIFY_LIBRARIES}
)

