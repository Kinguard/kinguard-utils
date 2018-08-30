#include <cppunit/extensions/HelperMacros.h>
#include <kgpSysinfo.h>

using namespace CppUnit;

Json::Reader jsonreader;

#define SYSCONFIGFILE "/etc/kinguard/sysconfig.json"
#define SYSCONFIGFILE_BAK "/etc/kinguard/sysconfig.json.bak"
#define TEST_DB SRC_PATH "/test/sysconfig.json"

class TestConfig : public TestFixture
{
    CPPUNIT_TEST_SUITE(TestConfig);
    CPPUNIT_TEST(testReadJsonKey);
    CPPUNIT_TEST(testReadPlainKey);
    CPPUNIT_TEST(testWriteKey);
    CPPUNIT_TEST(testWriteBoolKey);
    CPPUNIT_TEST(testWriteNumKey);
    CPPUNIT_TEST(testWriteStringList);
    CPPUNIT_TEST(testReadJsonStringList);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

    void testReadJsonKey();
    void testReadPlainKey();
    void testWriteKey();
    void testWriteBoolKey();
    void testWriteNumKey();
    void testWriteStringList();
    void testReadJsonStringList();

};
