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

#ifndef OHOS_HRIL_DATA_H
#define OHOS_HRIL_DATA_H

#include "hril_base.h"

namespace OHOS {
namespace Telephony {
class HRilData : public HRilBase {
public:
    HRilData(int32_t slotId);
    virtual ~HRilData();

    int32_t DeactivatePdpContext(int32_t serialId, const OHOS::HDI::Ril::V1_1::UniInfo &uniInfo);
    int32_t ActivatePdpContext(int32_t serialId, const OHOS::HDI::Ril::V1_1::DataCallInfo &dataCallInfo);
    int32_t ActivatePdpContextWithApnTypes(int32_t serialId,
        const OHOS::HDI::Ril::V1_3::DataCallInfoWithApnTypes &dataCallInfoWithApnTypes, const int32_t version);
    int32_t GetPdpContextList(int32_t serialId, const OHOS::HDI::Ril::V1_1::UniInfo &uniInfo);
    int32_t SetInitApnInfo(int32_t serialId, const OHOS::HDI::Ril::V1_1::DataProfileDataInfo &dataProfileDataInfo);
    int32_t SendDataPerformanceMode(
        int32_t serialId, const OHOS::HDI::Ril::V1_1::DataPerformanceInfo &dataPerformanceInfo);
    int32_t SendDataSleepMode(int32_t serialId, const OHOS::HDI::Ril::V1_1::DataSleepInfo &dataSleepInfo);
    int32_t ActivatePdpContextResponse(int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t DeactivatePdpContextResponse(int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t GetPdpContextListResponse(int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t SetInitApnInfoResponse(int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t PdpContextListUpdated(
        int32_t notifyType, const HRilErrNumber error, const void *response, size_t responseLen);
    int32_t DataLinkCapabilityUpdated(
        int32_t notifyType, const HRilErrNumber error, const void *response, size_t responseLen);
    int32_t NetworkSliceUrspRpt(
        int32_t notifyType, const HRilErrNumber error, const void *response, size_t responseLen);
    int32_t NetworkSliceAllowedNssaiRpt(
        int32_t notifyType, const HRilErrNumber error, const void *response, size_t responseLen);
    int32_t NetworkSliceEhplmnRpt(
        int32_t notifyType, const HRilErrNumber error, const void *response, size_t responseLen);
    int32_t ProcessDataResponse(
        int32_t code, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t ProcessDataRequest(int32_t code, struct HdfSBuf *data);
    int32_t ProcessDataNotify(const struct ReportInfo *reportInfo, const void *response, size_t responseLen);
    int32_t GetLinkCapability(int32_t serialId);
    int32_t GetLinkCapabilityResponse(int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t GetLinkBandwidthInfo(int32_t serialId, int32_t cid);
    int32_t GetLinkBandwidthInfoResponse(int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t SetLinkBandwidthReportingRule(
        int32_t serialId, const OHOS::HDI::Ril::V1_1::DataLinkBandwidthReportingRule &linkBandwidthRule);
    int32_t SetLinkBandwidthReportingRuleResponse(int32_t requestNum,
        HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SetDataProfileInfo(int32_t serialId, const OHOS::HDI::Ril::V1_1::DataProfilesInfo &dataProfilesInfo);
    int32_t SetDataProfileInfoResponse(int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t SetDataPermitted(int32_t serialId, int32_t dataPermitted);
    int32_t SetDataPermittedResponse(int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t CleanAllConnections(int32_t serialId);
    int32_t CleanAllConnectionsResponse(int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    bool IsDataRespOrNotify(uint32_t code);

    bool IsDataResponse(uint32_t code);

    bool IsDataNotification(uint32_t code);
    void AddHandlerToMap();
    void AddHandlerToMapForSlice();
    void RegisterDataFuncs(const HRilDataReq *dataFuncs);
    int32_t SendUrspDecodeResult(int32_t serialId,
        const OHOS::HDI::Ril::V1_4::UePolicyDecodeResult &uePolicyDecodeResult);
    int32_t SendUePolicySectionIdentifier(
        int32_t serialId, const OHOS::HDI::Ril::V1_4::UePolicySectionIdentifier &uePolicySectionIdentifier);
    int32_t SendImsRsdList(int32_t serialId, const OHOS::HDI::Ril::V1_4::ImsRsdList &imsRsdList);
    int32_t GetNetworkSliceAllowedNssai(
        int32_t serialId, const OHOS::HDI::Ril::V1_4::SyncAllowedNssaiInfo &syncAllowedNssai);
    int32_t GetNetworkSliceEhplmn(int32_t serialId);
    int32_t ActivatePdpContextWithApnTypesforSlice(int32_t serialId,
        const OHOS::HDI::Ril::V1_4::DataCallInfoWithApnTypesforSlice &dataCallInfoWithApnTypesforslice,
        const int32_t version);
    int32_t SendUrspDecodeResultResponse(int32_t requestNum,
        HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SendUePolicySectionIdentifierResponse(int32_t requestNum,
        HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SendImsRsdListResponse(int32_t requestNum,
        HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetNetworkSliceAllowedNssaiResponse(int32_t requestNum,
        HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetNetworkSliceEhplmnResponse(int32_t requestNum,
        HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);

private:
    void SwitchHRilDataListToHal(
        const void *response, size_t responseLen, std::vector<HDI::Ril::V1_1::SetupDataCallResultInfo> &dcResultList);
    void SwitchRilDataToHal(const HRilDataCallResponse *response, HDI::Ril::V1_1::SetupDataCallResultInfo &result);
    static void RilDataCallCharToString(const char *src, std::string dst);
    HRilDataInfo BuildDataInfo(const OHOS::HDI::Ril::V1_1::DataProfileDataInfo &dataProfileInfo);

private:
    static constexpr uint32_t HRIL_ERROR_UNSPECIFIED_RSN = 0xffff;
    const HRilDataReq *dataFuncs_ = nullptr;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_HRIL_DATA_H
