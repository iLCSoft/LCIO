# wrapper macro for installing a shared library with
# correct permissions
MACRO( INSTALL_SHARED_LIBRARY )

    # install library
    INSTALL( TARGETS ${ARGN}
        PERMISSIONS
        OWNER_READ OWNER_WRITE OWNER_EXECUTE
        GROUP_READ GROUP_EXECUTE
        WORLD_READ WORLD_EXECUTE
    )

ENDMACRO( INSTALL_SHARED_LIBRARY )

