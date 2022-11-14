/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include "callemergencynotice_fuzzer.h"

#include <cstddef>
#include <cstdint>

#include "hril_call.h"
#include "hril_manager.h"
#include "hril_notification.h"
#include "system_ability_definition.h"

using namespace OHOS::Telephony;
namespace OHOS {
constexpr int32_t SLOT_NUM = 3;

void DoSomethingInterestingWithMyAPI(const uint8_t *data, size_t size)
{
    if (data == nullptr || size == 0) {
        return;
    }

    int32_t slotId = static_cast<int32_t>(size % SLOT_NUM);
    HRilErrNumber error = static_cast<HRilErrNumber>(size);
    HRilEmergencyInfo info;
    info.index = static_cast<int32_t>(size);
    info.total = static_cast<int32_t>(size);
    info.eccNum = reinterpret_cast<char *>(const_cast<uint8_t *>(data));
    info.category = static_cast<int32_t>(size);
    info.simpresent = static_cast<int32_t>(size);
    info.mcc = reinterpret_cast<char *>(const_cast<uint8_t *>(data));
    info.abnormalService = static_cast<int32_t>(size);

    HRilManager hrilManger;
    std::shared_ptr<HRilCall> hrilCall = std::make_shared<HRilCall>(slotId, hrilManger);

    hrilCall->CallEmergencyNotice(HRIL_RESPONSE_NOTICE, error, &info, sizeof(HRilEmergencyInfo));
    return;
}
} // namespace OHOS

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    /* Run your code on data */
    OHOS::DoSomethingInterestingWithMyAPI(data, size);
    return 0;
}
