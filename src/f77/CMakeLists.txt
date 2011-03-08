##############################################
# cmake file for building fortran examples
# @author Jan Engels, DESY
##############################################

# project name
PROJECT( LCIOF77 Fortran )

# ${CMAKE_Fortran_FLAGS} do not get propagated from the parent CMakeLists because
# this CMakeLists contains the line PROJECT( .... )
#IF( BUILD_32BIT_COMPATIBLE )
#    SET( CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -m32" )
#ENDIF()

#-------------------------------------------------------------------------


# fortran testjobs
SET( f77_bins
    anajob
    recjob
    simjob
    simjob_chain
)

# create symbolic bin target for calling targets bin_XXX
ADD_CUSTOM_TARGET( f77 )

# loop through list of binaries
FOREACH( bin ${f77_bins} )
    # create target for binary
    ADD_EXECUTABLE( f77_${bin} ${bin}.F )
    SET_TARGET_PROPERTIES( f77_${bin} PROPERTIES OUTPUT_NAME ${bin}_F )
    # tell bin target to call this bin_XXX target
    ADD_DEPENDENCIES( f77 f77_${bin} )
    
    #FIXME: frtbegin g2c and gfortran needs to be tested
    #TARGET_LINK_LIBRARIES( f77_${bin} lcio lcioF77 frtbegin g2c )
    TARGET_LINK_LIBRARIES( f77_${bin} lcio lcioF77 )

    # install binary commando
    INSTALL( TARGETS f77_${bin} DESTINATION bin )
ENDFOREACH()

