#####################################
# cmake file for building f77 bins
# @author Jan Engels, DESY
#####################################

# force cmake to use g77 fortran compiler
SET( CMAKE_Fortran_COMPILER_INIT g77 )

# project name
PROJECT( LCIOF77 Fortran )

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
    # tell bin target to call this bin_XXX target
    ADD_DEPENDENCIES( f77 f77_${bin} )
    # avoid conflicts between library and binary target names
    SET_TARGET_PROPERTIES( f77_${bin} PROPERTIES OUTPUT_NAME ${bin}_F )
    # link against project library
    
    #FIXME: frtbegin g2c and gfortran needs to be tested
    #TARGET_LINK_LIBRARIES( f77_${bin} lib_LCIO lib_LCIOF77 frtbegin g2c )
    TARGET_LINK_LIBRARIES( f77_${bin} lib_LCIO lib_LCIOF77 )

    # install binary commando
    INSTALL( TARGETS f77_${bin} DESTINATION bin )
ENDFOREACH()
