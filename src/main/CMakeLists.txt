project(notify-app)

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
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    notification-manager
)

finalize_binary(${PROJECT_NAME})
