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

#include "at_sim.h"

#include "vendor_adapter.h"
#include "vendor_report.h"

static int32_t GetSimType(void)
{
    char *pLine = NULL;
    int32_t ret;
    int32_t simType = 0;
    ResponseInfo *pResponse = NULL;

    ret = SendCommandLock("AT^CARDMODE", "^CARDMODE:", 0, &pResponse);
    if (ret != 0 || (pResponse != NULL && !pResponse->success)) {
        TELEPHONY_LOGE("AT^CARDMODE send failed");
        return HRIL_UNKNOWN;
    }
    if (pResponse->head) {
        pLine = pResponse->head->data;
    }
    ret = SkipATPrefix(&pLine);
    if (ret < 0) {
        return HRIL_UNKNOWN;
    }
    ret = NextInt(&pLine, &simType);
    if (ret < 0) {
        return HRIL_UNKNOWN;
    }
    FreeResponseInfo(pResponse);
    return simType;
}

static int32_t GetSimState(char *pLine, char *pResult, ResponseInfo *pResponse)
{
    int32_t status = HRIL_SIM_NOT_INSERTED;
    int32_t ret;

    ret = SkipATPrefix(&pLine);
    if (ret != 0) {
        return HRIL_SIM_NOT_READY;
    }
    ret = NextStr(&pLine, &pResult);
    if (ret != 0) {
        return HRIL_SIM_NOT_READY;
    }
    if (strcmp(pResult, "READY") == 0) {
        status = HRIL_SIM_READY;
    } else if (strcmp(pResult, "SIM PIN") == 0) {
        status = HRIL_SIM_PIN;
    } else if (strcmp(pResult, "SIM PUK") == 0) {
        status = HRIL_SIM_PUK;
    } else if (strcmp(pResult, "PH-NET PIN") == 0) {
        status = HRIL_PH_NET_PIN;
    } else if (strcmp(pResult, "PH-NET PUK") == 0) {
        status = HRIL_PH_NET_PIN;
    } else if (!ReportStrWith(pResponse->result, "+CME ERROR:")) {
        status = HRIL_SIM_NOT_READY;
    }
    return status;
}

static int32_t ParseSimResponseResult(char *pLine, HRilSimIOResponse *pSimResponse)
{
    int32_t err;
    err = SkipATPrefix(&pLine);
    if (err != 0) {
        return err;
    }
    err = NextInt(&pLine, &pSimResponse->sw1);
    if (err != 0) {
        return err;
    }
    err = NextInt(&pLine, &pSimResponse->sw2);
    if (err != 0) {
        return err;
    }
    if ((pLine != NULL && *pLine != '\0')) {
        err = NextStr(&pLine, &pSimResponse->response);
        if (err != 0) {
            return err;
        }
    }
    return 0;
}

static int32_t ParseSimPinInputTimesResult(char *pLine, HRilPinInputTimes *pinInputTimes)
{
    int32_t err = HRIL_ERR_GENERIC_FAILURE;
    if (pinInputTimes == NULL) {
        TELEPHONY_LOGE("pinInputTimes is null!!!");
        return err;
    }
    err = SkipATPrefix(&pLine);
    if (err != 0) {
        return err;
    }
    err = NextStr(&pLine, &pinInputTimes->code);
    if (err != 0) {
        return err;
    }
    err = NextInt(&pLine, &pinInputTimes->times);
    err = NextInt(&pLine, &pinInputTimes->pukTimes);
    if (err != 0) {
        return err;
    }
    err = NextInt(&pLine, &pinInputTimes->pinTimes);
    if (err != 0) {
        return err;
    }
    err = NextInt(&pLine, &pinInputTimes->puk2Times);
    if (err != 0) {
        return err;
    }
    err = NextInt(&pLine, &pinInputTimes->pin2Times);
    if (err != 0) {
        return err;
    }
    return 0;
}

