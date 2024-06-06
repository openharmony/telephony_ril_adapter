/*
 * Copyright (C) 2024 Huawei Device Co., Ltd.
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

#ifndef OHOS_HRIL_EVENT_MAP_H
#define OHOS_HRIL_EVENT_MAP_H

#include "hril_notification.h"
#include "hril_request.h"

namespace OHOS {
namespace Telephony {
static std::unordered_map<int32_t, std::string> requestEventMap_ = {
    { HREQ_CALL_BASE, "HREQ_CALL_BASE" },
    { HREQ_CALL_GET_CALL_LIST, "HREQ_CALL_GET_CALL_LIST" },
    { HREQ_CALL_DIAL, "HREQ_CALL_DIAL" },
    { HREQ_CALL_HANGUP, "HREQ_CALL_HANGUP" },
    { HREQ_CALL_REJECT, "HREQ_CALL_REJECT" },
    { HREQ_CALL_ANSWER, "HREQ_CALL_ANSWER" },
    { HREQ_CALL_HOLD_CALL, "HREQ_CALL_HOLD_CALL" },
    { HREQ_CALL_UNHOLD_CALL, "HREQ_CALL_UNHOLD_CALL" },
    { HREQ_CALL_SWITCH_CALL, "HREQ_CALL_SWITCH_CALL" },
    { HREQ_CALL_COMBINE_CONFERENCE, "HREQ_CALL_COMBINE_CONFERENCE" },
    { HREQ_CALL_SEPARATE_CONFERENCE, "HREQ_CALL_SEPARATE_CONFERENCE" },
    { HREQ_CALL_CALL_SUPPLEMENT, "HREQ_CALL_CALL_SUPPLEMENT" },
    { HREQ_CALL_SEND_DTMF, "HREQ_CALL_SEND_DTMF" },
    { HREQ_CALL_START_DTMF, "HREQ_CALL_START_DTMF" },
    { HREQ_CALL_STOP_DTMF, "HREQ_CALL_STOP_DTMF" },
    { HREQ_CALL_SET_CLIP, "HREQ_CALL_SET_CLIP" },
    { HREQ_CALL_GET_CLIP, "HREQ_CALL_GET_CLIP" },
    { HREQ_CALL_GET_CALL_WAITING, "HREQ_CALL_GET_CALL_WAITING" },
    { HREQ_CALL_SET_CALL_WAITING, "HREQ_CALL_SET_CALL_WAITING" },
    { HREQ_CALL_GET_CALL_RESTRICTION, "HREQ_CALL_GET_CALL_RESTRICTION" },
    { HREQ_CALL_SET_CALL_RESTRICTION, "HREQ_CALL_SET_CALL_RESTRICTION" },
    { HREQ_CALL_GET_CALL_TRANSFER_INFO, "HREQ_CALL_GET_CALL_TRANSFER_INFO" },
    { HREQ_CALL_SET_CALL_TRANSFER_INFO, "HREQ_CALL_SET_CALL_TRANSFER_INFO" },
    { HREQ_CALL_GET_CLIR, "HREQ_CALL_GET_CLIR" },
    { HREQ_CALL_SET_CLIR, "HREQ_CALL_SET_CLIR" },
    { HREQ_CALL_GET_CALL_PREFERENCE, "HREQ_CALL_GET_CALL_PREFERENCE" },
    { HREQ_CALL_SET_CALL_PREFERENCE, "HREQ_CALL_SET_CALL_PREFERENCE" },
    { HREQ_CALL_SET_USSD, "HREQ_CALL_SET_USSD" },
    { HREQ_CALL_GET_USSD, "HREQ_CALL_GET_USSD" },
    { HREQ_CALL_SET_MUTE, "HREQ_CALL_SET_MUTE" },
    { HREQ_CALL_GET_MUTE, "HREQ_CALL_GET_MUTE" },
    { HREQ_CALL_GET_EMERGENCY_LIST, "HREQ_CALL_GET_EMERGENCY_LIST" },
    { HREQ_CALL_GET_FAIL_REASON, "HREQ_CALL_GET_FAIL_REASON" },
    { HREQ_CALL_SET_BARRING_PASSWORD, "HREQ_CALL_SET_BARRING_PASSWORD" },
    { HREQ_CALL_CLOSE_UNFINISHED_USSD, "HREQ_CALL_CLOSE_UNFINISHED_USSD" },
    { HREQ_EXPLICIT_CALL_TRANSFER_CONNECTION, "HREQ_EXPLICIT_CALL_TRANSFER_CONNECTION" },
    { HREQ_SET_VONR_SWITCH, "HREQ_SET_VONR_SWITCH" },
    { HREQ_CALL_SET_SUPP_SRV_NOTIFICATION, "HREQ_CALL_SET_SUPP_SRV_NOTIFICATION" },
    { HREQ_CALL_GET_TTY_MODE, "HREQ_CALL_GET_TTY_MODE" },
    { HREQ_CALL_SET_TTY_MODE, "HREQ_CALL_SET_TTY_MODE" },

    { HREQ_SMS_BASE, "HREQ_SMS_BASE" },
    { HREQ_SMS_SEND_GSM_SMS, "HREQ_SMS_SEND_GSM_SMS" },
    { HREQ_SMS_SEND_CDMA_SMS, "HREQ_SMS_SEND_CDMA_SMS" },
    { HREQ_SMS_ADD_SIM_MESSAGE, "HREQ_SMS_ADD_SIM_MESSAGE" },
    { HREQ_SMS_DEL_SIM_MESSAGE, "HREQ_SMS_DEL_SIM_MESSAGE" },
    { HREQ_SMS_UPDATE_SIM_MESSAGE, "HREQ_SMS_UPDATE_SIM_MESSAGE" },
    { HREQ_SMS_SEND_SMS_MORE_MODE, "HREQ_SMS_SEND_SMS_MORE_MODE" },
    { HREQ_SMS_SEND_SMS_ACK, "HREQ_SMS_SEND_SMS_ACK" },
    { HREQ_SMS_SET_SMSC_ADDR, "HREQ_SMS_SET_SMSC_ADDR" },
    { HREQ_SMS_GET_SMSC_ADDR, "HREQ_SMS_GET_SMSC_ADDR" },
    { HREQ_SMS_GET_CB_CONFIG, "HREQ_SMS_GET_CB_CONFIG" },
    { HREQ_SMS_GET_CDMA_CB_CONFIG, "HREQ_SMS_GET_CDMA_CB_CONFIG" },
    { HREQ_SMS_SET_CDMA_CB_CONFIG, "HREQ_SMS_SET_CDMA_CB_CONFIG" },
    { HREQ_SMS_ADD_CDMA_SIM_MESSAGE, "HREQ_SMS_ADD_CDMA_SIM_MESSAGE" },
    { HREQ_SMS_DEL_CDMA_SIM_MESSAGE, "HREQ_SMS_DEL_CDMA_SIM_MESSAGE" },
    { HREQ_SMS_UPDATE_CDMA_SIM_MESSAGE, "HREQ_SMS_UPDATE_CDMA_SIM_MESSAGE" },
    { HREQ_SMS_SET_MEMORY_STATUS, "HREQ_SMS_SET_MEMORY_STATUS" },

    { HREQ_SIM_BASE, "HREQ_SIM_BASE" },
    { HREQ_SIM_GET_IMSI, "HREQ_SIM_GET_IMSI" },
    { HREQ_SIM_GET_SIM_LOCK_STATUS, "HREQ_SIM_GET_SIM_LOCK_STATUS" },
    { HREQ_SIM_SET_SIM_LOCK, "HREQ_SIM_SET_SIM_LOCK" },
    { HREQ_SIM_CHANGE_SIM_PASSWORD, "HREQ_SIM_CHANGE_SIM_PASSWORD" },
    { HREQ_SIM_UNLOCK_PIN, "HREQ_SIM_UNLOCK_PIN" },
    { HREQ_SIM_UNLOCK_PUK, "HREQ_SIM_UNLOCK_PUK" },
    { HREQ_SIM_GET_SIM_PIN_INPUT_TIMES, "HREQ_SIM_GET_SIM_PIN_INPUT_TIMES" },
    { HREQ_SIM_UNLOCK_PIN2, "HREQ_SIM_UNLOCK_PIN2" },
    { HREQ_SIM_UNLOCK_PUK2, "HREQ_SIM_UNLOCK_PUK2" },
    { HREQ_SIM_GET_SIM_PIN2_INPUT_TIMES, "HREQ_SIM_GET_SIM_PIN2_INPUT_TIMES" },
    { HREQ_SIM_SET_ACTIVE_SIM, "HREQ_SIM_SET_ACTIVE_SIM" },
    { HREQ_SIM_SET_RADIO_PROTOCOL, "HREQ_SIM_SET_RADIO_PROTOCOL" },
    { HREQ_SIM_STK_SEND_TERMINAL_RESPONSE, "HREQ_SIM_STK_SEND_TERMINAL_RESPONSE" },
    { HREQ_SIM_STK_SEND_ENVELOPE, "HREQ_SIM_STK_SEND_ENVELOPE" },
    { HREQ_SIM_STK_SEND_ENVELOPE_RESPONSE_CONTAIN_STATUS, "HREQ_SIM_STK_SEND_ENVELOPE_RESPONSE_CONTAIN_STATUS" },
    { HREQ_SIM_STK_SEND_CALL_SETUP_REQUEST_RESULT, "HREQ_SIM_STK_SEND_CALL_SETUP_REQUEST_RESULT" },
    { HREQ_SIM_STK_IS_READY, "HREQ_SIM_STK_IS_READY" },
    { HREQ_SIM_OPEN_LOGICAL_CHANNEL, "HREQ_SIM_OPEN_LOGICAL_CHANNEL" },
    { HREQ_SIM_CLOSE_LOGICAL_CHANNEL, "HREQ_SIM_CLOSE_LOGICAL_CHANNEL" },
    { HREQ_SIM_TRANSMIT_APDU_LOGICAL_CHANNEL, "HREQ_SIM_TRANSMIT_APDU_LOGICAL_CHANNEL" },
    { HREQ_SIM_TRANSMIT_APDU_BASIC_CHANNEL, "HREQ_SIM_TRANSMIT_APDU_BASIC_CHANNEL" },
    { HREQ_SIM_AUTHENTICATION, "HREQ_SIM_AUTHENTICATION" },
    { HREQ_SIM_UNLOCK_SIM_LOCK, "HREQ_SIM_UNLOCK_SIM_LOCK" },
    { HREQ_SIM_SEND_NCFG_OPER_INFO, "HREQ_SIM_SEND_NCFG_OPER_INFO" },
    { HREQ_SIM_SET_SIM_POWER_STATE, "HREQ_SIM_SET_SIM_POWER_STATE" },

    { HREQ_DATA_BASE, "HREQ_DATA_BASE" },
    { HREQ_DATA_SET_INIT_APN_INFO, "HREQ_DATA_SET_INIT_APN_INFO" },
    { HREQ_DATA_DEACTIVATE_PDP_CONTEXT, "HREQ_DATA_DEACTIVATE_PDP_CONTEXT" },
    { HREQ_DATA_GET_PDP_CONTEXT_LIST, "HREQ_DATA_GET_PDP_CONTEXT_LIST" },
    { HREQ_DATA_GET_LINK_BANDWIDTH_INFO, "HREQ_DATA_GET_LINK_BANDWIDTH_INFO" },
    { HREQ_DATA_SET_LINK_BANDWIDTH_REPORTING_RULE, "HREQ_DATA_SET_LINK_BANDWIDTH_REPORTING_RULE" },
    { HREQ_DATA_SET_DATA_PROFILE_INFO, "HREQ_DATA_SET_DATA_PROFILE_INFO" },
    { HREQ_DATA_SEND_DATA_PERFORMANCE_MODE, "HREQ_DATA_SEND_DATA_PERFORMANCE_MODE" },
    { HREQ_DATA_SEND_DATA_SLEEP_MODE, "HREQ_DATA_SEND_DATA_SLEEP_MODE" },
    { HREQ_DATA_GET_LINK_CAPABILITY, "HREQ_DATA_GET_LINK_CAPABILITY" },
    { HREQ_DATA_CLEAN_ALL_CONNECTIONS, "HREQ_DATA_CLEAN_ALL_CONNECTIONS" },

    { HREQ_NETWORK_BASE, "HREQ_NETWORK_BASE" },
    { HREQ_NETWORK_GET_NETWORK_SEARCH_INFORMATION, "HREQ_NETWORK_GET_NETWORK_SEARCH_INFORMATION" },
    { HREQ_NETWORK_GET_NETWORK_SELECTION_MODE, "HREQ_NETWORK_GET_NETWORK_SELECTION_MODE" },
    { HREQ_NETWORK_SET_NETWORK_SELECTION_MODE, "HREQ_NETWORK_SET_NETWORK_SELECTION_MODE" },
    { HREQ_NETWORK_GET_NEIGHBORING_CELLINFO_LIST, "HREQ_NETWORK_GET_NEIGHBORING_CELLINFO_LIST" },
    { HREQ_NETWORK_SET_PREFERRED_NETWORK, "HREQ_NETWORK_SET_PREFERRED_NETWORK" },
    { HREQ_NETWORK_GET_PREFERRED_NETWORK, "HREQ_NETWORK_GET_PREFERRED_NETWORK" },
    { HREQ_NETWORK_GET_RADIO_CAPABILITY, "HREQ_NETWORK_GET_RADIO_CAPABILITY" },
    { HREQ_NETWORK_GET_PHYSICAL_CHANNEL_CONFIG, "HREQ_NETWORK_GET_PHYSICAL_CHANNEL_CONFIG" },
    { HREQ_NETWORK_SET_LOCATE_UPDATES, "HREQ_NETWORK_SET_LOCATE_UPDATES" },
    { HREQ_NETWORK_SET_NR_OPTION_MODE, "HREQ_NETWORK_SET_NR_OPTION_MODE" },
    { HREQ_NETWORK_GET_NR_OPTION_MODE, "HREQ_NETWORK_GET_NR_OPTION_MODE" },
    { HREQ_NETWORK_GET_NR_SSBID_INFO, "HREQ_NETWORK_GET_NR_SSBID_INFO" },

    { HREQ_COMMON_BASE, "HREQ_COMMON_BASE" },
    { HREQ_MODEM_SHUT_DOWN, "HREQ_MODEM_SHUT_DOWN" },
    { HREQ_MODEM_SET_RADIO_STATUS, "HREQ_MODEM_SET_RADIO_STATUS" },
    { HREQ_MODEM_GET_RADIO_STATUS, "HREQ_MODEM_GET_RADIO_STATUS" },
    { HREQ_MODEM_GET_IMEI, "HREQ_MODEM_GET_IMEI" },
    { HREQ_MODEM_GET_MEID, "HREQ_MODEM_GET_MEID" },
    { HREQ_MODEM_GET_BASEBAND_VERSION, "HREQ_MODEM_GET_BASEBAND_VERSION" },
    { HREQ_MODEM_GET_VOICE_RADIO, "HREQ_MODEM_GET_VOICE_RADIO" },

    { HREQ_MODEM_EXIT, "HREQ_MODEM_EXIT" },
};

static std::unordered_map<int32_t, std::string> notificationEventMap_ = {
    { HNOTI_CALL_STATE_UPDATED, "HNOTI_CALL_STATE_UPDATED" },
    { HNOTI_CALL_USSD_REPORT, "HNOTI_CALL_USSD_REPORT" },
    { HNOTI_CALL_RINGBACK_VOICE_REPORT, "HNOTI_CALL_RINGBACK_VOICE_REPORT" },
    { HNOTI_CALL_SRVCC_STATUS_REPORT, "HNOTI_CALL_SRVCC_STATUS_REPORT" },
    { HNOTI_CALL_EMERGENCY_NUMBER_REPORT, "HNOTI_CALL_EMERGENCY_NUMBER_REPORT" },
    { HNOTI_CALL_RSRVCC_STATUS_REPORT, "HNOTI_CALL_RSRVCC_STATUS_REPORT" },

    { HNOTI_SMS_NEW_SMS, "HNOTI_SMS_NEW_SMS" },
    { HNOTI_SMS_NEW_CDMA_SMS, "HNOTI_SMS_NEW_CDMA_SMS" },
    { HNOTI_SMS_STATUS_REPORT, "HNOTI_SMS_STATUS_REPORT" },
    { HNOTI_SMS_NEW_SMS_STORED_ON_SIM, "HNOTI_SMS_NEW_SMS_STORED_ON_SIM" },
    { HNOTI_CB_CONFIG_REPORT, "HNOTI_CB_CONFIG_REPORT" },

    { HNOTI_SIM_STATUS_CHANGED, "HNOTI_SIM_STATUS_CHANGED" },
    { HNOTI_SIM_STK_SESSION_END_NOTIFY, "HNOTI_SIM_STK_SESSION_END_NOTIFY" },
    { HNOTI_SIM_STK_PROACTIVE_NOTIFY, "HNOTI_SIM_STK_PROACTIVE_NOTIFY" },
    { HNOTI_SIM_STK_ALPHA_NOTIFY, "HNOTI_SIM_STK_ALPHA_NOTIFY" },
    { HNOTI_SIM_STK_EVENT_NOTIFY, "HNOTI_SIM_STK_EVENT_NOTIFY" },
    { HNOTI_SIM_STK_CALL_SETUP_NOTIFY, "HNOTI_SIM_STK_CALL_SETUP_NOTIFY" },

    { HNOTI_DATA_PDP_CONTEXT_LIST_UPDATED, "HNOTI_DATA_PDP_CONTEXT_LIST_UPDATED" },
    { HNOTI_DATA_LINK_CAPABILITY_UPDATED, "HNOTI_DATA_LINK_CAPABILITY_UPDATED" },

    { HNOTI_NETWORK_CS_REG_STATUS_UPDATED, "HNOTI_NETWORK_CS_REG_STATUS_UPDATED" },
    { HNOTI_NETWORK_SIGNAL_STRENGTH_UPDATED, "HNOTI_NETWORK_SIGNAL_STRENGTH_UPDATED" },
    { HNOTI_NETWORK_TIME_UPDATED, "HNOTI_NETWORK_TIME_UPDATED" },
    { HNOTI_NETWORK_TIME_ZONE_UPDATED, "HNOTI_NETWORK_TIME_ZONE_UPDATED" },
    { HNOTI_NETWORK_PS_REG_STATUS_UPDATED, "HNOTI_NETWORK_PS_REG_STATUS_UPDATED" },
    { HNOTI_NETWORK_PHY_CHNL_CFG_UPDATED, "HNOTI_NETWORK_PHY_CHNL_CFG_UPDATED" },
    { HNOTI_NETWORK_CURRENT_CELL_UPDATED, "HNOTI_NETWORK_CURRENT_CELL_UPDATED" },
    { HNOTI_NETWORK_RRC_CONNECTION_STATE_UPDATED, "HNOTI_NETWORK_RRC_CONNECTION_STATE_UPDATED" },

    { HNOTI_MODEM_RADIO_STATE_UPDATED, "HNOTI_MODEM_RADIO_STATE_UPDATED" },
    { HNOTI_MODEM_VOICE_TECH_UPDATED, "HNOTI_MODEM_VOICE_TECH_UPDATED" },
    { HNOTI_MODEM_DSDS_MODE_UPDATED, "HNOTI_MODEM_DSDS_MODE_UPDATED" },
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_HRIL_EVENT_MAP_H