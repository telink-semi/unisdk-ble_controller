/********************************************************************************************************
 * @file    tlk_ble_common.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#ifndef TLK_BLE_COMMON_H
#define TLK_BLE_COMMON_H

typedef enum
{
    TLK_BLE_SUCCESS = 0,

    //// HCI Status, See the Core_v5.0(Vol 2/Part D/1.3 "list of Error Codes") for more information)
    TLK_BLE_HCI_ERR_UNKNOWN_HCI_CMD                       = 0x01,
    TLK_BLE_HCI_ERR_UNKNOWN_CONN_ID                       = 0x02,
    TLK_BLE_HCI_ERR_HW_FAILURE                            = 0x03,
    TLK_BLE_HCI_ERR_PAGE_TIMEOUT                          = 0x04,
    TLK_BLE_HCI_ERR_AUTH_FAILURE                          = 0x05,
    TLK_BLE_HCI_ERR_PIN_KEY_MISSING                       = 0x06,
    TLK_BLE_HCI_ERR_MEM_CAP_EXCEEDED                      = 0x07,
    TLK_BLE_HCI_ERR_CONN_TIMEOUT                          = 0x08,
    TLK_BLE_HCI_ERR_CONN_LIMIT_EXCEEDED                   = 0x09,
    TLK_BLE_HCI_ERR_SYNCH_CONN_LIMIT_EXCEEDED             = 0x0A,
    TLK_BLE_HCI_ERR_CONN_ALREADY_EXISTS                   = 0x0B,
    TLK_BLE_HCI_ERR_CMD_DISALLOWED                        = 0x0C,
    TLK_BLE_HCI_ERR_CONN_REJ_LIMITED_RESOURCES            = 0x0D,
    TLK_BLE_HCI_ERR_CONN_REJECTED_SECURITY_REASONS        = 0x0E,
    TLK_BLE_HCI_ERR_CONN_REJECTED_UNACCEPTABLE_BDADDR     = 0x0F,
    TLK_BLE_HCI_ERR_CONN_ACCEPT_TIMEOUT_EXCEEDED          = 0x10,
    TLK_BLE_HCI_ERR_UNSUPPORTED_FEATURE_PARAM_VALUE       = 0x11,
    TLK_BLE_HCI_ERR_INVALID_HCI_CMD_PARAMS                = 0x12,
    TLK_BLE_HCI_ERR_REMOTE_USER_TERM_CONN                 = 0x13,
    TLK_BLE_HCI_ERR_REMOTE_DEVICE_TERM_CONN_LOW_RESOURCES = 0x14,
    TLK_BLE_HCI_ERR_REMOTE_DEVICE_TERM_CONN_POWER_OFF     = 0x15,
    TLK_BLE_HCI_ERR_CONN_TERM_BY_LOCAL_HOST               = 0x16,
    TLK_BLE_HCI_ERR_REPEATED_ATTEMPTS                     = 0x17,
    TLK_BLE_HCI_ERR_PAIRING_NOT_ALLOWED                   = 0x18,
    TLK_BLE_HCI_ERR_UNKNOWN_LMP_PDU                       = 0x19,
    TLK_BLE_HCI_ERR_UNSUPPORTED_REMOTE_FEATURE            = 0x1A,
    TLK_BLE_HCI_ERR_SCO_OFFSET_REJ                        = 0x1B,
    TLK_BLE_HCI_ERR_SCO_INTERVAL_REJ                      = 0x1C,
    TLK_BLE_HCI_ERR_SCO_AIR_MODE_REJ                      = 0x1D,
    TLK_BLE_HCI_ERR_INVALID_LMP_PARAMS                    = 0x1E,
    TLK_BLE_HCI_ERR_UNSPECIFIED_ERROR                     = 0x1F,
    TLK_BLE_HCI_ERR_UNSUPPORTED_LMP_PARAM_VAL             = 0x20,
    TLK_BLE_HCI_ERR_ROLE_CHANGE_NOT_ALLOWED               = 0x21,
    TLK_BLE_HCI_ERR_LMP_LL_RESP_TIMEOUT                   = 0x22,
    TLK_BLE_HCI_ERR_LMP_ERR_TRANSACTION_COLLISION         = 0x23,
    TLK_BLE_HCI_ERR_LMP_PDU_NOT_ALLOWED                   = 0x24,
    TLK_BLE_HCI_ERR_ENCRYPT_MODE_NOT_ACCEPTABLE           = 0x25,
    TLK_BLE_HCI_ERR_LINK_KEY_CAN_NOT_BE_CHANGED           = 0x26,
    TLK_BLE_HCI_ERR_REQ_QOS_NOT_SUPPORTED                 = 0x27,
    TLK_BLE_HCI_ERR_INSTANT_PASSED                        = 0x28,
    TLK_BLE_HCI_ERR_PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED   = 0x29,
    TLK_BLE_HCI_ERR_DIFFERENT_TRANSACTION_COLLISION       = 0x2A,
    TLK_BLE_HCI_ERR_RESERVED1                             = 0x2B,
    TLK_BLE_HCI_ERR_QOS_UNACCEPTABLE_PARAM                = 0x2C,
    TLK_BLE_HCI_ERR_QOS_REJ                               = 0x2D,
    TLK_BLE_HCI_ERR_CHAN_ASSESSMENT_NOT_SUPPORTED         = 0x2E,
    TLK_BLE_HCI_ERR_INSUFFICIENT_SECURITY                 = 0x2F,
    TLK_BLE_HCI_ERR_PARAM_OUT_OF_MANDATORY_RANGE          = 0x30,
    TLK_BLE_HCI_ERR_RESERVED2                             = 0x31,
    TLK_BLE_HCI_ERR_ROLE_SWITCH_PENDING                   = 0x32,
    TLK_BLE_HCI_ERR_RESERVED3                             = 0x33,
    TLK_BLE_HCI_ERR_RESERVED_SLOT_VIOLATION               = 0x34,
    TLK_BLE_HCI_ERR_ROLE_SWITCH_FAILED                    = 0x35,
    TLK_BLE_HCI_ERR_EXTENDED_INQUIRY_RESP_TOO_LARGE       = 0x36,
    TLK_BLE_HCI_ERR_SIMPLE_PAIRING_NOT_SUPPORTED_BY_HOST  = 0x37,
    TLK_BLE_HCI_ERR_HOST_BUSY_PAIRING                     = 0x38,
    TLK_BLE_HCI_ERR_CONN_REJ_NO_SUITABLE_CHAN_FOUND       = 0x39,
    TLK_BLE_HCI_ERR_CONTROLLER_BUSY                       = 0x3A,
    TLK_BLE_HCI_ERR_UNACCEPTABLE_CONN_INTERVAL            = 0x3B,
    TLK_BLE_HCI_ERR_UNACCEPTABLE_CONN_PARAMETERS          = 0x3B,
    TLK_BLE_HCI_ERR_ADVERTISING_TIMEOUT                   = 0x3C,
    TLK_BLE_HCI_ERR_CONN_TERM_MIC_FAILURE                 = 0x3D,
    TLK_BLE_HCI_ERR_CONN_FAILED_TO_ESTABLISH              = 0x3E,
    TLK_BLE_HCI_ERR_MAC_CONN_FAILED                       = 0x3F,
    TLK_BLE_HCI_ERR_COARSE_CLOCK_ADJUSTMENT_REJECT        = 0x40,
    TLK_BLE_HCI_ERR_TYPE0_SUBMAP_NOT_DEFINED              = 0x41,
    TLK_BLE_HCI_ERR_UNKNOWN_ADV_IDENTIFIER                = 0x42,
    TLK_BLE_HCI_ERR_LIMIT_REACHED                         = 0x43,
    TLK_BLE_HCI_ERR_OP_CANCELLED_BY_HOST                  = 0x44,
    TLK_BLE_HCI_ERR_PACKET_TOO_LONG                       = 0x45,
    TLK_BLE_HCI_ERR_TOO_LATE                              = 0x46,
    TLK_BLE_HCI_ERR_TOO_EARLY                             = 0x47,
    TLK_BLE_HCI_ERR_INSUFFICIENT_CHANNELS                 = 0x48,


    ///////////////////////// TELINK define status /////////////////////////////

    //LL status
    TLK_BLE_LL_ERR_CONNECTION_NOT_ESTABLISH = 0x80,
    TLK_BLE_LL_ERR_TX_FIFO_NOT_ENOUGH,
    TLK_BLE_LL_ERR_ENCRYPTION_BUSY,
    TLK_BLE_LL_ERR_CURRENT_STATE_NOT_SUPPORTED_THIS_CMD,
    TLK_BLE_LL_ERR_INVALID_PARAMETER,
    TLK_BLE_LL_ERR_UNKNOWN_OPCODE,


    ///////// Initialization Error ///////////

    TLK_BLE_LL_ACL_RX_BUF_NO_INIT = 0x1001,
    TLK_BLE_LL_ACL_RX_BUF_PARAM_INVALID,
    TLK_BLE_LL_ACL_RX_BUF_SIZE_NOT_MEET_MAX_RX_OCT,
    TLK_BLE_LL_ACL_TX_BUF_NO_INIT,
    TLK_BLE_LL_ACL_TX_BUF_PARAM_INVALID,
    TLK_BLE_LL_ACL_TX_BUF_SIZE_NOT_MEET_MAX_TX_OCT,
    TLK_BLE_HCI_ACL_DATA_BUF_PARAM_INVALID,
    TLK_BLE_HCI_ACL_DATA_BUF_SIZE_NOT_MEET_MAX_TX_OCT,
    TLK_BLE_LL_ACL_CONN_NUM_NOT_SUPPORTED,
    
} tlk_ble_controller_status_t;

/////////////////////////////// BLE  MAC ADDRESS //////////////////////////////////////////////
#define TLK_BLE_ADDR_PUBLIC         0 //BDA: Public
#define TLK_BLE_ADDR_RANDOM         1 //BDA: Random
#define TLK_BLE_ADDR_PUBLIC_ID      2 //BDA: Public Identity Address, RPA
#define TLK_BLE_ADDR_RANDOM_ID      3 //BDA: Random (Static) Identity Address, RPA

#define TLK_BLE_ADDR_INVALID 0xff
#define TLK_BLE_ADDR_LEN     6


//Definition for BLE Common Address Type
/*
 *
 *                |--public  ..................................................... TLK_BLE_DEVICE_ADDRESS_PUBLIC
 *                |
 * Address Type --|           |-- random static  ................................. TLK_BLE_DEVICE_ADDRESS_RANDOM_STATIC
 *                |           |
 *                |--random --|
 *                            |                    |-- non_resolvable private  ... TLK_BLE_DEVICE_ADDRESS_NON_RESOLVABLE_PRIVATE
 *                            |-- random private --|
 *                                                 |-- resolvable private  ....... TLK_BLE_DEVICE_ADDRESS_RESOLVABLE_PRIVATE
 *
 */

