/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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

    {HNOTI_CALL_STATE_UPDATED, NEED_LOCK},
    {HNOTI_CALL_USSD_REPORT, NEED_LOCK},
    {HNOTI_CALL_RINGBACK_VOICE_REPORT, NEED_LOCK},
    {HNOTI_CALL_SRVCC_STATUS_REPORT, NEED_LOCK},
    {HNOTI_CALL_EMERGENCY_NUMBER_REPORT, NEED_LOCK},
    {HNOTI_CALL_RSRVCC_STATUS_REPORT, NEED_LOCK},

    {HNOTI_SMS_NEW_SMS, NEED_LOCK},
    {HNOTI_SMS_NEW_CDMA_SMS, NEED_LOCK},
    {HNOTI_SMS_STATUS_REPORT, NEED_LOCK},
    {HNOTI_SMS_NEW_SMS_STORED_ON_SIM, NEED_LOCK},
    {HNOTI_CB_CONFIG_REPORT, NEED_LOCK},

    {HNOTI_SIM_STATUS_CHANGED, NEED_LOCK},
    {HNOTI_SIM_STK_SESSION_END_NOTIFY, NEED_LOCK},
    {HNOTI_SIM_STK_PROACTIVE_NOTIFY, NEED_LOCK},
    {HNOTI_SIM_STK_ALPHA_NOTIFY, NEED_LOCK},
    {HNOTI_SIM_STK_EVENT_NOTIFY, NEED_LOCK},
    {HNOTI_SIM_STK_CALL_SETUP_NOTIFY, NEED_LOCK},

    {HNOTI_DATA_PDP_CONTEXT_LIST_UPDATED, NEED_LOCK},
    {HNOTI_DATA_LINK_CAPABILITY_UPDATED, NEED_LOCK},

    {HNOTI_NETWORK_CS_REG_STATUS_UPDATED, NEED_LOCK},
    {HNOTI_NETWORK_SIGNAL_STRENGTH_UPDATED, UNNEED_LOCK},
    {HNOTI_NETWORK_TIME_UPDATED, NEED_LOCK},
    {HNOTI_NETWORK_TIME_ZONE_UPDATED, NEED_LOCK},
    {HNOTI_NETWORK_PS_REG_STATUS_UPDATED, NEED_LOCK},
    {HNOTI_NETWORK_PHY_CHNL_CFG_UPDATED, NEED_LOCK},
    {HNOTI_NETWORK_CURRENT_CELL_UPDATED, NEED_LOCK},
    {HNOTI_NETWORK_RRC_CONNECTION_STATE_UPDATED, UNNEED_LOCK},

    {HNOTI_MODEM_RADIO_STATE_UPDATED, NEED_LOCK},
    {HNOTI_MODEM_VOICE_TECH_UPDATED, NEED_LOCK},
    {HNOTI_MODEM_DSDS_MODE_UPDATED, NEED_LOCK},