void ReqGetSimStatus(const ReqDataInfo *requestInfo)
{
    int32_t ret;
    ResponseInfo *pResponse = NULL;
    char *pLine = NULL;
    char *pResult = NULL;
    struct ReportInfo reportInfo = {0};
    HRilCardState cardState = {0};
    HRilRadioState radioState = GetRadioState();
    if (radioState == HRIL_RADIO_POWER_STATE_UNAVAILABLE || radioState == HRIL_RADIO_POWER_STATE_OFF) {
        cardState.simState = HRIL_SIM_NOT_READY;
    }
    cardState.simType = GetSimType();
    ret = SendCommandLock("AT+CPIN?", "+CPIN:", 0, &pResponse);
    if (ret != 0 || (pResponse != NULL && !pResponse->success)) {
        TELEPHONY_LOGE("AT+CPIN? send failed");
        if (pResponse && pResponse->result) {
            pLine = pResponse->result;
            SkipATPrefix(&pLine);
            NextInt(&pLine, &ret);
        } else {
            ret = HRIL_ERR_GENERIC_FAILURE;
        }
        if (ret == HRIL_ERR_NO_SIMCARD_INSERTED) {
            cardState.simState = HRIL_SIM_NOT_INSERTED;
            reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
            OnSimReport(HRIL_SIM_SLOT_1, reportInfo, (const uint8_t *)&cardState, sizeof(cardState));
        } else {
            reportInfo = CreateReportInfo(requestInfo, ret, HRIL_RESPONSE, 0);
            OnSimReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        }
        FreeResponseInfo(pResponse);
        return;
    }
    if (pResponse->head) {
        pLine = pResponse->head->data;
    }
    cardState.simState = GetSimState(pLine, pResult, pResponse);
    reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    OnSimReport(HRIL_SIM_SLOT_1, reportInfo, (const uint8_t *)&cardState, sizeof(cardState));
    FreeResponseInfo(pResponse);
}

static int32_t ReqGetSimIOFDN(HRilSimIO *pSim, ResponseInfo **ppResponse, size_t dataLen)
{
    char cmdCheck[MAX_CMD_LENGTH] = {0};
    char cmdActive[MAX_CMD_LENGTH] = {0};
    char cmdClose[MAX_CMD_LENGTH] = {0};

    int32_t result = GenerateCommand(cmdCheck, MAX_CMD_LENGTH, "AT+CLCK=\"%s\",%d,\"%s\"", "FD", 1, pSim->pin2);
    if (result <= 0) {
        TELEPHONY_LOGE("GenerateCommand is failed");
    }
    int32_t ret = SendCommandLock(cmdCheck, "+CLCK", 0, ppResponse);
    if (ret != 0 || (ppResponse != NULL && !(*ppResponse)->success)) {
        TELEPHONY_LOGE("AT+CLCK failed");
        return VENDOR_FAIL;
    }

    result = GenerateCommand(cmdActive, MAX_CMD_LENGTH, "AT+CRSM=%d,%d,%d,%d,%d,\"%s\",\"%s\"", pSim->command,
        pSim->fileid, pSim->p1, pSim->p2, pSim->p3, ((pSim->data == NULL) ? "" : (pSim->data)),
        (pSim->pathid == NULL) ? "" : (pSim->pathid));
    if (result <= 0) {
        TELEPHONY_LOGE("GenerateCommand is failed");
        return VENDOR_FAIL;
    }
    ret = SendCommandLock(cmdActive, "+CRSM", 0, ppResponse);
    if (ret != 0 || (ppResponse != NULL && !(*ppResponse)->success)) {
        TELEPHONY_LOGE("send failed ");
        return VENDOR_FAIL;
    }

    result = GenerateCommand(cmdClose, MAX_CMD_LENGTH, "AT+CLCK=\"%s\",%d,\"%s\"", "FD", 0, pSim->pin2);
    if (result <= 0) {
        TELEPHONY_LOGE("GenerateCommand is failed");
        return VENDOR_FAIL;
    }
    ret = SendCommandLock(cmdClose, "+CLCK", 0, ppResponse);
    if (ret != 0 || (ppResponse != NULL && !(*ppResponse)->success)) {
        TELEPHONY_LOGE("AT+CLCK failed");
        return VENDOR_FAIL;
    }
    TELEPHONY_LOGE("HRIL_ERR_SUCCESS return");
    return (int32_t)HRIL_ERR_SUCCESS;
}

