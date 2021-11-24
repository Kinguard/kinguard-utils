#ifndef KGPSYSINFO_H
#define KGPSYSINFO_H

#include "kgp-utils.h"
#include "Config.h"
#include <syslog.h>

#include <unistd.h>
#include <nlohmann/json.hpp>
#include <libopi/SysInfo.h>
#include <libopi/SysConfig.h>
#include <libopi/Secop.h>
#include <libutils/String.h>

using json = nlohmann::json;

class kgp_sysType
{
private:
    int type;
    string typeText;

public:
    kgp_sysType();
	json jsonData();
    void plainPrint();
};

class kgp_deviceinfo
{
private:
    /**
     * @brief NetworkDevice Get primary network interface
     * @return string with name of network device (fron sysinfo)
     */
    string NetworkDevice;

    /**
     * @brief SerialNumber Get the serial number from flash
     * @return string with the serial number
     */
    string SerialNumber;

public:
    kgp_deviceinfo();
	json jsonData();
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
	json jsonData();
    void plainPrint();
};

#endif // KGPSYSINFO_H

