#include "kgpSysinfo.h"

using namespace OPI;

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
    printf("\tdefault:\tPrint all available system info as a Json string\n");
    printf("\t-t:\t\tPrint system type\n");
    printf("\t-s:\t\tPrint storage information\n");
    printf("\t-p:\t\tPrint plaintext istead of Json\n");
    printf("\t-d:\t\tPrint debug\n");
}

int main(int argc, char **argv)
{

    bool asJson = true;
    bool getStorage = false;
    bool getType = false;
    bool getAll = false;
    int c;
    Json::Value retval(Json::objectValue);
    Json::FastWriter writer;

    while ((c = getopt (argc, argv, "dpst")) != -1)
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
        default:
            help();
            return 1;
        }
    }
    getAll = ! (getType || getStorage);
    if (getAll)
    {
        dprint("--  No specific parameter asked for, printing all information --");
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
    if(asJson)
    {
        printf("%s",writer.write(retval).c_str());
    }

	return 0;
}