#define TLK_BLE_DEVICE_ADDRESS_PUBLIC                 1
#define TLK_BLE_DEVICE_ADDRESS_RANDOM_STATIC          2
#define TLK_BLE_DEVICE_ADDRESS_NON_RESOLVABLE_PRIVATE 3
#define TLK_BLE_DEVICE_ADDRESS_RESOLVABLE_PRIVATE     4


#define TLK_BLE_IS_PUBLIC_ADDR(Type, Addr)                      ( (Type)==TLK_BLE_ADDR_PUBLIC)
#define TLK_BLE_IS_RANDOM_STATIC_ADDR(Type, Addr)          ((Type) == TLK_BLE_ADDR_RANDOM && (Addr[5] & 0xC0) == 0xC0)
#define TLK_BLE_IS_NON_RESOLVABLE_PRIVATE_ADDR(Type, Addr) ((Type) == TLK_BLE_ADDR_RANDOM && (Addr[5] & 0xC0) == 0x00)
#define TLK_BLE_IS_RESOLVABLE_PRIVATE_ADDR(Type, Addr)     ((Type) == TLK_BLE_ADDR_RANDOM && (Addr[5] & 0xC0) == 0x40)


#define TLK_BLE_MAC_MATCH16(md, ms)                        (md[0] == ms[0] && md[1] == ms[1] && md[2] == ms[2])

/**
 * @brief   HCI ACL DATA buffer length = LE_ACL_Data_Packet_Length + 4, tlk_ble_pkt_len is integer multiple of 4, so result is 4 Byte align
 *          4 = 2(connHandle) + 1(PBFlag) + 1(length)
 */
#define CALCULATE_HCI_ACL_DATA_FIFO_SIZE(tlk_ble_pkt_len) ((tlk_ble_pkt_len + 4 + 3) / 4 * 4)


/* HCI ACL in data format in telink  (host to controller)
 *
+++++
| 1     | 2       | 2         | n               |
+++++
| type  | handle  | data_len  | data_total_len  |
+++++
 */
#define TLK_BLE_HCI_ACL_IN_FIFO_SIZE(n) ((n + 5 + 3) / 4 * 4)

#endif
