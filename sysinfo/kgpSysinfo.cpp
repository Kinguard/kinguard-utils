#include "kgpSysinfo.h"
#include <syslog.h>
#include <libutils/Logger.h>
#include <algorithm>
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

json kgp_sysType::jsonData()
{
	json ret;
    ret["type"] = this->type;
    ret["typeText"] = this->typeText;
    return ret;
}

void kgp_sysType::plainPrint()
{
    printf("type: %d\n", this->type);
    printf("typeText: %s\n", this->typeText.c_str());

}


kgp_deviceinfo::kgp_deviceinfo()
{
    this->SerialNumber = sysinfo.SerialNumber();
    this->NetworkDevice = sysinfo.NetworkDevice();
}

json kgp_deviceinfo::jsonData()
{
	json ret;
    ret["SerialNumber"] = this->SerialNumber;
    ret["NetworkDevice"] = this->NetworkDevice;
    return ret;
}

void kgp_deviceinfo::plainPrint()
{
    printf("SerialNumber: %s\n", this->SerialNumber.c_str());
    printf("NetworkDevice: %s\n", this->NetworkDevice.c_str());
}


kgp_storage::kgp_storage()
{
    this->StorageDevice = sysinfo.StorageDevice();
    this->StorageDevicePath = sysinfo.StorageDevicePath();
    this->StorageDeviceBlock = sysinfo.StorageDeviceBlock();
    this->StorageDevicePartition = sysinfo.StorageDevicePartition();
    this->BackupRootPath = sysinfo.BackupRootPath();
}

json kgp_storage::jsonData()
{
	json ret;
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
    printf("\t-l:\t\tCheck if system is locked, implies '-p'\n");
	printf("\t-q:\t\tCheck if a config varialbe exists and only set return value, needs -l,-i or -c & -k\n");
	printf("\t-i systype:\t\tCheck system type, implies '-p'\n");
    printf("\t-c 'scope' -k 'key':\tScope and Key to use in system config\n");
    printf("\t-w 'value' -c 'scope' -k 'key':\tWrite 'value' to 'scope->key' (default written as a string to sysconfig)\n");
    printf("\t-b:\t\tValue to write to config is boolean\n");
    printf("\t-n:\t\tValue to write to config is numeric\n");
    printf("\t-a:\t\tValue to write to config is a comma separated list of strings\n");
}

string str_tolower(string s) {
    transform(s.begin(), s.end(), s.begin(),
              [](unsigned char c){ return tolower(c); } // correct
              );
    return s;
}

