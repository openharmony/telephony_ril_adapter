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

#include "ril_radio_indication_test.h"

#include "hril_notification.h"
#include "ril_manager_test.h"
#include "telephony_log_wrapper.h"

namespace OHOS {
namespace Telephony {
RilRadioIndicationTest::RilRadioIndicationTest(RilManagerTest *mRilManager) : IPCObjectStub(std::u16string(u""))
{
    mRilManager_ = mRilManager;
}

RilRadioIndicationTest::~RilRadioIndicationTest() {}

int32_t RilRadioIndicationTest::OnRemoteRequest(
    uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    const int32_t DEFAULT_VALUE = 1;
    switch (code) {
        case HNOTI_MODEM_RADIO_STATE_UPDATED:
            RadioStateUpdated(data);
            return 0;
        case HNOTI_CALL_STATE_UPDATED:
            CallStateChgInd(data);
            return 0;
        case HNOTI_NETWORK_CS_REG_STATUS_UPDATED:
            NetworkStateNotify(data);
            return 0;
        case HNOTI_SMS_NEW_SMS:
            NewSmsNotify(data);
            return 0;
        case HNOTI_SMS_NEW_CDMA_SMS:
            NewCdmaSmsNotify(data);
            return 0;
        case HNOTI_SMS_STATUS_REPORT:
            SmsStatusReportNotify(data);
            return 0;
        case HNOTI_SMS_NEW_SMS_STORED_ON_SIM:
            NewSmsStoredOnSimNotify(data);
            return 0;
        case HNOTI_NETWORK_SIGNAL_STRENGTH_UPDATED:
            SignalStrengthUpdated(data);
            return 0;
        case HNOTI_NETWORK_PS_REG_STATUS_UPDATED:
            NetworkPsRegStatusNotify(data);
            return 0;
        case HNOTI_NETWORK_PHY_CHNL_CFG_UPDATED:
            NetworkPhyChnlCfgUpdated(data);
            return 0;
        default:
            return DEFAULT_VALUE;
    }
}

void RilRadioIndicationTest::RadioStateUpdated(MessageParcel &data)
{
    int32_t radioState = data.ReadInt32();
    int32_t indicationType = data.ReadInt32();
    TELEPHONY_LOGI(
        "func :%{public}s indicationType: %{public}d state:%{public}d", __func__, indicationType, radioState);
}

void RilRadioIndicationTest::CallStateChgInd(MessageParcel &data)
{
    int32_t indicationType = data.ReadInt32();
    TELEPHONY_LOGI("func :%{public}s indicationType: %{public}d", __func__, indicationType);
}

void RilRadioIndicationTest::NetworkStateNotify(MessageParcel &data)
{
    int32_t indicationType = data.ReadInt32();
    TELEPHONY_LOGI("func :%{public}s indicationType: %{public}d", __func__, indicationType);
}

void RilRadioIndicationTest::NewSmsNotify(MessageParcel &data)
{
    TELEPHONY_LOGI("NewSmsNotify");
    std::unique_ptr<SmsMessageInfo> smsMessageInfo = std::make_unique<SmsMessageInfo>();
    if (smsMessageInfo == nullptr) {
        return;
    }
    smsMessageInfo.get()->ReadFromParcel(data);
    int32_t indicationType = smsMessageInfo->indicationType;
    TELEPHONY_LOGI("func :%{public}s indicationType: %{public}d", __func__, indicationType);
}

void RilRadioIndicationTest::NewCdmaSmsNotify(MessageParcel &data)
{
    std::unique_ptr<SmsMessageInfo> smsMessageInfo = std::make_unique<SmsMessageInfo>();
    if (smsMessageInfo == nullptr) {
        return;
    }
    smsMessageInfo.get()->ReadFromParcel(data);
    int32_t indicationType = smsMessageInfo->indicationType;
    TELEPHONY_LOGI("func :%{public}s indicationType: %{public}d", __func__, indicationType);
}

void RilRadioIndicationTest::SmsStatusReportNotify(MessageParcel &data)
{
    TELEPHONY_LOGI("SmsStatusReportNotify");
    std::unique_ptr<SmsMessageInfo> smsMessageInfo = std::make_unique<SmsMessageInfo>();
    if (smsMessageInfo == nullptr) {
        return;
    }
    smsMessageInfo.get()->ReadFromParcel(data);
    int32_t indicationType = smsMessageInfo.get()->indicationType;
    TELEPHONY_LOGI("func :%{public}s indicationType: %{public}d", __func__, indicationType);
}

void RilRadioIndicationTest::NewSmsStoredOnSimNotify(MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioIndicationTest::NewSmsStoredOnSimNotify --> ");
    data.ReadInt32();
    int32_t indicationType = data.ReadInt32();
    TELEPHONY_LOGI("func :%{public}s indicationType: %{public}d", __func__, indicationType);
}

void RilRadioIndicationTest::SignalStrengthUpdated(MessageParcel &data)
{
    size_t readSize = sizeof(struct Rssi);
    const uint8_t *buffer = data.ReadBuffer(readSize);
    if (buffer == nullptr) {
        TELEPHONY_LOGE("SignalStrengthUpdated MessageParcel read buffer failed");
        return;
    }
    const struct Rssi *rssi = reinterpret_cast<const struct Rssi *>(buffer);
    int32_t indicationType = data.ReadInt32();
    TELEPHONY_LOGI("func :%{public}s indicationType: %{public}d", __func__, indicationType);
    struct std::unique_ptr<Rssi> mSignalStrength = std::make_unique<Rssi>();
    if (mSignalStrength == nullptr) {
        return;
    }
    if (memcpy_s(mSignalStrength.get(), readSize, rssi, readSize) != EOK) {
        TELEPHONY_LOGE("SignalStrengthUpdated memcpy_s failed");
        return;
    }
}

void RilRadioIndicationTest::NetworkPsRegStatusNotify(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioIndicationTest::NetworkPsRegStatusNotify --> ");
    int32_t indicationType = data.ReadInt32();
    TELEPHONY_LOGI("func :%{public}s indicationType: %{public}d", __func__, indicationType);
}

void RilRadioIndicationTest::NetworkPhyChnlCfgUpdated(OHOS::MessageParcel &data)
{
    std::shared_ptr<ChannelConfigInfoList> phyChnlCfgList = std::make_shared<ChannelConfigInfoList>();
    if (phyChnlCfgList == nullptr) {
        TELEPHONY_LOGE("RilRadioIndicationTest::NetworkPhyChnlCfgUpdated phyChnlCfgList == nullptr");
        return;
    }
    phyChnlCfgList->ReadFromParcel(data);
    std::vector<PhysicalChannelConfig> &configs = phyChnlCfgList->channelConfigInfos;
    for (int32_t i = 0; i < static_cast<int32_t>(configs.size()); i++) {
        TELEPHONY_LOGI(
            "func :%{public}s cellConnStatus:%{public}d cellBandwidthDownlinkKhz:%{public}d"
            "cellBandwidthUplinkKhz:%{public}d ratType:%{public}d freqRange:%{public}d downlinkChannelNum:%{public}d"
            "uplinkChannelNum:%{public}d physicalCellId:%{public}d contextIdNum:%{public}d",
            __func__, configs[i].cellConnStatus, configs[i].cellBandwidthDownlinkKhz,
            configs[i].cellBandwidthUplinkKhz, configs[i].ratType, configs[i].freqRange,
            configs[i].downlinkChannelNum, configs[i].uplinkChannelNum,
            configs[i].physicalCellId, configs[i].contextIdNum);
    }
    int32_t indicationType = data.ReadInt32();
    TELEPHONY_LOGI("func :%{public}s indicationType:%{public}d size:%{public}d",
        __func__, indicationType, phyChnlCfgList->itemNum);
}
} // namespace Telephony
} // namespace OHOS
