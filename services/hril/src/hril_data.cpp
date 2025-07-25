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

#include "hril_data.h"

#include "hril_notification.h"
#include "hril_request.h"

namespace OHOS {
namespace Telephony {
namespace {
const int32_t HRILOPS_ACTIVE_VERSION = 13;
}

HRilData::HRilData(int32_t slotId) : HRilBase(slotId)
{
    AddHandlerToMap();
    AddHandlerToMapForSlice();
}

HRilData::~HRilData()
{
    dataFuncs_ = nullptr;
}

bool HRilData::IsDataResponse(uint32_t code)
{
    return ((code >= HREQ_DATA_BASE) && (code < HREQ_NETWORK_BASE));
}

bool HRilData::IsDataNotification(uint32_t code)
{
    return ((code >= HNOTI_DATA_BASE) && (code < HNOTI_NETWORK_BASE));
}

bool HRilData::IsDataRespOrNotify(uint32_t code)
{
    return IsDataResponse(code) || IsDataNotification(code);
}

void HRilData::AddHandlerToMap()
{
    // Notification
    notiMemberFuncMap_[HNOTI_DATA_PDP_CONTEXT_LIST_UPDATED] =
        [this](int32_t notifyType, HRilErrNumber error, const void *response,
        size_t responseLen) { return PdpContextListUpdated(notifyType, error, response, responseLen); };
    notiMemberFuncMap_[HNOTI_DATA_LINK_CAPABILITY_UPDATED] =
        [this](int32_t notifyType, HRilErrNumber error, const void *response,
        size_t responseLen) { return DataLinkCapabilityUpdated(notifyType, error, response, responseLen); };
        
    // response
    respMemberFuncMap_[HREQ_DATA_SET_INIT_APN_INFO] =
        [this](int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response,
        size_t responseLen) { return SetInitApnInfoResponse(requestNum, responseInfo, response, responseLen); };
    respMemberFuncMap_[HREQ_DATA_SET_DATA_PROFILE_INFO] =
        [this](int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response,
        size_t responseLen) { return SetDataProfileInfoResponse(requestNum, responseInfo, response, responseLen); };
    respMemberFuncMap_[HREQ_DATA_ACTIVATE_PDP_CONTEXT] =
        [this](int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response,
        size_t responseLen) { return ActivatePdpContextResponse(requestNum, responseInfo, response, responseLen); };
    respMemberFuncMap_[HREQ_DATA_DEACTIVATE_PDP_CONTEXT] =
        [this](int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response,
        size_t responseLen) { return DeactivatePdpContextResponse(requestNum, responseInfo, response, responseLen); };
    respMemberFuncMap_[HREQ_DATA_GET_PDP_CONTEXT_LIST] =
        [this](int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response,
        size_t responseLen) { return GetPdpContextListResponse(requestNum, responseInfo, response, responseLen); };
    respMemberFuncMap_[HREQ_DATA_GET_LINK_BANDWIDTH_INFO] =
        [this](int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response,
        size_t responseLen) { return GetLinkBandwidthInfoResponse(requestNum, responseInfo, response, responseLen); };
    respMemberFuncMap_[HREQ_DATA_SET_LINK_BANDWIDTH_REPORTING_RULE] = [this](int32_t requestNum,
        HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response, size_t responseLen) {
        return SetLinkBandwidthReportingRuleResponse(requestNum, responseInfo, response, responseLen);
    };
    respMemberFuncMap_[HREQ_DATA_SET_DATA_PERMITTED] =
        [this](int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response,
        size_t responseLen) { return SetDataPermittedResponse(requestNum, responseInfo, response, responseLen); };
    respMemberFuncMap_[HREQ_DATA_GET_LINK_CAPABILITY] =
        [this](int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response,
        size_t responseLen) { return GetLinkCapabilityResponse(requestNum, responseInfo, response, responseLen); };
    respMemberFuncMap_[HREQ_DATA_CLEAN_ALL_CONNECTIONS] =
        [this](int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response,
        size_t responseLen) { return CleanAllConnectionsResponse(requestNum, responseInfo, response, responseLen); };
}

void HRilData::AddHandlerToMapForSlice()
{
    notiMemberFuncMap_[HNOTI_DATA_NETWORKSLICE_URSP_RPT] =
        [this](int32_t notifyType, HRilErrNumber error, const void *response,
        size_t responseLen) { return NetworkSliceUrspRpt(notifyType, error, response, responseLen); };
    notiMemberFuncMap_[HNOTI_DATA_NETWORKSLICE_ALLOWED_NSSAI_RPT] =
        [this](int32_t notifyType, HRilErrNumber error, const void *response,
        size_t responseLen) { return NetworkSliceAllowedNssaiRpt(notifyType, error, response, responseLen); };
    notiMemberFuncMap_[HNOTI_DATA_NETWORKSLICE_EHPLMN_RPT] =
        [this](int32_t notifyType, HRilErrNumber error, const void *response,
        size_t responseLen) { return NetworkSliceEhplmnRpt(notifyType, error, response, responseLen); };
    respMemberFuncMap_[HREQ_DATA_SEND_UEPOLICY_DECODE_RESULT] =
        [this](int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response,
        size_t responseLen) {
            return SendUrspDecodeResultResponse(requestNum, responseInfo, response, responseLen);
        };
    respMemberFuncMap_[HREQ_DATA_SEND_UE_SECTION_IDENTIFIER] =
        [this](int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response,
        size_t responseLen) {
            return SendUePolicySectionIdentifierResponse(requestNum, responseInfo, response, responseLen);
        };
    respMemberFuncMap_[HREQ_DATA_SEND_IMS_RSD_LIST] =
        [this](int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response,
        size_t responseLen) { return SendImsRsdListResponse(requestNum, responseInfo, response, responseLen); };
    respMemberFuncMap_[HREQ_DATA_SYNC_ALLOWED_NSSAI_WITH_MODEM] =
        [this](int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response,
        size_t responseLen) {
            return GetNetworkSliceAllowedNssaiResponse(requestNum, responseInfo, response, responseLen);
        };
    respMemberFuncMap_[HREQ_DATA_SYNC_EHPLMN_WITH_MODEM] =
        [this](int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response,
        size_t responseLen) { return GetNetworkSliceEhplmnResponse(requestNum, responseInfo, response, responseLen); };
}

void HRilData::SwitchRilDataToHal(const HRilDataCallResponse *response, HDI::Ril::V1_1::SetupDataCallResultInfo &result)
{
    if (response == nullptr) {
        TELEPHONY_LOGE("SwitchRilDataToHal response is null!!!");
        return;
    }
    result.active = response->active;
    result.reason = response->reason;
    result.retryTime = response->retryTime;
    result.cid = response->cid;
    result.pduSessionId = response->pduSessionId;
    result.maxTransferUnit = response->maxTransferUnit;
    result.address = (response->address == nullptr) ? "" : response->address;
    result.type = (response->type == nullptr) ? "" : response->type;
    result.dns = (response->dns == nullptr) ? "" : response->dns;
    result.dnsSec = (response->dnsSec == nullptr) ? "" : response->dnsSec;
    result.netPortName = (response->netPortName == nullptr) ? "" : response->netPortName;
    result.gateway = (response->gateway == nullptr) ? "" : response->gateway;
    result.pCscfPrimAddr = (response->pCscfPrimAddr == nullptr) ? "" : response->pCscfPrimAddr;
    result.pCscfSecAddr = (response->pCscfSecAddr == nullptr) ? "" : response->pCscfSecAddr;
}

void HRilData::SwitchHRilDataListToHal(
    const void *response, size_t responseLen, std::vector<HDI::Ril::V1_1::SetupDataCallResultInfo> &dcResultList)
{
    if (response == nullptr) {
        TELEPHONY_LOGE("SwitchHRilDataListToHal response is null!!!");
        return;
    }
    size_t dataNum = responseLen / sizeof(HRilDataCallResponse);
    const HRilDataCallResponse *dataCallResponse = (const HRilDataCallResponse *)response;
    dcResultList.resize(dataNum);

    size_t i = 0;
    while (i < dataNum) {
        SwitchRilDataToHal(&dataCallResponse[i], dcResultList[i]);
        i++;
    }
}

int32_t HRilData::DeactivatePdpContext(int32_t serialId, const OHOS::HDI::Ril::V1_1::UniInfo &uniInfo)
{
    HRilDataInfo dataInfo = {};
    dataInfo.cid = uniInfo.gsmIndex;
    dataInfo.reason = uniInfo.arg1;
    return RequestVendor(
        serialId, HREQ_DATA_DEACTIVATE_PDP_CONTEXT, dataFuncs_, &HRilDataReq::DeactivatePdpContext, &dataInfo);
}

int32_t HRilData::ActivatePdpContext(int32_t serialId, const OHOS::HDI::Ril::V1_1::DataCallInfo &dataCallInfo)
{
    HRilDataInfo dataInfo;
    dataInfo.apn = StringToCString(dataCallInfo.dataProfileInfo.apn);
    dataInfo.type = StringToCString(dataCallInfo.dataProfileInfo.protocol);
    dataInfo.roamingType = StringToCString(dataCallInfo.dataProfileInfo.roamingProtocol);
    dataInfo.userName = StringToCString(dataCallInfo.dataProfileInfo.userName);
    dataInfo.password = StringToCString(dataCallInfo.dataProfileInfo.password);
    dataInfo.verType = dataCallInfo.dataProfileInfo.authenticationType;
    dataInfo.rat = dataCallInfo.radioTechnology;
    dataInfo.roamingEnable = dataCallInfo.roamingAllowed ? 1 : 0;
    return RequestVendor(
        serialId, HREQ_DATA_ACTIVATE_PDP_CONTEXT, dataFuncs_, &HRilDataReq::ActivatePdpContext, &dataInfo);
}

int32_t HRilData::ActivatePdpContextWithApnTypes(int32_t serialId,
    const OHOS::HDI::Ril::V1_3::DataCallInfoWithApnTypes &dataCallInfoWithApnTypes, const int32_t version)
{
    if (version < HRILOPS_ACTIVE_VERSION) {
        TELEPHONY_LOGI("Call V1_1 ActivatePdpContext");
        OHOS::HDI::Ril::V1_1::DataCallInfo dataCallInfo;
        dataCallInfo.dataProfileInfo.apn = dataCallInfoWithApnTypes.dataProfileInfo.apn;
        dataCallInfo.dataProfileInfo.protocol = dataCallInfoWithApnTypes.dataProfileInfo.protocol;
        dataCallInfo.dataProfileInfo.roamingProtocol = dataCallInfoWithApnTypes.dataProfileInfo.roamingProtocol;
        dataCallInfo.dataProfileInfo.userName = dataCallInfoWithApnTypes.dataProfileInfo.userName;
        dataCallInfo.dataProfileInfo.password = dataCallInfoWithApnTypes.dataProfileInfo.password;
        dataCallInfo.dataProfileInfo.authenticationType = dataCallInfoWithApnTypes.dataProfileInfo.authenticationType;
        dataCallInfo.isRoaming = dataCallInfoWithApnTypes.isRoaming;
        dataCallInfo.roamingAllowed = dataCallInfoWithApnTypes.roamingAllowed;
        dataCallInfo.radioTechnology = dataCallInfoWithApnTypes.radioTechnology;
        return ActivatePdpContext(serialId, dataCallInfo);
    }
    TELEPHONY_LOGI("Call V1_3 ActivatePdpContextWithApnTypes");
    HRilDataInfoWithApnTypes dataInfoWithApnTypes;
    dataInfoWithApnTypes.apn = StringToCString(dataCallInfoWithApnTypes.dataProfileInfo.apn);
    dataInfoWithApnTypes.type = StringToCString(dataCallInfoWithApnTypes.dataProfileInfo.protocol);
    dataInfoWithApnTypes.roamingType = StringToCString(dataCallInfoWithApnTypes.dataProfileInfo.roamingProtocol);
    dataInfoWithApnTypes.userName = StringToCString(dataCallInfoWithApnTypes.dataProfileInfo.userName);
    dataInfoWithApnTypes.password = StringToCString(dataCallInfoWithApnTypes.dataProfileInfo.password);
    dataInfoWithApnTypes.verType = dataCallInfoWithApnTypes.dataProfileInfo.authenticationType;
    dataInfoWithApnTypes.rat = dataCallInfoWithApnTypes.radioTechnology;
    dataInfoWithApnTypes.roamingEnable = dataCallInfoWithApnTypes.roamingAllowed ? 1 : 0;
    dataInfoWithApnTypes.supportedApnTypesBitmap = dataCallInfoWithApnTypes.dataProfileInfo.supportedApnTypesBitmap;
    return RequestVendor(serialId, HREQ_DATA_ACTIVATE_PDP_CONTEXT, dataFuncs_,
        &HRilDataReq::ActivatePdpContextWithApnTypes, &dataInfoWithApnTypes);
}

int32_t HRilData::GetPdpContextList(int32_t serialId, const OHOS::HDI::Ril::V1_1::UniInfo &uniInfo)
{
    TELEPHONY_LOGD("serial %{public}d on %{public}d", uniInfo.serial, uniInfo.flag);
    return RequestVendor(serialId, HREQ_DATA_GET_PDP_CONTEXT_LIST, dataFuncs_, &HRilDataReq::GetPdpContextList);
}

int32_t HRilData::SetInitApnInfo(int32_t serialId, const OHOS::HDI::Ril::V1_1::DataProfileDataInfo &dataProfileDataInfo)
{
    HRilDataInfo dataInfo = BuildDataInfo(dataProfileDataInfo);
    return RequestVendor(serialId, HREQ_DATA_SET_INIT_APN_INFO, dataFuncs_, &HRilDataReq::SetInitApnInfo, &dataInfo);
}

int32_t HRilData::SendDataPerformanceMode(
    int32_t serialId, const OHOS::HDI::Ril::V1_1::DataPerformanceInfo &dataPerformanceInfo)
{
    HRilDataPerformanceInfo hrilDataPerformanceInfo;
    hrilDataPerformanceInfo.performanceEnable = dataPerformanceInfo.performanceEnable;
    hrilDataPerformanceInfo.enforce = dataPerformanceInfo.enforce;
    TELEPHONY_LOGI("SendDataPerformanceMode: performanceEnable=%{public}d enforce=%{public}d",
        hrilDataPerformanceInfo.performanceEnable, hrilDataPerformanceInfo.enforce);
    return RequestVendor(serialId, HREQ_DATA_SEND_DATA_PERFORMANCE_MODE, dataFuncs_,
        &HRilDataReq::SendDataPerformanceMode, &hrilDataPerformanceInfo);
}

int32_t HRilData::SendDataSleepMode(int32_t serialId, const OHOS::HDI::Ril::V1_1::DataSleepInfo &dataSleepInfo)
{
    HRilDataSleepInfo hrilDataSleepInfo;
    hrilDataSleepInfo.sleepEnable = dataSleepInfo.sleepEnable;
    TELEPHONY_LOGI("SendDataSleepMode: sleepEnable=%{public}d", hrilDataSleepInfo.sleepEnable);
    return RequestVendor(
        serialId, HREQ_DATA_SEND_DATA_SLEEP_MODE, dataFuncs_, &HRilDataReq::SendDataSleepMode, &hrilDataSleepInfo);
}

int32_t HRilData::SetDataProfileInfo(int32_t serialId, const OHOS::HDI::Ril::V1_1::DataProfilesInfo &dataProfilesInfo)
{
    int32_t size = dataProfilesInfo.profilesSize;
    if (size <= 0 || size != static_cast<int32_t>(dataProfilesInfo.profiles.size())) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    std::unique_ptr<HRilDataInfo[]> dataInfos = std::make_unique<HRilDataInfo[]>(size);
    for (int32_t i = 0; i < size; i++) {
        dataInfos[i] = BuildDataInfo(dataProfilesInfo.profiles[i]);
    }
    return RequestVendor(
        serialId, HREQ_DATA_SET_DATA_PROFILE_INFO, dataFuncs_, &HRilDataReq::SetDataProfileInfo, dataInfos.get(), size);
}

HRilDataInfo HRilData::BuildDataInfo(const OHOS::HDI::Ril::V1_1::DataProfileDataInfo &dataProfileInfo)
{
    HRilDataInfo dataInfo;
    dataInfo.cid = dataProfileInfo.profileId;
    dataInfo.apn = StringToCString(dataProfileInfo.apn);
    dataInfo.type = StringToCString(dataProfileInfo.protocol);
    dataInfo.roamingType = StringToCString(dataProfileInfo.roamingProtocol);
    dataInfo.userName = StringToCString(dataProfileInfo.userName);
    dataInfo.password = StringToCString(dataProfileInfo.password);
    dataInfo.verType = dataProfileInfo.authenticationType;
    return dataInfo;
}

int32_t HRilData::GetLinkCapability(int32_t serialId)
{
    return RequestVendor(serialId, HREQ_DATA_GET_LINK_CAPABILITY, dataFuncs_, &HRilDataReq::GetLinkCapability);
}

int32_t HRilData::GetLinkBandwidthInfo(int32_t serialId, int32_t cid)
{
    return RequestVendor(
        serialId, HREQ_DATA_GET_LINK_BANDWIDTH_INFO, dataFuncs_, &HRilDataReq::GetLinkBandwidthInfo, cid);
}

int32_t HRilData::SetLinkBandwidthReportingRule(
    int32_t serialId, const OHOS::HDI::Ril::V1_1::DataLinkBandwidthReportingRule &linkBandwidthRule)
{
    HRilLinkBandwidthReportingRule hLinkBandwidthRule;
    hLinkBandwidthRule.rat = (RatType)linkBandwidthRule.rat;
    hLinkBandwidthRule.delayMs = linkBandwidthRule.delayMs;
    hLinkBandwidthRule.delayUplinkKbps = linkBandwidthRule.delayUplinkKbps;
    hLinkBandwidthRule.delayDownlinkKbps = linkBandwidthRule.delayDownlinkKbps;
    hLinkBandwidthRule.maximumUplinkKbpsSize = linkBandwidthRule.maximumUplinkKbpsSize;
    hLinkBandwidthRule.maximumDownlinkKbpsSize = linkBandwidthRule.maximumDownlinkKbpsSize;
    hLinkBandwidthRule.maximumUplinkKbps = new int32_t[linkBandwidthRule.maximumUplinkKbpsSize];
    hLinkBandwidthRule.maximumDownlinkKbps = new int32_t[linkBandwidthRule.maximumDownlinkKbpsSize];
    TELEPHONY_LOGI("maximumUplinkKbpsSize:%{public}d, maximumDownlinkKbpsSize:%{public}d",
        linkBandwidthRule.maximumUplinkKbpsSize, linkBandwidthRule.maximumDownlinkKbpsSize);
    for (int32_t i = 0; i < hLinkBandwidthRule.maximumUplinkKbpsSize; i++) {
        hLinkBandwidthRule.maximumUplinkKbps[i] = linkBandwidthRule.maximumUplinkKbps[i];
    }
    for (int32_t i = 0; i < hLinkBandwidthRule.maximumDownlinkKbpsSize; i++) {
        hLinkBandwidthRule.maximumDownlinkKbps[i] = linkBandwidthRule.maximumDownlinkKbps[i];
    }
    int32_t resutlt = RequestVendor(serialId, HREQ_DATA_SET_LINK_BANDWIDTH_REPORTING_RULE, dataFuncs_,
        &HRilDataReq::SetLinkBandwidthReportingRule, &hLinkBandwidthRule);
    delete[] hLinkBandwidthRule.maximumUplinkKbps;
    delete[] hLinkBandwidthRule.maximumDownlinkKbps;
    return resutlt;
}

int32_t HRilData::SetDataPermitted(int32_t serialId, int32_t dataPermitted)
{
    return RequestVendor(
        serialId, HREQ_DATA_SET_DATA_PERMITTED, dataFuncs_, &HRilDataReq::SetDataPermitted, dataPermitted);
}

int32_t HRilData::CleanAllConnections(int32_t serialId)
{
    return RequestVendor(serialId, HREQ_DATA_CLEAN_ALL_CONNECTIONS, dataFuncs_, &HRilDataReq::CleanAllConnections);
}

int32_t HRilData::CleanAllConnectionsResponse(
    int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_2::IRilCallback::CleanAllConnectionsResponse);
}

int32_t HRilData::DeactivatePdpContextResponse(
    int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_1::IRilCallback::DeactivatePdpContextResponse);
}

