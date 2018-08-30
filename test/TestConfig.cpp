#include <algorithm>
#include "TestConfig.h"
#include <json/json.h>
#include <libutils/Process.h>
#include <libutils/FileUtils.h>
#include <libopi/SysConfig.h>
#include <libutils/Logger.h>
#include "Config.h"
#include <libutils/String.h>


CPPUNIT_TEST_SUITE_REGISTRATION(TestConfig);

using namespace Utils;

void TestConfig::setUp()
{
    // Divert logger to syslog
    openlog( NOTIFIER_APPNAME, LOG_PERROR, LOG_DAEMON);
    logg.SetOutputter( [](const string& msg){ syslog(LOG_DEBUG, "%s",msg.c_str());});
    logg.SetLogName("");

    string ConfigDB;

    if( File::FileExists(SYSCONFIGFILE) )
    {
        if( File::FileExists(SYSCONFIGFILE_BAK) )
        {
            File::Delete(SYSCONFIGFILE_BAK);
        }
        File::Move(SYSCONFIGFILE,SYSCONFIGFILE_BAK);
    }
    ConfigDB = File::GetContentAsString(TEST_DB,true);
    File::Write(SYSCONFIGFILE, ConfigDB, 0600);
}

void TestConfig::tearDown()
{
    File::Delete(SYSCONFIGFILE);
    if( File::FileExists(SYSCONFIGFILE_BAK))
    {
        File::Move(SYSCONFIGFILE_BAK,SYSCONFIGFILE);
    }
}


void TestConfig::testReadJsonKey()
{
    string app;
    app = APP_PATH "/" SYSINFO_APPNAME " -c webapps -k theme";

    Json::Value parsedFromString, ConfigDB;
    string jsonMessage, jsonConfigDB;
    bool parsingSuccessful, parseDB;
    bool retval;

    tie(retval,jsonMessage) = Utils::Process::Exec(app);
    parsingSuccessful = jsonreader.parse(jsonMessage,parsedFromString);

    jsonConfigDB = File::GetContentAsString(TEST_DB);
    parseDB = jsonreader.parse(jsonConfigDB,ConfigDB);

    CPPUNIT_ASSERT(parsingSuccessful);
    CPPUNIT_ASSERT(parseDB);

    const Json::Value theme = parsedFromString["webapps"]["theme"];
    const Json::Value DBtheme = ConfigDB["webapps"]["theme"];

    CPPUNIT_ASSERT_EQUAL_MESSAGE("JSON parameter mismatch",DBtheme,theme);
}

void TestConfig::testReadPlainKey()
{
    string app;
    app = APP_PATH "/" SYSINFO_APPNAME " -c webapps -k theme -p";

    Json::Value ConfigDB;
    string key, jsonConfigDB;
    bool parseDB;
    bool retval;

    tie(retval,key) = Utils::Process::Exec(app);

    jsonConfigDB = File::GetContentAsString(TEST_DB);
    parseDB = jsonreader.parse(jsonConfigDB,ConfigDB);

    CPPUNIT_ASSERT(parseDB);

    const Json::Value DBtheme = ConfigDB["webapps"]["theme"];

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Plain text parameter mismatch",DBtheme.asString(),String::Chomp(key));
}

void TestConfig::testWriteKey()
{
    string app;
    string value,retstring,keyval;
    string scope="testscope";
    string key="testkey";

    OPI::SysConfig sysConfig;

    value = Utils::String::UUID();

    app = APP_PATH "/" SYSINFO_APPNAME " -c "+scope+" -k "+key+" -w "+value;
    bool retval;

    tie(retval,retstring) = Utils::Process::Exec(app);
    CPPUNIT_ASSERT_MESSAGE("Failed to write key to sysconfig",retval);

    keyval=sysConfig.GetKeyAsString(scope,key);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed to read back the key",keyval,value);

}

void TestConfig::testWriteBoolKey()
{
    string app;
    string value,retstring;
    bool keyval;
    string scope="testscope";
    string key="testBoolkey";

    OPI::SysConfig sysConfig;

    value = "True";

    app = APP_PATH "/" SYSINFO_APPNAME " -b -c "+scope+" -k "+key+" -w "+value;
    bool retval;

    tie(retval,retstring) = Utils::Process::Exec(app);
    CPPUNIT_ASSERT_MESSAGE("Failed to write boolean key to sysconfig",retval);

    keyval=sysConfig.GetKeyAsBool(scope,key);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed to read back the boolean key",keyval,true);

}

void TestConfig::testWriteNumKey()
{
    string app;
    string value,retstring;
    int keyval;
    string scope="testscope";
    string key="testNumkey";

    OPI::SysConfig sysConfig;

    value = "10";

    app = APP_PATH "/" SYSINFO_APPNAME " -n -c "+scope+" -k "+key+" -w "+value;
    bool retval;

    tie(retval,retstring) = Utils::Process::Exec(app);
    CPPUNIT_ASSERT_MESSAGE("Failed to write numeric key to sysconfig",retval);

    keyval=sysConfig.GetKeyAsInt(scope,key);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed to read back the numeric key",keyval,stoi(value));

}

void TestConfig::testWriteStringList()
{
    string app;
    string retstring;
    OPI::SysConfig sysConfig;

    string scope="testscope";
    string key="testStringList";
    list<string> listValues;
    string value1 = "foo";
    string value2 = "bar";
    string value = value1+","+value2;

    app = APP_PATH "/" SYSINFO_APPNAME " -c "+scope+" -k "+key+" -a -w "+value;
    bool retval;

    tie(retval,retstring) = Utils::Process::Exec(app);
    CPPUNIT_ASSERT_MESSAGE("Failed to write numeric key to sysconfig",retval);

    listValues=sysConfig.GetKeyAsStringList(scope,key);

    list<string>::iterator findIter;
    findIter = std::find(listValues.begin(), listValues.end(), value1);
    CPPUNIT_ASSERT_MESSAGE("Failed find value1 in list", !(findIter == listValues.end()) );

    findIter = std::find(listValues.begin(), listValues.end(), value2);
    CPPUNIT_ASSERT_MESSAGE("Failed find value2 in list", !(findIter == listValues.end()) );

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong size of list",2,(int)listValues.size());

}

void TestConfig::testReadJsonStringList()
{
    string app;
    string scope="foo";
    string key="bar";

    app = APP_PATH "/" SYSINFO_APPNAME " -c "+scope+" -k "+key;

    Json::Value parsedFromString, ConfigDB;
    string jsonMessage, jsonConfigDB;
    bool parsingSuccessful, parseDB;
    bool retval;

    tie(retval,jsonMessage) = Utils::Process::Exec(app);
    parsingSuccessful = jsonreader.parse(jsonMessage,parsedFromString);

    jsonConfigDB = File::GetContentAsString(TEST_DB);
    parseDB = jsonreader.parse(jsonConfigDB,ConfigDB);

    CPPUNIT_ASSERT(parsingSuccessful);
    CPPUNIT_ASSERT(parseDB);

    Json::Value keyVal(Json::arrayValue);
    Json::Value dbKeyVal(Json::arrayValue);

    keyVal = parsedFromString[scope][key];
    dbKeyVal = ConfigDB[scope][key];

    CPPUNIT_ASSERT_EQUAL_MESSAGE("JSON list size mismatch",dbKeyVal.size(),keyVal.size());

}
