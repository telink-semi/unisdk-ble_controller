/********************************************************************************************************
 * @file    hci.h
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
#ifndef _TLK_BLE_CONTROLLER_HCI_H_
#define _TLK_BLE_CONTROLLER_HCI_H_

#include "tlk_ble_controller_common.h"
#ifdef __cplusplus
extern "C" {
#endif

/* ---------------------------------------------------------------------------
 * H4 header defines (from controller_v2 hci/inc/tlk_ble_ll_hci_internal.h)
 * -------------------------------------------------------------------------*/
#define TLK_BLE_LL_HCI_H4_HEADER_COMMAND        0x01
#define TLK_BLE_LL_HCI_H4_HEADER_ACL_DATA        0x02
#define TLK_BLE_LL_HCI_H4_HEADER_EVENT        0x04
#define TLK_BLE_LL_HCI_H4_HEADER_ISO_DATA        0x05

/* ---------------------------------------------------------------------------
 * ACL PB flag enum (from controller_v2 hci/inc/tlk_ble_ll_hci_internal.h)
 * -------------------------------------------------------------------------*/
enum tlk_ble_ll_hci_acl_pb_flag {
    TLK_BLE_LL_HCI_ACL_PB_FLAG_START_H_TO_C = 0x00,
    TLK_BLE_LL_HCI_ACL_PB_FLAG_CONTINUE = 0x01,
    TLK_BLE_LL_HCI_ACL_PB_FLAG_START_C_TO_H = 0x02,
};

#define TLK_BLE_HCI_FLAG_EVENT_PHYTEST_2_WIRE_UART TLK_BIT(23)
#define TLK_BLE_HCI_FLAG_EVENT_TLK_MODULE          TLK_BIT(24)
#define TLK_BLE_HCI_FLAG_EVENT_BT_STD              TLK_BIT(25)
#define TLK_BLE_HCI_FLAG_EVENT_STACK               TLK_BIT(26)
#define TLK_BLE_HCI_FLAG_ACL_BT_STD                TLK_BIT(27)
#define TLK_BLE_HCI_FLAG_BT_HCI_CMD                TLK_BIT(28)
#define TLK_BLE_HCI_FLAG_ISO_DATE_STD              TLK_BIT(29)
#define TLK_BLE_HCI_FLAG_CIS_DATA                  TLK_BIT(30)


/* ---------------------------------------------------------------------------
 * Core HCI data format structs (from controller_v2 tlk_ble_hci.h)
 * -------------------------------------------------------------------------*/

/** HCI ACL data packet format */
struct tlk_ble_hci_acl_data_format {
    uint16_t conn_handle : 12;
    uint16_t pb_flag : 2;
    uint16_t bc_flag : 2;
    uint16_t data_total_len;
    uint8_t data[0];
} __attribute__((packed));

/** HCI command packet format */
struct tlk_ble_hci_command_format {
    union {
        struct {
            uint16_t ocf : 10;
            uint16_t ogf : 6;
        };
        uint16_t opcode;
    };
    uint8_t param_len;
    uint8_t parameters[0];
} __attribute__((packed));

/** HCI event packet format */
struct tlk_ble_ll_hci_event_format {
    uint8_t event_code;
    uint8_t param_len;
    uint8_t parameter[0];
} __attribute__((packed));

/** HCI ISO data packet format */
struct tlk_ble_ll_hci_iso_data_format {
    uint16_t conn_handle : 12;
    uint16_t pb_flag : 2;
    uint16_t ts_flag : 1;
    uint16_t rfu : 1;
    uint16_t data_total_len : 14;
    uint16_t rfu2 : 2;
    uint8_t data[0];
} __attribute__((packed));

/** HCI Command Complete event format */
struct tlk_ble_hci_command_complete_format {
    uint8_t num_hci_command_packets;
    uint16_t command_opcode;
    uint8_t return_parameters[0];
} __attribute__((packed));

/** HCI Command Complete event format with default status field */
struct tlk_ble_hci_command_complete_default_format {
    uint8_t num_hci_command_packets;
    uint16_t command_opcode;
    uint8_t status;
    uint8_t return_parameters[0];
} __attribute__((packed));

/** HCI Command Status event format */
struct tlk_ble_hci_command_status_format {
    uint8_t status;
    uint8_t num_hci_command_packets;
    uint16_t command_opcode;
} __attribute__((packed));

/* ---------------------------------------------------------------------------
 * Handler typedefs (from controller_v2 tlk_ble_hci.h)
 * -------------------------------------------------------------------------*/

/** HCI command handler function type */
typedef void (*tlk_ble_ll_hci_command_deal_handler)(uint16_t opcode, const uint8_t *p_param);

/** Vendor-specific LE command handler function type */
typedef void (*tlk_ble_ll_hci_vendor_le_deal_handler)(uint16_t opcode, const uint8_t *p_param);

/** HCI ACL data handler callback type */
typedef void (*tlk_ble_hci_acl_data_handler)(const struct tlk_ble_hci_acl_data_format *p_acl_data);

