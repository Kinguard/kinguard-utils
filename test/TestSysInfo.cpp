#include "TestSysInfo.h"
#include <libutils/Process.h>
#include <libopi/SysInfo.h>
#include "Config.h"


CPPUNIT_TEST_SUITE_REGISTRATION(TestSysInfo);

using namespace OPI;


void TestSysInfo::testJsonOutput()
{
    string app;
    app = APP_PATH "/" SYSINFO_APPNAME;

	json parsedFromString;
    string jsonMessage;
    bool retval;

    tie(retval,jsonMessage) = Utils::Process::Exec(app);

	CPPUNIT_ASSERT_NO_THROW(parsedFromString = json::parse(jsonMessage) );

	const json storage = parsedFromString["storage"];
    CPPUNIT_ASSERT_MESSAGE("Storage must exist here", storage.size() > 0);

	const json systype = parsedFromString["systype"];
    CPPUNIT_ASSERT_MESSAGE("Systype must exist here", systype.size() > 0);

	int typ = systype.contains("type")?systype["type"].get<int>():0;
	CPPUNIT_ASSERT_EQUAL(typ, (int)sysinfo.Type());

}

void TestSysInfo::testJsonSysonly()
{
    // check that only the system type information is included
    string app;
    app = APP_PATH "/" SYSINFO_APPNAME " -t";

	json parsedFromString;
    string jsonMessage;
	//bool parsingSuccessful;
	bool retval;

	tie(retval,jsonMessage) = Utils::Process::Exec(app);
	//parsingSuccessful = reader.parse(jsonMessage,parsedFromString);

	CPPUNIT_ASSERT_NO_THROW(parsedFromString = json::parse(jsonMessage) );

	const json storage = parsedFromString["storage"];
    CPPUNIT_ASSERT_MESSAGE("Storage part may not exist here", storage.size() == 0 );

}

void TestSysInfo::testJsonStorageonly()
{
    // check that only the storage information is included
    string app;
    app = APP_PATH "/" SYSINFO_APPNAME " -s";

	json parsedFromString;
    string jsonMessage;
	//bool parsingSuccessful;
	bool retval;

	tie(retval,jsonMessage) = Utils::Process::Exec(app);
	//parsingSuccessful = reader.parse(jsonMessage,parsedFromString);
	CPPUNIT_ASSERT_NO_THROW(parsedFromString = json::parse(jsonMessage) );

	const json storage = parsedFromString["systype"];
    CPPUNIT_ASSERT_MESSAGE("Systype part may not exist here", storage.size() == 0 );

}

void TestSysInfo::testJsonDeviceInfo()
{
    // check that only the device info is included
    string app;
    app = APP_PATH "/" SYSINFO_APPNAME " -u";

	json parsedFromString;
    string jsonMessage;
	//bool parsingSuccessful;
	bool retval;

	tie(retval,jsonMessage) = Utils::Process::Exec(app);
	//parsingSuccessful = reader.parse(jsonMessage,parsedFromString);
	CPPUNIT_ASSERT_NO_THROW(parsedFromString = json::parse(jsonMessage) );
	CPPUNIT_ASSERT_MESSAGE("Got more than one item", parsedFromString.size() == 1 );


	const json deviceinfo = parsedFromString["deviceinfo"];
	string netdev = deviceinfo.contains("NetworkDevice")?deviceinfo["NetworkDevice"]:"";
	CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect network device", sysinfo.NetworkDevice(), netdev );
	string sernum = deviceinfo.contains("SerialNumber")?deviceinfo["SerialNumber"]:"";
	CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect network device", sysinfo.SerialNumber(), sernum );

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

void TestSysInfo::testIsLocked()
{
    string msg = "Expecting secop to be running, is it not?";;
    bool expectLocked=false;

    string app;
    app = APP_PATH "/" SYSINFO_APPNAME " -l";

    string Message;
    bool retval;

    tie(retval,Message) = Utils::Process::Exec(app);

    if (OPI::sysinfo.isPC() )
    {
        // running on PC, do not expect secop to be running.
        expectLocked=true;
        msg = "Not expecting secop to be running, is it?";
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE(msg,expectLocked,retval);

}

void TestSysInfo::testIsType()
{
    string app;
    app = APP_PATH "/" SYSINFO_APPNAME " -i " + sysinfo.SysTypeText[sysinfo.Type()];
    string Message;
    bool retval;

    tie(retval,Message) = Utils::Process::Exec(app);
    CPPUNIT_ASSERT_MESSAGE("Test for sysType failed",retval);

}

void TestSysInfo::testIsTypeFail()
{
    string app;
    app = APP_PATH "/" SYSINFO_APPNAME " -p -i foobar";
    string Message;
    bool retval;

    tie(retval,Message) = Utils::Process::Exec(app);
    CPPUNIT_ASSERT_MESSAGE("Foobar sysType test failed",!retval);

}