int main(int argc, char **argv)
{

    bool asJson = true;
    bool getStorage = false;
    bool getType = false;
    bool getDeviceInfo = false;
    bool getAll = false;
    bool checkLocked = false;
    bool isNumeric = false;
    bool isBool = false;
    bool isList = false;
	bool quiet = false;
    string configScope, configKey, configValue, checkType;
    int c;
	json retval;

    if ( argc == 1 )
    {
        // get all syinfo data if no args are passed.
        getAll = true;
    }
    else
    {
		while ((c = getopt (argc, argv, "abc:dhi:k:lnpstuqw:")) != -1)
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
            case 'k':  // key for sysconfig
                configKey = optarg;
                break;
            case 'w':  // value to write to sysconfig
                configValue = optarg;
                break;
            case 'i':  // type to test for
                checkType = optarg;
                break;
            case 'l':  // check if locked
                checkLocked = true;
                break;
            case 'a':  // arg to write to config is list of strings
                isList = true;
                break;
            case 'b':  // arg to write to config is boolean
                isBool = true;
                break;
            case 'n':  // arg to write to config is numeric
                isNumeric = true;
                break;
            case 'u':
                getDeviceInfo = true;
                break;
			case 'q':
				quiet = true;
				break;
            case 'h':
            default:
                help();
                return 1;
            }
        }
    }

    // Divert logger to syslog
    openlog( "kgp-sysinfo", 0, LOG_DAEMON);
    logg.SetOutputter( [](const string& msg){ syslog(LOG_INFO, "%s",msg.c_str());});

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
    if(getDeviceInfo || getAll)
    {
        dprint("Printing device info");
        kgp_deviceinfo DeviceInfo;
        if(asJson)
        {
            retval["deviceinfo"] = DeviceInfo.jsonData();
        }
        else
        {
            DeviceInfo.plainPrint();
        }

    }

    if(checkType.length()) {
        bool res = checkType == sysinfo.SysTypeText[sysinfo.Type()];
		if ( ! quiet )
		{
			printf("%d\n",res);
		}
        return ! res;
    }

    if(checkLocked) {
        Secop::State st = Secop::Unknown;

        try
        {
            Secop s;
            st  = s.Status();
        }
        catch( runtime_error& e)
        {
            logg << Logger::Info << "Failed to check status: "<<e.what()<<lend;
        }
        bool res = (st == Secop::Uninitialized) || (st == Secop::Unknown);
		if ( ! quiet )
		{
			printf("%d\n",res);
		}
        return ! res;
    }

    if( configScope.length() ||  configKey.length() || configValue.length() )
    {
        bool success=false;
        if ( !( configScope.length() &&  configKey.length() ) )
        {
            printf("Invalid parameters\n");
            help();
            return 1;
        }
        if ( configValue.length() )
        {
            SysConfig sysConfig(true);
            if (isNumeric && isBool)
            {
                logg << Logger::Error << "Value to write can not be both boolean and numeric" << lend;
                return 1;
            }
            logg << Logger::Debug << "Trying to write config parameter" << lend;
            try
            {
                try {
                    if (isNumeric)
                    {
                        sysConfig.PutKey(configScope,configKey,stoi(configValue));
                    }
                    else if (isBool)
                    {
                        bool boolval = false;
                        configValue = str_tolower(configValue);
                        if (configValue == "1" || configValue == "true" )
                        {
                            boolval = true;
                        }
                        sysConfig.PutKey(configScope,configKey,boolval);
                    }
                    else if (isList)
                    {
                        list<string> listVals;
                        Utils::String::Split(configValue,listVals,",");

                        sysConfig.PutKey(configScope,configKey,listVals);
                    }

                    else
                    {
                        sysConfig.PutKey(configScope,configKey,configValue);
                    }
                    return 0;
                }
                catch (const invalid_argument& ia) {
                    dprint("Invalid argument: ");
                    dprint(ia.what());
                    logg << Logger::Error << "Invalid argument: " << ia.what() << lend;
                    return 1;
                }
            }
            catch (runtime_error& e)
            {
                logg << Logger::Info << "Unable to write Key '" << configKey.c_str() << "as 'string'" << lend;
                logg << e.what() << lend;
                return 1;
            }
        }
        else
        {
            SysConfig sysConfig;
            string value;
            logg << Logger::Debug << "Trying to read config parameter" << lend;
            try
            {
                value = sysConfig.GetKeyAsString(configScope,configKey);
                success=true;
            }
            catch (runtime_error& e)
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

            if (! success )
            {
                try
                {
                    list<string> listValues;
                    // Try to get the value as list of strings
                    listValues = sysConfig.GetKeyAsStringList(configScope,configKey);

                    if(asJson)
                    {
                        // can not use the "normal" json output below, it uses a single string value.
						json jsonlist=json::array();
                        for (auto x:listValues) {
						   jsonlist.push_back(x);
                        }

                        retval[configScope.c_str()][configKey.c_str()]=jsonlist;
						cout << retval.dump();
                        return 0;
                    }
                    else
                    {
                        //join values
                        bool first=true;
                        for (auto x : listValues) {
                           if (first)
                           {
                               value = x;
                               first = false;
                           }
                           else
                           {
                               value += ","+x;
                           }
                        }
                        success=true;
                    }
                }
                catch (runtime_error e)
                {
                    logg << Logger::Info << "Unable to get Key '" << configKey.c_str() << "as 'Bool'" << lend;
                    logg << e.what() << lend;
                }
            }

            if ( success )
            {
				if ( ! quiet )
				{
					if(asJson)
					{
						retval[configScope.c_str()][configKey.c_str()] = value;
					}
					else
					{
						printf("%s\n",value.c_str());
					}
				}
            }
            else {
                logg << "Failed to read key '" << configKey << "'" << lend;
                return 1;
            }

        }
    }  // end configOps

    if(asJson)
    {
		if ( ! quiet )
		{
			cout << retval.dump();
		}
    }

	return 0;
}



