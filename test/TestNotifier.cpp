#include "TestNotifier.h"
#include <libutils/Process.h>
#include "Config.h"
#include <stdio.h>

void testSimleMsg();

CPPUNIT_TEST_SUITE_REGISTRATION(TestNotifier);

using namespace Utils;
using namespace Notify;

void TestNotifier::setUp()
{
    // Divert logger to syslog
    openlog( NOTIFIER_APPNAME, LOG_PERROR, LOG_DAEMON);
    logg.SetOutputter( [](const string& msg){ syslog(LOG_DEBUG, "%s",msg.c_str());});
    logg.SetLogName("");

    // make sure dirs for testdata exists
    if( ! File::DirExists(SPOOLDIR))
    {
        File::MkDir(SPOOLDIR,0776);
    }
    if( ! File::DirExists(HISTORYDIR))
    {
        File::MkDir(HISTORYDIR,0776);

    }
}

void TestNotifier::tearDown()
{
    // Clean up test dirs
/*
        list<string> messages;

    messages = File::Glob(SPOOLDIR "*");
    for( const string& message: messages)
    {
        if(File::FileExists(message)) {
            File::Delete(message);
        }
    }

    messages = File::Glob(HISTORYDIR "*");
    for( const string& message: messages)
    {
        if(File::FileExists(message)) {
            File::Delete(message);
        }
    }
*/
}


void TestNotifier::testSimpleMsg()
{
    int retval;
    string output;
    string msgpath;

    tie(retval,output) = Process::Exec(TESTAPP " -l \"LOG_NOTICE\" -m \"Test Simple Message\"");
    msgpath = SPOOLDIR+String::Chomp(output);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong return code from notifier app.",1,retval);
    CPPUNIT_ASSERT_MESSAGE("App should have returned an UUID string.",String::Chomp(output).size()==36);
    CPPUNIT_ASSERT_MESSAGE("Missing message file.",File::FileExists(msgpath) == true);
}

void TestNotifier::testEmptyLevel()
{
    int retval;
    string output;

    tie(retval,output) = Process::Exec(TESTAPP " -l -m \"Test Simple Message\"");

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong return code from notifier app.",0,retval);
}

void TestNotifier::testEmptyBody()
{
    int retval;
    string output;

    tie(retval,output) = Process::Exec(TESTAPP " -l \"LOG_NOTICE\" -m ");

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong return code from notifier app.",0,retval);
}

void TestNotifier::testAckmsg()
{
    int retval;
    string output;
    string msgpath;
    string id;

    tie(retval,output) = Process::Exec(TESTAPP " -l \"LOG_NOTICE\" -m \"Test Simple Message\"");
    id = String::Chomp(output);
    msgpath = SPOOLDIR+id;

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong return code from notifier app.",1,retval);
    CPPUNIT_ASSERT_MESSAGE("App should have returned an UUID string.",id.size()==36);
    CPPUNIT_ASSERT_MESSAGE("Missing message file.",File::FileExists(msgpath) == true);

    tie(retval,output) = Process::Exec(TESTAPP " -a " + id);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong return code from notifier app.",1,retval);
    CPPUNIT_ASSERT_MESSAGE("Missing message file.",File::FileExists(msgpath) == false);
}

void TestNotifier::testAckNomsg()
{
    int retval;
    string output;
    string id;

    id = String::UUID();

    tie(retval,output) = Process::Exec(TESTAPP " -a " + id);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong return code from notifier app.",0,retval);
}

void TestNotifier::testBootCleanup()
{
    int retval;
    string output;
    string msgpath;
    string id;

    tie(retval,output) = Process::Exec(TESTAPP " -b -l \"LOG_NOTICE\" -m \"Keep on Boot Message\"");
    id = String::Chomp(output);
    msgpath = SPOOLDIR+id;

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong return code from notifier app.",1,retval);
    CPPUNIT_ASSERT_MESSAGE("App should have returned an UUID string.",id.size()==36);
    CPPUNIT_ASSERT_MESSAGE("Missing message file.",File::FileExists(msgpath) == true);

    tie(retval,output) = Process::Exec(TESTAPP " -s");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong return code from notifier app.",1,retval);
    CPPUNIT_ASSERT_MESSAGE("Missing message file.",File::FileExists(msgpath) == true);

    tie(retval,output) = Process::Exec(TESTAPP " -l \"LOG_NOTICE\" -m \"Remove on Boot Message\"");
    id = String::Chomp(output);
    msgpath = SPOOLDIR+id;

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong return code from notifier app.",1,retval);
    CPPUNIT_ASSERT_MESSAGE("App should have returned an UUID string.",id.size()==36);
    CPPUNIT_ASSERT_MESSAGE("Missing message file.",File::FileExists(msgpath) == true);

    tie(retval,output) = Process::Exec(TESTAPP " -s");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong return code from notifier app.",1,retval);
    CPPUNIT_ASSERT_MESSAGE("Message file still present.",File::FileExists(msgpath) == false);

}

void TestNotifier::testPersistantCleanup()
{
    int retval;
    string output;
    string msgpath;
    string id;

    tie(retval,output) = Process::Exec(TESTAPP " -p -l \"LOG_NOTICE\" -m \"Persistant message\"");
    id = String::Chomp(output);
    msgpath = SPOOLDIR+id;

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong return code from notifier app.",1,retval);
    CPPUNIT_ASSERT_MESSAGE("App should have returned an UUID string.",id.size()==36);
    CPPUNIT_ASSERT_MESSAGE("Missing message file.",File::FileExists(msgpath) == true);

    tie(retval,output) = Process::Exec(TESTAPP " -s");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong return code from notifier app.",1,retval);
    CPPUNIT_ASSERT_MESSAGE("Missing message file.",File::FileExists(msgpath) == true);

}

