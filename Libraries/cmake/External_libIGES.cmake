
#    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../libIGES/

ExternalProject_Add( LIBIGES
    URL ${CMAKE_CURRENT_SOURCE_DIR}/libIGES-f29393171574.zip
	CMAKE_ARGS -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
		-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
		-DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
		-DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
		-DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
		-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
		-DSTATIC_IGES=TRUE
)
ExternalProject_Get_Property( LIBIGES INSTALL_DIR )
SET( LIBIGES_INSTALL_DIR ${INSTALL_DIR} )
