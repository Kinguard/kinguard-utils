#include "kgpSysinfo.h"
#include <syslog.h>
#include <libutils/Logger.h>

using namespace OPI;
using namespace Utils;

bool debug = false;

void dprint(string s)
{
    if (debug)
    {
        printf("KGP_UTILS::SYSINFO: %s\n",s.c_str());
    }
    return;
}


kgp_sysType::kgp_sysType()
{
    this->type = sysinfo.Type();
    this->typeText = sysinfo.SysTypeText[this->type];
}

Json::Value kgp_sysType::jsonData()
{
    Json::Value ret;
    ret["type"] = this->type;
    ret["typeText"] = this->typeText;
    return ret;
}

void kgp_sysType::plainPrint()
{
    printf("type: %d\n", this->type);
    printf("typeText: %s\n", this->typeText.c_str());

}


kgp_storage::kgp_storage()
{
    this->StorageDevice = sysinfo.StorageDevice();
    this->StorageDevicePath = sysinfo.StorageDevicePath();
    this->StorageDeviceBlock = sysinfo.StorageDeviceBlock();
    this->StorageDevicePartition = sysinfo.StorageDevicePartition();
    this->BackupRootPath = sysinfo.BackupRootPath();
}

Json::Value kgp_storage::jsonData()
{
    Json::Value ret;
    ret["StorageDevicePath"] = this->StorageDevicePath;
    ret["StorageDevice"] = this->StorageDevice;
    ret["StorageDeviceBlock"] = this->StorageDeviceBlock;
    ret["StorageDevicePartition"] = this->StorageDevicePartition;
    ret["BackupRootPath"] = this->BackupRootPath;
    return ret;
}
void kgp_storage::plainPrint()
{
    printf("StorageDevicePath: %s\n", this->StorageDevicePath.c_str());
    printf("StorageDevice: %s\n", this->StorageDevice.c_str());
    printf("StorageDeviceBlock: %s\n", this->StorageDeviceBlock.c_str());
    printf("StorageDevicePartition: %s\n", this->StorageDevicePartition.c_str());
    printf("BackupRootPath: %s\n", this->BackupRootPath.c_str());
    return;
}


void help()
{
    printf("Usage: kgp-sysinfo [options]\n");
    printf("\t-d:\t\tPrint debug\n");
    printf("\tdefault:\tPrint all available system info as a Json string\n");
    printf("\t-t:\t\tPrint system type\n");
    printf("\t-s:\t\tPrint storage information\n");
    printf("\t-p:\t\tPrint plaintext istead of Json\n");
    printf("\t-i systype:\t\tCheck system type, implies '-p'\n");
    printf("\t-c 'scope' -k 'key':\tScope and Key to use in system config\n");
}

int main(int argc, char **argv)
{

    bool asJson = true;
    bool getStorage = false;
    bool getType = false;
    bool getAll = false;
    string configScope, configKey, checkType;
    int c;
    Json::Value retval(Json::objectValue);
    Json::FastWriter writer;

    while ((c = getopt (argc, argv, "dpstc:k:i:")) != -1)
    {
        switch (c)
        {
        case 'd': // output debug prints
            debug = true;
            break;
        case 'p':  // plain text output
            asJson = false;
            break;
        case 's':  // output storage information
            getStorage = true;
            break;
        case 't':  // output type information
            getType = true;
            break;
        case 'c':  // config scope
            configScope = optarg;
            break;
        case 'k':  // output type information
            configKey = optarg;
            break;
        case 'i':  // output type information
            checkType = optarg;
            break;
        default:
            help();
            return 1;
        }
    }


    // Divert logger to syslog
    openlog( "kgp-sysinfo", 0, LOG_DAEMON);
    logg.SetOutputter( [](const string& msg){ syslog(LOG_INFO, "%s",msg.c_str());});

    getAll = ! (getType || getStorage || configKey.length() || configScope.length() || checkType.length() );
    if (getAll)
    {
        dprint("--  No specific parameter asked for, printing all sysinfo information --");
    }


    if(getType || getAll)
    {
        dprint("Type asked for");
        kgp_sysType sysType;
        if(asJson)
        {
            retval["systype"] = sysType.jsonData();
        }
        else
        {
            sysType.plainPrint();
        }

    }
    if(getStorage || getAll)
    {
        dprint("Storage asked for");
        kgp_storage storage;
        if(asJson)
        {
            retval["storage"] = storage.jsonData();
        }
        else
        {
            storage.plainPrint();
        }
    }

    if(checkType.length()) {
        bool res = checkType == sysinfo.SysTypeText[sysinfo.Type()];
        printf("%d\n",res);
        return ! res;
    }

    if( configScope.length() && configKey.length() )
    {
        SysConfig sysConfig;
        bool success=false;
        string value;
        logg << Logger::Debug << "Trying to read config parameter" << lend;
        try
        {
            value = sysConfig.GetKeyAsString(configScope,configKey);
            success=true;
        }
        catch (runtime_error e)
        {
            logg << Logger::Info << "Unable to get Key '" << configKey.c_str() << "as 'string'" << lend;
            logg << e.what() << lend;
        }

        if (! success )
        {
            try
            {
                value = to_string(sysConfig.GetKeyAsInt(configScope,configKey));
                success=true;
            }
            catch (runtime_error e)
            {
                logg << Logger::Info << "Unable to get Key '" << configKey.c_str() << "as 'int'" << lend;
                logg << e.what() << lend;
            }
        }
        if (! success )
        {
            try
            {
                value = to_string(sysConfig.GetKeyAsBool(configScope,configKey));
                success=true;
            }
            catch (runtime_error e)
            {
                logg << Logger::Info << "Unable to get Key '" << configKey.c_str() << "as 'Bool'" << lend;
                logg << e.what() << lend;
            }
        }

        if ( success )
        {
            if(asJson)
            {
                retval[configKey.c_str()] = value;
            }
            else
            {
                printf("%s\n",value.c_str());
            }
        }
        else {
            logg << "Failed to read key '" << configKey << "'" << lend;
            return 1;
        }

    }
    if(asJson)
    {
        printf("%s",writer.write(retval).c_str());
    }

	return 0;
}



