pkg_check_modules( CPPUNIT cppunit>=1.12.1 )

set( testapp_src
	test.cpp
        TestSysInfo.cpp
    )

include_directories(
    "${PROJECT_SOURCE_DIR}/sysinfo"
	"${LIBOPI_INCLUDE_DIRS}"
)

add_definitions( -std=c++11 )
add_executable( testapp ${testapp_src} )

target_link_libraries( testapp ${CPPUNIT_LDFLAGS} ${LIBOPI_LDFLAGS} )

