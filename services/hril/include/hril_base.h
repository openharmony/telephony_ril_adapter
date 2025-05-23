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

#ifndef OHOS_HRIL_BASE_H
#define OHOS_HRIL_BASE_H

#include <any>
#include <cstdlib>
#include <map>
#include <mutex>
#include <securec.h>

#include "hdf_remote_service.h"
#include "hdf_sbuf_ipc.h"
#include "hril_types.h"
#include "telephony_log_wrapper.h"
#include "v1_5/iril.h"
#include "v1_5/iril_callback.h"
#include "hril_notification.h"

namespace OHOS {
namespace Telephony {
class IHRilReporter {
public:
    virtual ReqDataInfo *CreateHRilRequest(int32_t serial, int32_t slotId, int32_t request) = 0;
    virtual void ReleaseHRilRequest(int32_t request, ReqDataInfo *requestInfo) = 0;
};

class HRilBase {
public:
    // The "reply" event processing entry.
    template<typename T>
    int32_t ProcessResponse(
        int32_t code, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    // The "Active reporting" event processing entry.
    template<typename T>
    int32_t ProcessNotify(
        int32_t notifyType, const struct ReportInfo *reportInfo, const void *response, size_t responseLen);
    void SetRilCallback(const sptr<HDI::Ril::V1_5::IRilCallback> &callback);
    std::string StringToHex(const char *data, int byteLength);
protected:
    HRilBase(int32_t slotId) : slotId_(slotId) {}
    virtual ~HRilBase() {}
    HRilNotiType ConvertIntToRadioNoticeType(int32_t indicationType);
    uint8_t ConvertHexCharToInt(uint8_t c);
    uint8_t *ConvertHexStringToBytes(const void *response, size_t responseLen);
    bool ConvertToString(char **dest, const std::string &src);
    void CopyToCharPoint(char **a, const std::string &temp);
    HDI::Ril::V1_1::RilRadioResponseInfo BuildIHRilRadioResponseInfo(
        const HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo);

    template<typename M>
    inline void SafeFrees(M &m)
    {
        if (m != nullptr) {
            free(m);
            m = nullptr;
        }
    }

    template<typename FuncType, typename... ParamTypes>
    inline int32_t Response(
        HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, FuncType &&_func, ParamTypes &&... _args);
    template<typename FuncType, typename... ParamTypes>
    inline int32_t Notify(int32_t notifyType, const HRilErrNumber error, FuncType &&_func, ParamTypes &&... _args);
    int32_t ConvertHexStringToInt(char **response, int32_t index, int32_t length);
    inline char *StringToCString(const std::string &src)
    {
        return static_cast<char *>(const_cast<char *>(src.c_str()));
    }

    // get slotid
    int32_t GetSlotId() const
    {
        return slotId_;
    }
    ReqDataInfo *CreateHRilRequest(int32_t serial, int32_t request);
    template<typename ReqFuncSet, typename FuncPointer, typename... ValueTypes>
    int32_t RequestVendor(
        int32_t serial, int32_t requestId, ReqFuncSet reqFuncSet, FuncPointer func, ValueTypes &&... vals);

protected:
    using RespFunc = std::function<int32_t(int32_t requestNum, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen)>;
    using NotiFunc =
        std::function<int32_t(int32_t notifyType, HRilErrNumber error, const void *response, size_t responseLen)>;
    std::map<uint32_t, RespFunc> respMemberFuncMap_;
    std::map<uint32_t, NotiFunc> notiMemberFuncMap_;
    sptr<HDI::Ril::V1_5::IRilCallback> callback_ = nullptr;

private:
    // Get the function pointer of the event handler.
    template<typename F>
    F GetFunc(std::map<uint32_t, F> &funcs, uint32_t code);
private:
    int32_t slotId_;
    std::mutex mutex_;
    sptr<HDI::Ril::V1_5::IRilCallback> GetRilCallback()
    {
        std::lock_guard<std::mutex> mutexLock(mutex_);
        return callback_;
    }
};

template<typename ReqFuncSet, typename FuncPointer, typename... ValueTypes>
int32_t HRilBase::RequestVendor(
    int32_t serial, int32_t requestId, ReqFuncSet reqFuncSet, FuncPointer func, ValueTypes &&... vals)
{
    if (reqFuncSet == nullptr || (reqFuncSet->*func) == nullptr) {
        TELEPHONY_LOGE("reqFunSet or reqFuncSet->*fun is null");
        auto callback = GetRilCallback();
        if (callback == nullptr) {
            TELEPHONY_LOGE("callback is null");
            return HRIL_ERR_NULL_POINT;
        }
        HDI::Ril::V1_1::RilRadioResponseInfo responseInfo = { 0 };
        responseInfo.slotId = GetSlotId();
        responseInfo.serial = serial;
        responseInfo.error = HDI::Ril::V1_1::RilErrType::RIL_ERR_VENDOR_NOT_IMPLEMENT;
        callback->CommonErrorResponse(responseInfo);
        return HRIL_ERR_NULL_POINT;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, requestId);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("requestInfo == nullptr: serial=%{public}d, request=%{public}d", serial, requestId);
        return HRIL_ERR_MEMORY_FULL;
    }
    (reqFuncSet->*func)(requestInfo, std::forward<ValueTypes>(vals)...);
    return HRIL_ERR_SUCCESS;
}

template<typename F>
F HRilBase::GetFunc(std::map<uint32_t, F> &funcs, uint32_t code)
{
    auto itFunc = funcs.find(code);
    if (itFunc != funcs.end()) {
        return itFunc->second;
    }
    if (code != HNOTI_NETWORK_RESTRICTED_STATE_UPDATED) {
        TELEPHONY_LOGE("Can not find Request code in func map: %{public}d", code);
    }
    return nullptr;
}

template<typename T>
int32_t HRilBase::ProcessResponse(
    int32_t code, HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    auto func = GetFunc<RespFunc>(respMemberFuncMap_, code);
    if (func != nullptr) {
        return func(code, responseInfo, response, responseLen);
    }
    return HRIL_ERR_INVALID_PARAMETER;
}

template<typename T>
int32_t HRilBase::ProcessNotify(
    int32_t notifyType, const struct ReportInfo *reportInfo, const void *response, size_t responseLen)
{
    if (reportInfo == nullptr) {
        return HRIL_ERR_INVALID_PARAMETER;
    }
    int32_t code = reportInfo->notifyId;
    HRilErrNumber error = (HRilErrNumber)reportInfo->error;
    auto func = GetFunc<NotiFunc>(notiMemberFuncMap_, code);
    if (func != nullptr) {
        return func(notifyType, error, response, responseLen);
    }
    return HRIL_ERR_INVALID_PARAMETER;
}

template<typename FuncType, typename... ParamTypes>
inline int32_t HRilBase::Response(HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, FuncType &&_func,
    ParamTypes &&... _args)
{
    auto callback = GetRilCallback();
    if (callback == nullptr || _func == nullptr) {
        TELEPHONY_LOGE("callback_ or _func is null");
        return HRIL_ERR_NULL_POINT;
    }
    (callback->*(_func))(BuildIHRilRadioResponseInfo(responseInfo), std::forward<ParamTypes>(_args)...);
    return HRIL_ERR_SUCCESS;
}

template<typename FuncType, typename... ParamTypes>
inline int32_t HRilBase::Notify(int32_t notifyType, const HRilErrNumber error, FuncType &&_func, ParamTypes &&... _args)
{
    auto callback = GetRilCallback();
    if (callback == nullptr) {
        TELEPHONY_LOGE("callback_ is null");
        return HRIL_ERR_NULL_POINT;
    }
    HDI::Ril::V1_1::RilRadioResponseInfo mResponseInfo = { 0 };
    mResponseInfo.slotId = GetSlotId();
    mResponseInfo.type = (HDI::Ril::V1_1::RilResponseTypes)notifyType;
    (callback->*(_func))(mResponseInfo, std::forward<ParamTypes>(_args)...);
    return HRIL_ERR_SUCCESS;
}
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_HRIL_UTILS_H
