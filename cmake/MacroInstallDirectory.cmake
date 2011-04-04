# wrapper macro to install a directory
# excluding any backup, temporary files
# and .svn / CVS directories
MACRO( INSTALL_DIRECTORY )
    INSTALL( DIRECTORY ${ARGN}
        PATTERN "*~" EXCLUDE
        PATTERN "*#*" EXCLUDE
        PATTERN ".#*" EXCLUDE
        PATTERN "*CVS" EXCLUDE
        PATTERN "*.svn" EXCLUDE
    )
ENDMACRO( INSTALL_DIRECTORY )

