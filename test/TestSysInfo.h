#include <cppunit/extensions/HelperMacros.h>
#include <kgpSysinfo.h>

using namespace CppUnit;


class TestSysInfo : public TestFixture
{
    CPPUNIT_TEST_SUITE(TestSysInfo);
    CPPUNIT_TEST(testJsonOutput);
    CPPUNIT_TEST(testJsonSysonly);
    CPPUNIT_TEST(testJsonStorageonly);
    CPPUNIT_TEST(testJsonDeviceInfo);
    CPPUNIT_TEST(testWrongFlag);
    CPPUNIT_TEST(testIsType);
    CPPUNIT_TEST(testIsTypeFail);
    CPPUNIT_TEST(testIsLocked);
    CPPUNIT_TEST_SUITE_END();

public:
    void testJsonOutput();
    void testJsonSysonly();
    void testJsonStorageonly();
    void testJsonDeviceInfo();
    void testIsType();
    void testIsTypeFail();
    void testIsLocked();

    void testWrongFlag();
};
