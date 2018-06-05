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

    const Json::Value theme = parsedFromString["theme"];
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

