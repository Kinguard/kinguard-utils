#include <cppunit/extensions/HelperMacros.h>
#include <libkgpnotify/kgpNotify.h>

using namespace CppUnit;

#define SPOOLDIR "/var/spool/kgp-notify/"
#define HISTORYDIR "/var/spool/kgp-notify/history/"
#define TESTAPP APP_PATH "/" NOTIFIER_APPNAME


class TestNotifier : public TestFixture
{
    CPPUNIT_TEST_SUITE(TestNotifier);

    CPPUNIT_TEST(testSimpleMsg);
    CPPUNIT_TEST(testEmptyLevel);
    CPPUNIT_TEST(testEmptyBody);
    CPPUNIT_TEST(testAckmsg);
    CPPUNIT_TEST(testAckNomsg);
    CPPUNIT_TEST(testBootCleanup);
    CPPUNIT_TEST(testPersistantCleanup);

    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

    void testSimpleMsg();
    void testEmptyLevel();
    void testEmptyBody();
    void testAckmsg();
    void testAckNomsg();
    void testBootCleanup();
    void testPersistantCleanup();
};

