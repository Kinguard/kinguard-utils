project(kinguard-utils)
cmake_minimum_required(VERSION 2.8)

include( FindPkgConfig )
pkg_check_modules ( LIBOPI REQUIRED libopi )
pkg_check_modules ( LIBJSONCPP REQUIRED jsoncpp )

set (KGP_UTILS_VERSION_MAJOR 1)
set (KGP_UTILS_VERSION_MINOR 0)
set (KGP_UTILS_APP_NAME "kinguard-utils")

set (APP_PATH "${PROJECT_BINARY_DIR}")
set (SRC_PATH "${PROJECT_SOURCE_DIR}")

if(APP_BUILD MATCHES deb)
		set ( APP_BUILD_PACKAGE 1 )
else()
		set ( APP_BUILD_LOCAL 1 )
endif()

configure_file (
	"${PROJECT_SOURCE_DIR}/Config.h.in"
	"${PROJECT_BINARY_DIR}/Config.h"
)

# Set source list for kgp-sysinfo 
set( sysinfo_src
	./sysinfo/kgpSysinfo.cpp
	)


include_directories("${PROJECT_BINARY_DIR}")

#add_subdirectory( test )


include_directories (
		"${LIBOPI_INCLUDE_DIRS}" "./include"
	)

add_definitions( -std=c++11 )

add_executable("kgp-sysinfo" "${sysinfo_src}")
target_link_libraries( "kgp-sysinfo" ${LIBOPI_LDFLAGS} )

#enable_testing()
#add_test( NAME TestApp COMMAND testapp )

install( TARGETS kgp-sysinfo DESTINATION sbin )

