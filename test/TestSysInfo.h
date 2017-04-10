#include <cppunit/extensions/HelperMacros.h>
#include <kgpSysinfo.h>

using namespace CppUnit;

Json::Reader reader;


class TestSysInfo : public TestFixture
{
    CPPUNIT_TEST_SUITE(TestSysInfo);
    CPPUNIT_TEST(testJsonOutput);
    CPPUNIT_TEST(testJsonSysonly);
    CPPUNIT_TEST(testJsonStorageonly);
    CPPUNIT_TEST(testWrongFlag);
    CPPUNIT_TEST_SUITE_END();

public:
    void testJsonOutput();
    void testJsonSysonly();
    void testJsonStorageonly();

    void testWrongFlag();
};