int32_t HRilData::ActivatePdpContextResponse(
    int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if ((response == nullptr && responseLen != 0) || (responseLen % sizeof(HRilDataCallResponse)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    HDI::Ril::V1_1::SetupDataCallResultInfo result = {};
    result.reason = HRIL_ERROR_UNSPECIFIED_RSN;
    result.cid = -1;
    if (response != nullptr) {
        SwitchRilDataToHal((HRilDataCallResponse *)response, result);
    }
    return Response(responseInfo, &HDI::Ril::V1_1::IRilCallback::ActivatePdpContextResponse, result);
}

int32_t HRilData::GetPdpContextListResponse(
    int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if ((response == nullptr && responseLen != 0) || (responseLen % sizeof(HRilDataCallResponse)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    HDI::Ril::V1_1::DataCallResultList dataCallResultList = {};
    if (response != nullptr) {
        SwitchHRilDataListToHal(response, responseLen, dataCallResultList.dcList);
    }
    dataCallResultList.size = dataCallResultList.dcList.size();
    return Response(responseInfo, &HDI::Ril::V1_1::IRilCallback::GetPdpContextListResponse, dataCallResultList);
}

int32_t HRilData::SetInitApnInfoResponse(
    int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_1::IRilCallback::SetInitApnInfoResponse);
}

int32_t HRilData::SetDataProfileInfoResponse(
    int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return HRIL_ERR_SUCCESS;
}

int32_t HRilData::SetLinkBandwidthReportingRuleResponse(
    int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_1::IRilCallback::SetLinkBandwidthReportingRuleResponse);
}

int32_t HRilData::PdpContextListUpdated(
    int32_t notifyType, const HRilErrNumber error, const void *response, size_t responseLen)
{
    if ((response == nullptr) || (responseLen % sizeof(HRilDataCallResponse)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    HDI::Ril::V1_1::DataCallResultList dataCallResultList = {};
    SwitchHRilDataListToHal(response, responseLen, dataCallResultList.dcList);
    dataCallResultList.size = dataCallResultList.dcList.size();
    return Notify(notifyType, error, &HDI::Ril::V1_1::IRilCallback::PdpContextListUpdated, dataCallResultList);
}

int32_t HRilData::DataLinkCapabilityUpdated(
    int32_t notifyType, const HRilErrNumber error, const void *response, size_t responseLen)
{
    if ((response == nullptr) || (responseLen % sizeof(HRilDataLinkCapability)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    HDI::Ril::V1_1::DataLinkCapability dataLinkCapability = { 0 };
    const HRilDataLinkCapability *result = static_cast<const HRilDataLinkCapability *>(response);
    dataLinkCapability.primaryDownlinkKbps = result->primaryDownlinkKbps;
    dataLinkCapability.primaryUplinkKbps = result->primaryUplinkKbps;
    dataLinkCapability.secondaryDownlinkKbps = result->secondaryDownlinkKbps;
    dataLinkCapability.secondaryUplinkKbps = result->secondaryUplinkKbps;
    return Notify(notifyType, error, &HDI::Ril::V1_1::IRilCallback::DataLinkCapabilityUpdated, dataLinkCapability);
}

int32_t HRilData::GetLinkCapabilityResponse(
    int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if ((response == nullptr && responseLen != 0) || (responseLen % sizeof(HRilDataLinkCapability)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    HDI::Ril::V1_1::DataLinkCapability dataLinkCapability = { 0 };
    if (response != nullptr) {
        const HRilDataLinkCapability *result = static_cast<const HRilDataLinkCapability *>(response);
        dataLinkCapability.primaryDownlinkKbps = result->primaryDownlinkKbps;
        dataLinkCapability.primaryUplinkKbps = result->primaryUplinkKbps;
        dataLinkCapability.secondaryDownlinkKbps = result->secondaryDownlinkKbps;
        dataLinkCapability.secondaryUplinkKbps = result->secondaryUplinkKbps;
    }
    return Response(responseInfo, &HDI::Ril::V1_1::IRilCallback::GetLinkCapabilityResponse, dataLinkCapability);
}

int32_t HRilData::GetLinkBandwidthInfoResponse(
    int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if ((response == nullptr && responseLen != 0) || (responseLen % sizeof(HRilLinkBandwidthInfo)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    HDI::Ril::V1_1::DataLinkBandwidthInfo uplinkAndDownlinkBandwidthInfo = {};
    if (response != nullptr) {
        const HRilLinkBandwidthInfo *result = static_cast<const HRilLinkBandwidthInfo *>(response);
        uplinkAndDownlinkBandwidthInfo.cid = result->cid;
        uplinkAndDownlinkBandwidthInfo.qi = result->qi;
        uplinkAndDownlinkBandwidthInfo.dlGfbr = result->dlGfbr;
        uplinkAndDownlinkBandwidthInfo.ulGfbr = result->ulGfbr;
        uplinkAndDownlinkBandwidthInfo.dlMfbr = result->dlMfbr;
        uplinkAndDownlinkBandwidthInfo.ulMfbr = result->ulMfbr;
        uplinkAndDownlinkBandwidthInfo.ulSambr = result->ulSambr;
        uplinkAndDownlinkBandwidthInfo.dlSambr = result->dlSambr;
        uplinkAndDownlinkBandwidthInfo.averagingWindow = result->averagingWindow;
    }
    return Response(
        responseInfo, &HDI::Ril::V1_1::IRilCallback::GetLinkBandwidthInfoResponse, uplinkAndDownlinkBandwidthInfo);
}

int32_t HRilData::SetDataPermittedResponse(
    int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_1::IRilCallback::SetDataPermittedResponse);
}

void HRilData::RegisterDataFuncs(const HRilDataReq *dataFuncs)
{
    dataFuncs_ = dataFuncs;
}

int32_t HRilData::NetworkSliceUrspRpt(
    int32_t notifyType, const HRilErrNumber error, const void *response, size_t responseLen)
{
    if ((response == nullptr) || (responseLen == 0)) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    HDI::Ril::V1_4::NetworkSliceUrspInfo networksliceUrspInfo;
    uint8_t *temp = (uint8_t *)response;
    for (size_t i = 0; i < responseLen; i++) {
        networksliceUrspInfo.urspInfo.push_back(*temp);
        temp++;
    }
    return Notify(notifyType, error, &HDI::Ril::V1_4::IRilCallback::NetworkSliceUrspRpt, networksliceUrspInfo);
}

int32_t HRilData::NetworkSliceAllowedNssaiRpt(
    int32_t notifyType, const HRilErrNumber error, const void *response, size_t responseLen)
{
    if ((response == nullptr) || (responseLen == 0)) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    HDI::Ril::V1_4::NetworkSliceAllowedNssaiInfo networksliceAllowedNssaiInfo;
    uint8_t *temp = (uint8_t *)response;
    for (size_t i = 0; i < responseLen; i++) {
        networksliceAllowedNssaiInfo.allowednssaiInfo.push_back(*temp);
        temp++;
    }
    return Notify(notifyType, error,
        &HDI::Ril::V1_4::IRilCallback::NetworkSliceAllowedNssaiRpt, networksliceAllowedNssaiInfo);
}

int32_t HRilData::NetworkSliceEhplmnRpt(
    int32_t notifyType, const HRilErrNumber error, const void *response, size_t responseLen)
{
    if ((response == nullptr) || (responseLen == 0)) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    HDI::Ril::V1_4::NetworkSliceEhplmnInfo networkSliceEhplmnInfo;
    uint8_t *temp = (uint8_t *)response;
    for (size_t i = 0; i < responseLen; i++) {
        networkSliceEhplmnInfo.ehplmnInfo.push_back(*temp);
        temp++;
    }
    return Notify(notifyType, error,
        &HDI::Ril::V1_4::IRilCallback::NetworkSliceEhplmnRpt, networkSliceEhplmnInfo);
}

int32_t HRilData::SendUrspDecodeResult(int32_t serialId,
    const OHOS::HDI::Ril::V1_4::UePolicyDecodeResult &uePolicyDecodeResult)
{
    HRilUePolicyDecodeResult hriluePolicyDecodeResult;
    hriluePolicyDecodeResult.uePolicyDecodeResultInfoSize =
        static_cast<int>(uePolicyDecodeResult.uePolicyDecodeResultInfo.size());
    hriluePolicyDecodeResult.uePolicyDecodeResultInfo =
        new unsigned char[hriluePolicyDecodeResult.uePolicyDecodeResultInfoSize];
    for (int32_t i = 0; i < hriluePolicyDecodeResult.uePolicyDecodeResultInfoSize; i++) {
        hriluePolicyDecodeResult.uePolicyDecodeResultInfo[i] = uePolicyDecodeResult.uePolicyDecodeResultInfo[i];
    }
    int32_t result = RequestVendor(serialId, HREQ_DATA_SEND_UEPOLICY_DECODE_RESULT, dataFuncs_,
        &HRilDataReq::SendUrspDecodeResult, &hriluePolicyDecodeResult);
    delete[] hriluePolicyDecodeResult.uePolicyDecodeResultInfo;
    return result;
}

int32_t HRilData::SendUePolicySectionIdentifier(int32_t serialId,
    const OHOS::HDI::Ril::V1_4::UePolicySectionIdentifier &uePolicySectionIdentifier)
{
    HRilUePolicySectionIdentifier hRilUePolicySectionIdentifier;
    hRilUePolicySectionIdentifier.uePolicySectionIdentifierInfoSize =
        static_cast<int>(uePolicySectionIdentifier.uePolicySectionIdentifierInfo.size());
    hRilUePolicySectionIdentifier.uePolicySectionIdentifierInfo =
        new unsigned char[hRilUePolicySectionIdentifier.uePolicySectionIdentifierInfoSize];
    for (int32_t i = 0; i < hRilUePolicySectionIdentifier.uePolicySectionIdentifierInfoSize; i++) {
        hRilUePolicySectionIdentifier.uePolicySectionIdentifierInfo[i] =
            uePolicySectionIdentifier.uePolicySectionIdentifierInfo[i];
    }
    int32_t result = RequestVendor(serialId, HREQ_DATA_SEND_UE_SECTION_IDENTIFIER, dataFuncs_,
        &HRilDataReq::SendUePolicySectionIdentifier, &hRilUePolicySectionIdentifier);
    delete[] hRilUePolicySectionIdentifier.uePolicySectionIdentifierInfo;
    return result;
}

int32_t HRilData::SendImsRsdList(int32_t serialId, const OHOS::HDI::Ril::V1_4::ImsRsdList &imsRsdList)
{
    HRilImsRsdList hRilImsRsdList;
    hRilImsRsdList.imsRsdListInfoSize = static_cast<int>(imsRsdList.imsRsdListInfo.size());
    hRilImsRsdList.imsRsdListInfo = new unsigned char[hRilImsRsdList.imsRsdListInfoSize];
    for (int32_t i = 0; i < hRilImsRsdList.imsRsdListInfoSize; i++) {
        hRilImsRsdList.imsRsdListInfo[i] = imsRsdList.imsRsdListInfo[i];
    }
    int32_t result = RequestVendor(serialId, HREQ_DATA_SEND_IMS_RSD_LIST, dataFuncs_,
        &HRilDataReq::SendImsRsdList, &hRilImsRsdList);
    delete[] hRilImsRsdList.imsRsdListInfo;
    return result;
}

int32_t HRilData::GetNetworkSliceAllowedNssai(int32_t serialId,
    const OHOS::HDI::Ril::V1_4::SyncAllowedNssaiInfo &dsyncAllowedNssaiInfo)
{
    HRilSyncAllowedNssaiInfo hRilsyncAllowedNssaiInfo;
    hRilsyncAllowedNssaiInfo.syncAllowedNssaiInfoSize =
        static_cast<int>(dsyncAllowedNssaiInfo.syncAllowedNssaiInfo.size());
    hRilsyncAllowedNssaiInfo.syncAllowedNssaiInfo =
        new unsigned char[hRilsyncAllowedNssaiInfo.syncAllowedNssaiInfoSize];
    for (int32_t i = 0; i < hRilsyncAllowedNssaiInfo.syncAllowedNssaiInfoSize; i++) {
        hRilsyncAllowedNssaiInfo.syncAllowedNssaiInfo[i] = dsyncAllowedNssaiInfo.syncAllowedNssaiInfo[i];
    }
    int32_t result = RequestVendor(serialId, HREQ_DATA_SYNC_ALLOWED_NSSAI_WITH_MODEM, dataFuncs_,
        &HRilDataReq::GetNetworkSliceAllowedNssai, &hRilsyncAllowedNssaiInfo);
    delete[] hRilsyncAllowedNssaiInfo.syncAllowedNssaiInfo;
    return result;
}

int32_t HRilData::GetNetworkSliceEhplmn(int32_t serialId)
{
    int32_t result = RequestVendor(serialId, HREQ_DATA_SYNC_EHPLMN_WITH_MODEM, dataFuncs_,
        &HRilDataReq::GetNetworkSliceEhplmn);
    return result;
}

int32_t HRilData::ActivatePdpContextWithApnTypesforSlice(int32_t serialId,
    const OHOS::HDI::Ril::V1_4::DataCallInfoWithApnTypesforSlice &dataCallInfoWithApnTypesforslice,
    const int32_t version)
{
    if (version < HRILOPS_ACTIVE_VERSION) {
        TELEPHONY_LOGI("Call V1_1 ActivatePdpContext");
        OHOS::HDI::Ril::V1_1::DataCallInfo dataCallInfo;
        dataCallInfo.dataProfileInfo.apn = dataCallInfoWithApnTypesforslice.dataProfileInfo.apn;
        dataCallInfo.dataProfileInfo.protocol = dataCallInfoWithApnTypesforslice.dataProfileInfo.protocol;
        dataCallInfo.dataProfileInfo.roamingProtocol = dataCallInfoWithApnTypesforslice.dataProfileInfo.roamingProtocol;
        dataCallInfo.dataProfileInfo.userName = dataCallInfoWithApnTypesforslice.dataProfileInfo.userName;
        dataCallInfo.dataProfileInfo.password = dataCallInfoWithApnTypesforslice.dataProfileInfo.password;
        dataCallInfo.dataProfileInfo.authenticationType =
            dataCallInfoWithApnTypesforslice.dataProfileInfo.authenticationType;
        dataCallInfo.isRoaming = dataCallInfoWithApnTypesforslice.isRoaming;
        dataCallInfo.roamingAllowed = dataCallInfoWithApnTypesforslice.roamingAllowed;
        dataCallInfo.radioTechnology = dataCallInfoWithApnTypesforslice.radioTechnology;
        return ActivatePdpContext(serialId, dataCallInfo);
    }
    TELEPHONY_LOGI("Call V1_4 ActivatePdpContextWithApnTypesforSlice");
    HRilDataInfoWithApnTypesforSlice dataInfoWithApnTypesforSlice;
    dataInfoWithApnTypesforSlice.apn = StringToCString(dataCallInfoWithApnTypesforslice.dataProfileInfo.apn);
    dataInfoWithApnTypesforSlice.type = StringToCString(dataCallInfoWithApnTypesforslice.dataProfileInfo.protocol);
    dataInfoWithApnTypesforSlice.roamingType =
        StringToCString(dataCallInfoWithApnTypesforslice.dataProfileInfo.roamingProtocol);
    dataInfoWithApnTypesforSlice.userName = StringToCString(dataCallInfoWithApnTypesforslice.dataProfileInfo.userName);
    dataInfoWithApnTypesforSlice.password = StringToCString(dataCallInfoWithApnTypesforslice.dataProfileInfo.password);
    dataInfoWithApnTypesforSlice.verType = dataCallInfoWithApnTypesforslice.dataProfileInfo.authenticationType;
    dataInfoWithApnTypesforSlice.rat = dataCallInfoWithApnTypesforslice.radioTechnology;
    dataInfoWithApnTypesforSlice.roamingEnable = dataCallInfoWithApnTypesforslice.roamingAllowed ? 1 : 0;
    dataInfoWithApnTypesforSlice.supportedApnTypesBitmap =
        dataCallInfoWithApnTypesforslice.dataProfileInfo.supportedApnTypesBitmap;
    std::string sscmode = std::to_string(dataCallInfoWithApnTypesforslice.dataProfileInfo.sscMode);
    dataInfoWithApnTypesforSlice.sscmode = StringToCString(sscmode);
    dataInfoWithApnTypesforSlice.snssai = StringToCString(dataCallInfoWithApnTypesforslice.dataProfileInfo.snssai);
    return RequestVendor(serialId, HREQ_DATA_ACTIVATE_PDP_CONTEXT, dataFuncs_,
        &HRilDataReq::ActivatePdpContextWithApnTypesforSlice, &dataInfoWithApnTypesforSlice);
}

int32_t HRilData::SendUrspDecodeResultResponse(
    int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return HRIL_ERR_SUCCESS;
}

int32_t HRilData::SendUePolicySectionIdentifierResponse(
    int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return HRIL_ERR_SUCCESS;
}

int32_t HRilData::SendImsRsdListResponse(
    int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return HRIL_ERR_SUCCESS;
}

int32_t HRilData::GetNetworkSliceAllowedNssaiResponse(
    int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return HRIL_ERR_SUCCESS;
}

int32_t HRilData::GetNetworkSliceEhplmnResponse(
    int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return HRIL_ERR_SUCCESS;
}

} // namespace Telephony
} // namespace OHOS
