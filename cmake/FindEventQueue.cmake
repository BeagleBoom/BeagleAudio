if (EventQueue_LIBRARIES AND EventQueue_INCLUDE_DIRS)
    # in cache already
    set(EventQueue_FOUND TRUE)
else (EventQueue_LIBRARIES AND EventQueue_INCLUDE_DIRS)

    find_path(EventQueue_INCLUDE_DIR
            NAMES
            EventQueue/Event.h
            PATHS
            /usr/include
            /usr/local/include
            /opt/local/include
            /sw/include
            )

    find_library(EventQueue_LIBRARY
            NAMES
            EventQueue
            PATHS
            /usr/lib
            /usr/local/lib
            /opt/local/lib
            /sw/lib
            )

    set(EventQueue_INCLUDE_DIRS
            ${EventQueue_INCLUDE_DIR}
            )

    if (EventQueue_LIBRARY)
        set(EventQueue_LIBRARIES
                ${EventQueue_LIBRARIES}
                ${EventQueue_LIBRARY}
                )
    endif (EventQueue_LIBRARY)

    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(EventQueue DEFAULT_MSG EventQueue_LIBRARIES EventQueue_INCLUDE_DIRS)

    mark_as_advanced(EventQueue_INCLUDE_DIRS EventQueue_LIBRARIES)

endif (EventQueue_LIBRARIES AND EventQueue_INCLUDE_DIRS)