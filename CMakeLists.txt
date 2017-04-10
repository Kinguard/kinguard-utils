project(kinguard-utils)
cmake_minimum_required(VERSION 2.8)

include( FindPkgConfig )
pkg_check_modules ( LIBOPI REQUIRED libopi )
pkg_check_modules ( LIBUTILS REQUIRED libutils>=1.5.1 )
pkg_check_modules ( LIBJSONCPP REQUIRED libjsoncpp )

set (KGP_UTILS_VERSION_MAJOR 1)
set (KGP_UTILS_VERSION_MINOR 0)
set (KGP_UTILS_APP_NAME "kinguard-utils")

set (APP_PATH "${PROJECT_BINARY_DIR}")
set (SRC_PATH "${PROJECT_SOURCE_DIR}")
set(SYSINFO_APPNAME "kgp-sysinfo")

if(APP_BUILD MATCHES deb)
		set ( APP_BUILD_PACKAGE 1 )
else()
		set ( APP_BUILD_LOCAL 1 )
endif()

add_definitions( -std=c++11 )

include_directories("${PROJECT_BINARY_DIR}" "include")
include_directories ( "${LIBOPI_INCLUDE_DIRS}" )

configure_file (
	"${PROJECT_SOURCE_DIR}/Config.h.in"
	"${PROJECT_BINARY_DIR}/Config.h"
)

# Set source list for kgp-sysinfo 
set( sysinfo_src
        "${PROJECT_SOURCE_DIR}/sysinfo/kgpSysinfo"
        )

add_executable(${SYSINFO_APPNAME} "${sysinfo_src}")
target_link_libraries(${SYSINFO_APPNAME} ${LIBOPI_LDFLAGS} )

add_subdirectory( test )

enable_testing()
add_test( NAME TestApp COMMAND testapp )

install( TARGETS ${SYSINFO_APPNAME} DESTINATION sbin )