static void HandlerSimIOResult(ResponseInfo *pResponse, HRilSimIOResponse *simResponse,
    const ReqDataInfo *requestInfo, char *pLine, int32_t *ret)
{
    if (pResponse == NULL) {
        TELEPHONY_LOGE("pResponse is NULL");
        return;
    }
    struct ReportInfo reportInfo = {0};

    if (*ret != HRIL_ERR_SUCCESS) {
        if (pResponse && pResponse->result) {
            pLine = pResponse->result;
            SkipATPrefix(&pLine);
            NextInt(&pLine, ret);
        } else {
            *ret = HRIL_ERR_GENERIC_FAILURE;
        }
    }
    reportInfo = CreateReportInfo(requestInfo, *ret, HRIL_RESPONSE, 0);
    if (simResponse == NULL) {
        TELEPHONY_LOGE("simResponse is NULL");
        OnSimReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
    } else {
        OnSimReport(HRIL_SIM_SLOT_1, reportInfo, (const uint8_t *)simResponse, sizeof(HRilSimIOResponse));
    }

    FreeResponseInfo(pResponse);
}

void ReqGetSimIO(const ReqDataInfo *requestInfo, const HRilSimIO *data, size_t dataLen)
{
    char *pLine = NULL;
    int32_t ret;
    const int32_t FILEID = 0x6F3B;
    HRilSimIO *pSim = (HRilSimIO *)data;
    HRilSimIOResponse simResponse = {0};
    ResponseInfo *pResponse = NULL;
    struct ReportInfo reportInfo = {};

    if (pSim->pin2 != NULL && pSim->fileid == FILEID) {
        ret = ReqGetSimIOFDN(pSim, &pResponse, dataLen);
        TELEPHONY_LOGE("ReqGetSimIOFDN call over");
        if (ret != HRIL_ERR_SUCCESS || !pResponse->success) {
            TELEPHONY_LOGE("FDN is failed");
            HandlerSimIOResult(pResponse, NULL, requestInfo, pLine, &ret);
            return;
        } else {
            TELEPHONY_LOGE("FDN is success");
            HandlerSimIOResult(pResponse, &simResponse, requestInfo, pLine, &ret);
            return;
        }
    }
    char cmd[MAX_CMD_LENGTH] = {0};
    int32_t result = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CRSM=%d,%d,%d,%d,%d,\"%s\",\"%s\"", pSim->command,
        pSim->fileid, pSim->p1, pSim->p2, pSim->p3, ((pSim->data == NULL) ? "" : (pSim->data)), pSim->pathid);
    if (result <= 0) {
        TELEPHONY_LOGE("GenerateCommand is failed");
    }
    ret = SendCommandLock(cmd, "+CRSM", 0, &pResponse);
    if (ret != HRIL_ERR_SUCCESS || (pResponse != NULL && !pResponse->success)) {
        TELEPHONY_LOGE("send failed  dataLen:%{public}zu, ret:%{public}d", dataLen, ret);
        HandlerSimIOResult(pResponse, NULL, requestInfo, pLine, &ret);
        return;
    }
    if (pResponse->head) {
        pLine = pResponse->head->data;
    }
    ret = ParseSimResponseResult(pLine, &simResponse);
    if (ret != HRIL_ERR_SUCCESS) {
        HandlerSimIOResult(pResponse, NULL, requestInfo, pLine, &ret);
        return;
    }
    reportInfo = CreateReportInfo(requestInfo, ret, HRIL_RESPONSE, 0);
    OnSimReport(HRIL_SIM_SLOT_1, reportInfo, (const uint8_t *)&simResponse, sizeof(HRilSimIOResponse));
    FreeResponseInfo(pResponse);
}

