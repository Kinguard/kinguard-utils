#ifndef KGPSYSINFO_H
#define KGPSYSINFO_H

#include "kgp-utils.h"
#include "Config.h"
#include <syslog.h>

#include <unistd.h>
#include <json/json.h>
#include <libopi/SysInfo.h>
#include <libopi/SysConfig.h>
#include <libopi/Secop.h>
#include <libutils/String.h>

class kgp_sysType
{
private:
    int type;
    string typeText;

public:
    kgp_sysType();
    Json::Value jsonData();
    void plainPrint();
};

class kgp_storage
{
private:
    /**
     * @brief StorageDevicePath Get complete path to storage device
     * @return  path to storage device
     */
    string StorageDevicePath;

    /**
     * @brief StorageDevice Get path to storage block device (/dev/sdg)
     * @return path to storage block device
     */
    string StorageDevice;

    /**
     * @brief StorageDeviceBlock Get storage device (sdg, mmcblk0)
     * @return name of storage device
     */
    string StorageDeviceBlock;

    /**
     * @brief StorageDevicePartition Get storage partition (1, -part1)
     * @return name of storage partition
     */
    string StorageDevicePartition;

    /**
     * @brief BackupRootPath Get base path on where remote FS for backup is mounted.
     * @return path to use for mount points, i.e. "/mnt/backup"
     */
    string BackupRootPath;

public:
    kgp_storage();
    Json::Value jsonData();
    void plainPrint();
};

#endif // KGPSYSINFO_H