/** HCI ISO data handler callback type */
typedef void (*tlk_ble_hci_iso_data_handler)(const struct tlk_ble_ll_hci_iso_data_format *p_iso_data);

/* ---------------------------------------------------------------------------
 * Function declarations – already implemented in v1
 * (originally declared in tlk_ble_controller_hci_internal.h)
 * -------------------------------------------------------------------------*/

/**
 * @brief   Send an HCI event to the Host.
 * @param[in] event_code HCI event code.
 * @param[in] p_buf Pointer to event data buffer.
 * @param[in] len Length of event data.
 */
void tlk_ble_ll_hci_send_event(uint8_t event_code, const void *p_buf, uint16_t len);

/**
 * @brief   Send an LE Meta event to the Host.
 * @param[in] p_buf Pointer to event data buffer.
 * @param[in] len Length of event data.
 */
void tlk_ble_ll_hci_send_le_meta_event(const void *p_buf, uint16_t len);

/**
 * @brief   Send ACL data to the Host.
 * @param[in] conn_handle Connection handle.
 * @param[in] start Start of packet flag.
 * @param[in] data_length Length of data.
 * @param[in] p_data Pointer to data buffer.
 */
void tlk_ble_ll_hci_send_acl_data(uint16_t conn_handle, bool start,
                                  uint16_t data_length, const void *p_data);

/**
 * @brief   Send a Command Complete event.
 * @param[in] opcode HCI command opcode.
 * @param[in] status Command status.
 * @param[in] p_param Pointer to return parameters.
 * @param[in] param_len Length of return parameters.
 */
void tlk_ble_ll_hci_send_command_complete_event(uint16_t opcode, uint8_t status,
                                                const void *p_param, uint8_t param_len);

/**
 * @brief   Send a Command Complete event with no parameters.
 * @param[in] opcode HCI command opcode.
 * @param[in] status Command status.
 */
void tlk_ble_ll_hci_send_command_complete_event_no_param(uint16_t opcode, uint8_t status);

/**
 * @brief   Send a Command Status event.
 * @param[in] opcode HCI command opcode.
 * @param[in] status Command status.
 */
void tlk_ble_ll_hci_send_command_status_event(uint16_t opcode, uint8_t status);

/**
 * @brief   Register an ACL data handler callback.
 * @param[in] handler Callback function pointer.
 */
void tlk_ble_hci_register_acl_data_handler(const tlk_ble_hci_acl_data_handler handler);

/**
 * @brief   Process all HCI commands.
 * @param[in] p_hci_command Pointer to the HCI command format structure.
 * @return  true if the command was handled, false otherwise.
 */
bool tlk_ble_hci_all_command_handler(const struct tlk_ble_hci_command_format *p_hci_command);

/* ---------------------------------------------------------------------------
 * Function declarations – new wrappers implemented in tlk_ble_controller_hci_v2_adapter.c
 * -------------------------------------------------------------------------*/

/**
 * @brief   Send an HCI packet (H4 framed) to the Host.
 *          First byte is H4 header type, rest is payload.
 * @param[in] p_buf Pointer to the H4 framed packet.
 * @param[in] len Total length including H4 header.
 */
void tlk_ble_ll_hci_send_packet(const void *p_buf, uint16_t len);

/**
 * @brief   Send ISO data to the Host.
 * @param[in] conn_handle Connection handle.
 * @param[in] ts_flag Timestamp flag.
 * @param[in] time_stamp Timestamp value (if ts_flag is true).
 * @param[in] data_length Length of ISO data.
 * @param[in] p_data Pointer to ISO data buffer.
 */
void tlk_ble_ll_hci_send_iso_data(uint16_t conn_handle, bool ts_flag,
                                  uint32_t time_stamp, uint16_t data_length,
                                  const void *p_data);

/**
 * @brief   Send an Unknown HCI Command event
 *          (Command Status with error code Unknown HCI Command).
 * @param[in] opcode HCI command opcode.
 * @param[in] p_param Pointer to command parameters (unused).
 */
void tlk_ble_ll_hci_send_unkown_hci_command(uint16_t opcode, const uint8_t *p_param);

/**
 * @brief   Register an ISO data handler callback.
 * @param[in] handler Callback function pointer.
 */
void tlk_ble_hci_register_iso_data_handler(const tlk_ble_hci_iso_data_handler handler);

/**
 * @brief   Set the HCI event mask.
 * @param[in] event_mask 64-bit event mask.
 */
void tlk_ble_ll_hci_set_event_mask(uint64_t event_mask);

/**
 * @brief   Check if an event is enabled in the event mask.
 * @param[in] event_mask 64-bit event mask to check.
 * @return  true if the event is enabled, false otherwise.
 */
bool tlk_ble_ll_hci_check_event_mask(uint64_t event_mask);

/**
 * @brief   Set the HCI event page 2 mask.
 * @param[in] event_mask 64-bit event page 2 mask.
 */
