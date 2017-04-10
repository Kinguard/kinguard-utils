#include "TestSysInfo.h"
#include <json/json.h>
#include <libutils/Process.h>
#include <libopi/SysInfo.h>
#include "Config.h"


CPPUNIT_TEST_SUITE_REGISTRATION(TestSysInfo);

using namespace OPI;


void TestSysInfo::testJsonOutput()
{
    string app;
    app = APP_PATH "/" SYSINFO_APPNAME;

    Json::Value parsedFromString;
    string jsonMessage;
    bool parsingSuccessful;
    bool retval;

    tie(retval,jsonMessage) = Utils::Process::Exec(app);

    parsingSuccessful = reader.parse(jsonMessage,parsedFromString);
    CPPUNIT_ASSERT(parsingSuccessful);

    const Json::Value storage = parsedFromString["storage"];
    CPPUNIT_ASSERT_MESSAGE("Storage must exist here", storage.size() > 0);

    const Json::Value systype = parsedFromString["systype"];
    CPPUNIT_ASSERT_MESSAGE("Systype must exist here", systype.size() > 0);

    CPPUNIT_ASSERT_EQUAL(systype.get("type",0).asInt(), (int)sysinfo.Type());

}

void TestSysInfo::testJsonSysonly()
{
    // check that only the system type information is included
    string app;
    app = APP_PATH "/" SYSINFO_APPNAME " -t";

    Json::Value parsedFromString;
    string jsonMessage;
    bool parsingSuccessful;
    bool retval;

    tie(retval,jsonMessage) = Utils::Process::Exec(app);
    parsingSuccessful = reader.parse(jsonMessage,parsedFromString);

    const Json::Value storage = parsedFromString["storage"];
    CPPUNIT_ASSERT_MESSAGE("Storage part may not exist here", storage.size() == 0 );

}

void TestSysInfo::testJsonStorageonly()
{
    // check that only the storage information is included
    string app;
    app = APP_PATH "/" SYSINFO_APPNAME " -s";

    Json::Value parsedFromString;
    string jsonMessage;
    bool parsingSuccessful;
    bool retval;

    tie(retval,jsonMessage) = Utils::Process::Exec(app);
    parsingSuccessful = reader.parse(jsonMessage,parsedFromString);

    const Json::Value storage = parsedFromString["systype"];
    CPPUNIT_ASSERT_MESSAGE("Systype part may not exist here", storage.size() == 0 );

}

void TestSysInfo::testWrongFlag()
{
    // check that only the return value is correct when an incorrect flag is passed.
    string app;
    app = APP_PATH "/" SYSINFO_APPNAME " -q 2&> /dev/null";

    string Message;
    bool retval;

    tie(retval,Message) = Utils::Process::Exec(app);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect return value",true,retval);

}
