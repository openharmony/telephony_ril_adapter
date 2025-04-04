/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "hril_hdf.h"
#ifdef UDEV_SUPPORT
#include <libudev.h>
#endif
#include "dlfcn.h"
#include "hdf_base.h"
#include "hril_enum.h"
#include "modem_adapter.h"
#include "parameter.h"
#include "stdlib.h"
#include "telephony_log_c.h"
#include "securec.h"

#define RIL_VENDOR_LIB_PATH "const.sys.radio.vendorlib.path"
#define VIRTUAL_MODEM_SWITCH  "const.booster.virtual_modem_switch"
#define VIRTUAL_MODEM_DEFAULT_SWITCH  "false"
#define TEL_SIM_SLOT_COUNT "const.telephony.slotCount"
#define DEFAULT_SLOT_COUNT "1"
#define BASE_HEX 16

static void *g_dlHandle = NULL;
static struct HRilReport g_reportOps = {
    OnCallReport,
    OnDataReport,
    OnModemReport,
    OnNetworkReport,
    OnSimReport,
    OnSmsReport,
    OnTimerCallback
};

static int32_t GetVendorLibPath(char *path)
{
    int32_t code = -1;
    code = GetParameter(RIL_VENDOR_LIB_PATH, "", path, PARAMETER_SIZE);
    char simSlotCount[PARAMETER_SIZE] = {0};
    GetParameter(TEL_SIM_SLOT_COUNT, DEFAULT_SLOT_COUNT, simSlotCount, PARAMETER_SIZE);
    int32_t slotCount = atoi(simSlotCount);
    char virtualModemSwitch[PARAMETER_SIZE] = {0};
    GetParameter(VIRTUAL_MODEM_SWITCH, VIRTUAL_MODEM_DEFAULT_SWITCH, virtualModemSwitch, PARAMETER_SIZE);
    if (slotCount == 0 && strcmp(virtualModemSwitch, "true") == 0) {
        if (strcpy_s(path, PARAMETER_SIZE, "libril_msgtransfer.z.so") == EOK) {
            TELEPHONY_LOGI("virtualModemSwitch on set path libril_msgtransfer.z.so");
            code = 1;
        }
    }
    if (code > 0) {
        return HDF_SUCCESS;
    }
    TELEPHONY_LOGE("Failed to get vendor library path through system properties. err:%{public}d", code);
    return HDF_FAILURE;
}

#ifdef UDEV_SUPPORT
static UsbDeviceInfo *GetPresetInformation(const char *vId, const char *pId)
{
    if (vId == NULL || pId == NULL) {
        return NULL;
    }
    char *out = NULL;
    UsbDeviceInfo *uDevInfo = NULL;
    int32_t idVendor = (int32_t)strtol(vId, &out, BASE_HEX);
    int32_t idProduct = (int32_t)strtol(pId, &out, BASE_HEX);
    for (uint32_t i = 0; i < sizeof(g_usbModemVendorInfo) / sizeof(UsbDeviceInfo); i++) {
        if (g_usbModemVendorInfo[i].idVendor == idVendor && g_usbModemVendorInfo[i].idProduct == idProduct) {
            TELEPHONY_LOGI("list index:%{public}d", i);
            uDevInfo = &g_usbModemVendorInfo[i];
            break;
        }
    }
    return uDevInfo;
}
#endif

static UsbDeviceInfo *GetUsbDeviceInfo(void)
{
    UsbDeviceInfo *uDevInfo = NULL;
#ifdef UDEV_SUPPORT
    struct udev *udev;
    struct udev_enumerate *enumerate;
    struct udev_list_entry *devices, *devListEntry;
    struct udev_device *dev;

    udev = udev_new();
    if (udev == NULL) {
        TELEPHONY_LOGE("Can't create udev");
        return uDevInfo;
    }
    enumerate = udev_enumerate_new(udev);
    if (enumerate == NULL) {
        TELEPHONY_LOGE("Can't create enumerate");
        udev_unref(udev);
        return uDevInfo;
    }
    udev_enumerate_add_match_subsystem(enumerate, "tty");
    udev_enumerate_scan_devices(enumerate);
    devices = udev_enumerate_get_list_entry(enumerate);
    udev_list_entry_foreach(devListEntry, devices) {
        const char *path = udev_list_entry_get_name(devListEntry);
        if (path == NULL) {
            continue;
        }
        dev = udev_device_new_from_syspath(udev, path);
        if (dev == NULL) {
            continue;
        }
        dev = udev_device_get_parent_with_subsystem_devtype(dev, "usb", "usb_device");
        if (!dev) {
            TELEPHONY_LOGE("Unable to find parent usb device.");
            return uDevInfo;
        }
        const char *cIdVendor = udev_device_get_sysattr_value(dev, "idVendor");
        const char *cIdProduct = udev_device_get_sysattr_value(dev, "idProduct");
        uDevInfo = GetPresetInformation(cIdVendor, cIdProduct);
        udev_device_unref(dev);
        if (uDevInfo != NULL) {
            break;
        }
    }
    udev_enumerate_unref(enumerate);
    udev_unref(udev);
#endif
    return uDevInfo;
}

static void LoadVendor(void)
{
    const char *rilLibPath = NULL;
    char vendorLibPath[PARAMETER_SIZE] = {0};
    // Pointer to ril init function in vendor ril
    const HRilOps *(*rilInitOps)(const struct HRilReport *) = NULL;
    // functions returned by ril init function in vendor ril
    const HRilOps *ops = NULL;

    UsbDeviceInfo *uDevInfo = GetUsbDeviceInfo();
    if (GetVendorLibPath(vendorLibPath) == HDF_SUCCESS) {
        rilLibPath = vendorLibPath;
    } else if (uDevInfo != NULL) {
        rilLibPath = uDevInfo->libPath;
    } else {
        TELEPHONY_LOGE("no vendor lib");
        return;
    }
    if (rilLibPath == NULL || rilLibPath[0] == '\0') {
        TELEPHONY_LOGE("dynamic library path is empty");
        return;
    }

    TELEPHONY_LOGI("RilInit LoadVendor start with rilLibPath:%{public}s", rilLibPath);
    g_dlHandle = dlopen(rilLibPath, RTLD_NOW);
    if (g_dlHandle == NULL) {
        TELEPHONY_LOGE("dlopen %{public}s is fail. %{public}s", rilLibPath, dlerror());
        return;
    }
    rilInitOps = (const HRilOps *(*)(const struct HRilReport *))dlsym(g_dlHandle, "RilInitOps");
    if (rilInitOps == NULL) {
        dlclose(g_dlHandle);
        TELEPHONY_LOGE("RilInit not defined or exported");
        return;
    }
    HRilInit();
    ops = rilInitOps(&g_reportOps);
    HRilRegOps(ops);
    TELEPHONY_LOGI("HRilRegOps completed");
}

void InitRilAdapter(void)
{
    LoadVendor();
}

void ReleaseRilAdapter(void)
{
    if (g_dlHandle == NULL) {
        TELEPHONY_LOGE("g_dlHandle has been null");
        return;
    }
    dlclose(g_dlHandle);
}