static void HandlerSimImsiResult(
    ResponseInfo *pResponse, struct ReportInfo reportInfo, const ReqDataInfo *requestInfo, char *pLine, int32_t *ret)
{
    if (pResponse == NULL) {
        TELEPHONY_LOGE("pResponse is NULL");
        return;
    }
    if (pResponse && pResponse->result) {
        pLine = pResponse->result;
        SkipATPrefix(&pLine);
        NextInt(&pLine, ret);
    } else {
        *ret = HRIL_ERR_GENERIC_FAILURE;
    }
    reportInfo = CreateReportInfo(requestInfo, *ret, HRIL_RESPONSE, 0);
    OnSimReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqGetSimImsi(const ReqDataInfo *requestInfo)
{
    ResponseInfo *pResponse = NULL;
    int32_t ret;
    char *pLine = NULL;
    struct ReportInfo reportInfo = {0};
    char *result = NULL;
    ret = SendCommandLock("AT+CIMI", NULL, 0, &pResponse);
    if (ret != 0 || (pResponse != NULL && !pResponse->success)) {
        TELEPHONY_LOGE("AT+CIMI send failed");
        HandlerSimImsiResult(pResponse, reportInfo, requestInfo, pLine, &ret);
        return;
    }
    if (pResponse->head) {
        result = pResponse->head->data;
    } else {
        HandlerSimImsiResult(pResponse, reportInfo, requestInfo, pLine, &ret);
        return;
    }
    reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    OnSimReport(HRIL_SIM_SLOT_1, reportInfo, (const uint8_t *)result, 0);
    FreeResponseInfo(pResponse);
}

static void HandlerSimLockStatusResult(
    ResponseInfo *pResponse, struct ReportInfo reportInfo, const ReqDataInfo *requestInfo, char *pLine, int32_t *ret)
{
    if (pResponse == NULL) {
        TELEPHONY_LOGE("pResponse is NULL");
        return;
    }
    if (pResponse && pResponse->result) {
        pLine = pResponse->result;
        SkipATPrefix(&pLine);
        NextInt(&pLine, ret);
    } else {
        *ret = HRIL_ERR_GENERIC_FAILURE;
    }
    reportInfo = CreateReportInfo(requestInfo, *ret, HRIL_RESPONSE, 0);
    OnSimReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqGetSimLockStatus(const ReqDataInfo *requestInfo, const HRilSimClock *data, size_t dataLen)
{
    ResponseInfo *pResponse = NULL;
    char cmd[MAX_CMD_LENGTH] = {0};
    int32_t ret;
    char *pLine = NULL;
    int32_t status = 0;
    HRilSimClock *pSimClck = NULL;
    struct ReportInfo reportInfo = {0};

    pSimClck = (HRilSimClock *)data;
    int32_t result = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CLCK=\"%s\",%d", pSimClck->fac, pSimClck->mode);
    if (result <= 0) {
        TELEPHONY_LOGE("GenerateCommand is failed");
    }
    ret = SendCommandLock(cmd, "+CLCK", 0, &pResponse);
    if (ret != 0 || (pResponse != NULL && !pResponse->success)) {
        TELEPHONY_LOGE("AT+CLCK send failed  dataLen:%{public}zu", dataLen);
        HandlerSimLockStatusResult(pResponse, reportInfo, requestInfo, pLine, &ret);
        return;
    }
    if (pResponse->head) {
        pLine = pResponse->head->data;
    }
    ret = SkipATPrefix(&pLine);
    if (ret != 0) {
        HandlerSimLockStatusResult(pResponse, reportInfo, requestInfo, pLine, &ret);
        return;
    }

    ret = NextInt(&pLine, &status);
    if (ret != 0) {
        HandlerSimLockStatusResult(pResponse, reportInfo, requestInfo, pLine, &ret);
        return;
    }
    reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    OnSimReport(HRIL_SIM_SLOT_1, reportInfo, (const uint8_t *)&status, sizeof(int32_t));
    FreeResponseInfo(pResponse);
}

void ReqSetSimLock(const ReqDataInfo *requestInfo, const HRilSimClock *data, size_t dataLen)
{
    ResponseInfo *pResponse = NULL;
    char cmd[MAX_CMD_LENGTH] = {0};
    char *pLine = NULL;
    int32_t ret;
    HRilSimClock *pSimClck = NULL;
    struct ReportInfo reportInfo = {0};

    pSimClck = (HRilSimClock *)data;
    int32_t result = GenerateCommand(
        cmd, MAX_CMD_LENGTH, "AT+CLCK=\"%s\",%d,\"%s\"", pSimClck->fac, pSimClck->mode, pSimClck->passwd);
    if (result <= 0) {
        TELEPHONY_LOGE("GenerateCommand is failed");
    }
    ret = SendCommandLock(cmd, "+CLCK", 0, &pResponse);
    if (ret != 0 || (pResponse != NULL && !pResponse->success)) {
        TELEPHONY_LOGE("AT+CLCK send failed  dataLen:%{public}zu", dataLen);
        if (pResponse && pResponse->result) {
            pLine = pResponse->result;
            SkipATPrefix(&pLine);
            NextInt(&pLine, &ret);
        } else {
            ret = HRIL_ERR_GENERIC_FAILURE;
        }
        reportInfo = CreateReportInfo(requestInfo, ret, HRIL_RESPONSE, 0);
        OnSimReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        FreeResponseInfo(pResponse);
        return;
    }
    reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    OnSimReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqChangeSimPassword(const ReqDataInfo *requestInfo, const HRilSimPassword *data, size_t dataLen)
{
    ResponseInfo *pResponse = NULL;
    char cmd[MAX_CMD_LENGTH] = {0};
    char *pLine = NULL;
    HRilSimPassword *pSimPassword = NULL;
    int32_t ret;
    struct ReportInfo reportInfo = {0};

    pSimPassword = (HRilSimPassword *)data;
    int32_t result = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CPWD=\"%s\",\"%s\",\"%s\"", pSimPassword->fac,
        pSimPassword->oldPassword, pSimPassword->newPassword);
    if (result <= 0) {
        TELEPHONY_LOGE("GenerateCommand is failed");
    }
    ret = SendCommandLock(cmd, "+CPWD", 0, &pResponse);
    if (ret != 0 || (pResponse != NULL && !pResponse->success)) {
        TELEPHONY_LOGE("AT+CPWD send failed  dataLen:%{public}zu", dataLen);
        if (pResponse && pResponse->result) {
            pLine = pResponse->result;
            SkipATPrefix(&pLine);
            NextInt(&pLine, &ret);
        } else {
            ret = HRIL_ERR_GENERIC_FAILURE;
        }
        reportInfo = CreateReportInfo(requestInfo, ret, HRIL_RESPONSE, 0);
        OnSimReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        FreeResponseInfo(pResponse);
        return;
    }

    reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    OnSimReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqUnlockPin(const ReqDataInfo *requestInfo, const char *pin)
{
    ResponseInfo *pResponse = NULL;
    char cmd[MAX_CMD_LENGTH] = {0};
    int32_t ret;
    char *pLine = NULL;
    struct ReportInfo reportInfo = {0};
    int32_t result = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CPIN=\"%s\"", pin);
    if (result <= 0) {
        TELEPHONY_LOGE("GenerateCommand is failed");
    }
    ret = SendCommandLock(cmd, "+CPIN", 0, &pResponse);
    if (ret != 0 || (pResponse != NULL && !pResponse->success)) {
        TELEPHONY_LOGE("AT+CPIN send failed");
        if (pResponse && pResponse->result) {
            pLine = pResponse->result;
            SkipATPrefix(&pLine);
            NextInt(&pLine, &ret);
        } else {
            ret = HRIL_ERR_GENERIC_FAILURE;
        }
        reportInfo = CreateReportInfo(requestInfo, ret, HRIL_RESPONSE, 0);
        OnSimReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        FreeResponseInfo(pResponse);
        return;
    }

    reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    OnSimReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqUnlockPuk(const ReqDataInfo *requestInfo, const char *puk, const char *pin)
{
    ResponseInfo *pResponse = NULL;
    char cmd[MAX_CMD_LENGTH] = {0};
    char *pLine = NULL;
    int32_t ret;
    struct ReportInfo reportInfo = {0};
    int32_t result = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CPIN=\"%s\",\"%s\"", puk, pin);
    if (result <= 0) {
        TELEPHONY_LOGE("GenerateCommand is failed");
    }
    ret = SendCommandLock(cmd, "+CPIN", 0, &pResponse);
    if (ret != 0 || (pResponse != NULL && !pResponse->success)) {
        TELEPHONY_LOGE("AT+CPIN send failed");
        if (pResponse && pResponse->result) {
            pLine = pResponse->result;
            SkipATPrefix(&pLine);
            NextInt(&pLine, &ret);
        } else {
            ret = HRIL_ERR_GENERIC_FAILURE;
        }
        reportInfo = CreateReportInfo(requestInfo, ret, HRIL_RESPONSE, 0);
        OnSimReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        FreeResponseInfo(pResponse);
        return;
    }
    reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    OnSimReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqGetSimPinInputTimes(const ReqDataInfo *requestInfo)
{
    int32_t ret;
    char *pLine = NULL;
    HRilPinInputTimes pinInputTimes = {0};
    ResponseInfo *pResponse = NULL;
    struct ReportInfo reportInfo = {0};

    ret = SendCommandLock("AT^CPIN?", "^CPIN", 0, &pResponse);
    if (ret != 0 || (pResponse != NULL && !pResponse->success)) {
        TELEPHONY_LOGE("AT^CPIN? send failed");
        if (pResponse && pResponse->result) {
            pLine = pResponse->result;
            SkipATPrefix(&pLine);
            NextInt(&pLine, &ret);
        } else {
            ret = HRIL_ERR_GENERIC_FAILURE;
        }
        reportInfo = CreateReportInfo(requestInfo, ret, HRIL_RESPONSE, 0);
        OnSimReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        FreeResponseInfo(pResponse);
        return;
    }
    if (pResponse->head) {
        pLine = pResponse->head->data;
    }
    ret = ParseSimPinInputTimesResult(pLine, &pinInputTimes);
    if (ret != 0) {
        if (pResponse && pResponse->result) {
            pLine = pResponse->result;
            SkipATPrefix(&pLine);
            NextInt(&pLine, &ret);
        } else {
            ret = HRIL_ERR_GENERIC_FAILURE;
        }
        reportInfo = CreateReportInfo(requestInfo, ret, HRIL_RESPONSE, 0);
        OnSimReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        FreeResponseInfo(pResponse);
        return;
    }
    reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    OnSimReport(HRIL_SIM_SLOT_1, reportInfo, (const uint8_t *)&pinInputTimes, sizeof(pinInputTimes));
    FreeResponseInfo(pResponse);
}

void ReqUnlockPin2(const ReqDataInfo *requestInfo, const char *pin2)
{
    ResponseInfo *pResponse = NULL;
    char cmd[MAX_CMD_LENGTH] = {0};
    int32_t ret;
    char *pLine = NULL;
    struct ReportInfo reportInfo = {0};

    int32_t result = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT^CPIN2=\"%s\"", pin2);
    if (result <= 0) {
        TELEPHONY_LOGE("GenerateCommand is failed");
    }
    ret = SendCommandLock(cmd, "^CPIN2", 0, &pResponse);
    if (ret != 0 || (pResponse != NULL && !pResponse->success)) {
        TELEPHONY_LOGE("AT^CPIN2 send failed");
        if (pResponse && pResponse->result) {
            pLine = pResponse->result;
            SkipATPrefix(&pLine);
            NextInt(&pLine, &ret);
        } else {
            ret = HRIL_ERR_GENERIC_FAILURE;
        }
        reportInfo = CreateReportInfo(requestInfo, ret, HRIL_RESPONSE, 0);
        OnSimReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        FreeResponseInfo(pResponse);
        return;
    }
    reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    OnSimReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqUnlockPuk2(const ReqDataInfo *requestInfo, const char *puk2, const char *pin2)
{
    ResponseInfo *pResponse = NULL;
    char cmd[MAX_CMD_LENGTH] = {0};
    char *pLine = NULL;
    int32_t ret;
    struct ReportInfo reportInfo = {0};

    int32_t result = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT^CPIN2=\"%s\",\"%s\"", puk2, pin2);
    if (result <= 0) {
        TELEPHONY_LOGE("GenerateCommand is failed");
    }
    ret = SendCommandLock(cmd, "^CPIN2", 0, &pResponse);
    if (ret != 0 || (pResponse != NULL && !pResponse->success)) {
        TELEPHONY_LOGE("AT^CPIN2 send failed");
        if (pResponse && pResponse->result) {
            pLine = pResponse->result;
            SkipATPrefix(&pLine);
            NextInt(&pLine, &ret);
        } else {
            ret = HRIL_ERR_GENERIC_FAILURE;
        }
        reportInfo = CreateReportInfo(requestInfo, ret, HRIL_RESPONSE, 0);
        OnSimReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        FreeResponseInfo(pResponse);
        return;
    }
    reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    OnSimReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqGetSimPin2InputTimes(const ReqDataInfo *requestInfo)
{
    int32_t ret;
    char *pLine = NULL;
    HRilPinInputTimes pin2InputTimes = {0};
    ResponseInfo *pResponse = NULL;
    struct ReportInfo reportInfo = {0};
    ret = SendCommandLock("AT^CPIN2?", "^CPIN2", 0, &pResponse);
    if (ret != 0 || (pResponse != NULL && !pResponse->success)) {
        TELEPHONY_LOGE("AT^CPIN2? send failed");
    }
    if (pResponse->head) {
        pLine = pResponse->head->data;
    }
    ret = ParseSimPinInputTimesResult(pLine, &pin2InputTimes);
    if (ret != 0) {
        if (pResponse && pResponse->result) {
            pLine = pResponse->result;
            SkipATPrefix(&pLine);
            NextInt(&pLine, &ret);
        } else {
            ret = HRIL_ERR_GENERIC_FAILURE;
        }
        reportInfo = CreateReportInfo(requestInfo, ret, HRIL_RESPONSE, 0);
        OnSimReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        FreeResponseInfo(pResponse);
        return;
    }
    reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    OnSimReport(HRIL_SIM_SLOT_1, reportInfo, (const uint8_t *)&pin2InputTimes, 0);
    FreeResponseInfo(pResponse);
}

void ReqSetActiveSim(const ReqDataInfo *requestInfo, int32_t index, int32_t enable)
{
    char cmd[MAX_CMD_LENGTH] = {0};
    char *pLine = NULL;
    int32_t ret;
    ResponseInfo *pResponse = NULL;
    struct ReportInfo reportInfo = {0};

    int32_t result = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT^HVSST=%d,%d", index, enable);
    if (result <= 0) {
        TELEPHONY_LOGE("GenerateCommand is failed");
    }
    ret = SendCommandLock(cmd, "^HVSST", 0, &pResponse);
    if (ret != 0 || (pResponse != NULL && !pResponse->success)) {
        TELEPHONY_LOGE("AT^HVSST send failed");
        if (pResponse && pResponse->result) {
            pLine = pResponse->result;
            SkipATPrefix(&pLine);
            NextInt(&pLine, &ret);
        } else {
            ret = HRIL_ERR_GENERIC_FAILURE;
        }
        reportInfo = CreateReportInfo(requestInfo, ret, HRIL_RESPONSE, 0);
        OnSimReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        FreeResponseInfo(pResponse);
        return;
    }
    reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    OnSimReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}