void tlk_ble_ll_hci_set_event_page_2_mask(uint64_t event_mask);

/**
 * @brief   Check if an event is enabled in the event page 2 mask.
 * @param[in] event_mask 64-bit event page 2 mask to check.
 * @return  true if the event is enabled, false otherwise.
 */
bool tlk_ble_ll_hci_check_event_page_2_mask(uint64_t event_mask);

/**
 * @brief   Set the LE event mask.
 * @param[in] le_event_mask 64-bit LE event mask.
 */
void tlk_ble_ll_hci_set_le_event_mask(uint64_t le_event_mask);

/**
 * @brief   Check if a LE event is enabled in the LE event mask.
 * @param[in] event_mask 64-bit LE event mask to check.
 * @return  true if the event is enabled, false otherwise.
 */
bool tlk_ble_ll_hci_check_le_event_mask(uint64_t event_mask);

/**
 * @brief   Get the LMP features value.
 * @return  64-bit LMP features bitmask.
 */
uint64_t tlk_ble_hci_get_lmp_features(void);

/**
 * @brief   Get the supported commands bitmap.
 * @return  Pointer to a 64-byte supported commands array.
 */
const uint8_t *tlk_ble_hci_get_supported_commands(void);

/**
 * @brief   Initialize HCI module (weak/default implementation).
 */
void tlk_ble_ll_hci_init(void);

/**
 * @brief      HCI event handler
 * @param[in]  h - HCI Event type
 * @param[in]  *para - the pointer of HCI event parameter
 * @param[in]  n - the length of HCI event parameter
 * @return     0
 */
typedef int (*tlk_ble_controller_hci_event_handler_t)(uint32_t h, uint8_t *para, uint32_t n);

/**
 * @brief      Controller data handler
 * @param[in]  conn - connection handle
 * @param[in]  *p - the pointer of HCI data
 * @return     0
 */
typedef int (*tlk_ble_controller_hci_data_handler_t)(uint16_t conn, uint8_t *p);

/**
 * @brief      for user to initialize HCI TX FIFO.
 * @param[in]  pRxbuf - TX FIFO buffer address.
 * @param[in]  fifo_size - RX FIFO size
 * @param[in]  fifo_number - RX FIFO number, can only be 4, 8, 16 or 32
 * @return     status, 0x00:  succeed
 *                     other: failed
 */
tlk_ble_controller_status_t tlk_ble_controller_hci_initTxFifo(uint8_t *pTxbuf, uint32_t fifo_size, uint8_t fifo_number);

/**
 * @brief      for user to initialize HCI RX FIFO.
 * @param[in]  pRxbuf - RX FIFO buffer address.
 * @param[in]  fifo_size - RX FIFO size
 * @param[in]  fifo_number - RX FIFO number, can only be 4, 8, 16 or 32
 * @return     status, 0x00:  succeed
 *                     other: failed
 */
tlk_ble_controller_status_t tlk_ble_controller_hci_initRxFifo(uint8_t *pRxbuf, uint32_t fifo_size, uint8_t fifo_number);

/**
 * @brief      for user to initialize HCI RX ACL Data FIFO.
 * @param[in]  pRxbuf - RX FIFO buffer address (Attention: buffer size = fifo_size*fifo_num*conn_max_num).
 * @param[in]  fifo_size - RX FIFO size
 * @param[in]  fifo_number - RX FIFO number, can only be 4, 8, 16 or 32
 * @return     status, 0x00:  succeed
 *                     other: failed
 */
tlk_ble_controller_status_t tlk_ble_controller_hci_initAclDataFifo(uint8_t *pAclbuf, uint32_t fifo_size, uint8_t fifo_number);


/**
 * @brief      this function is used to process HCI data.
 * @param[in]  *p - the pointer of HCI data packet.
 * @param[in]  n - the length of HCI data packet.
 * @return     0
 */
int tlk_ble_controller_hci_handler(uint8_t *p, int n);

/**
 * @brief      This function is used to register the controller event processing callback
 * @param[in]  handler - tlk_ble_controller_hci_event_handler_t
 * @return     none.
 */
void tlk_ble_controller_hci_registerEventHandler(tlk_ble_controller_hci_event_handler_t handler);


/**
 * @brief      This function is used to register ACL data transmission to Host for processing callback function.
 * @param[in]  handler - tlk_ble_controller_hci_data_handler_t
 * @return     none.
 */
void tlk_ble_controller_hci_registerDataHandler(tlk_ble_controller_hci_data_handler_t handle);

/**
 * @brief      this function is used to send ACL data to HOST
 * @param[in]  handle - connect handle
 * @param[in]  *p - the pointer of l2cap data
 * @return     0
 */
int tlk_ble_controller_hci_sendACLData(uint16_t handle, uint8_t *p);

#ifdef __cplusplus
}
#endif

#endif /* _TLK_BLE_CONTROLLER_HCI_H_ */
