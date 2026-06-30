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

#define TLK_BLE_HCI_FLAG_EVENT_PHYTEST_2_WIRE_UART TLK_BIT(23)
#define TLK_BLE_HCI_FLAG_EVENT_TLK_MODULE          TLK_BIT(24)
#define TLK_BLE_HCI_FLAG_EVENT_BT_STD              TLK_BIT(25)
#define TLK_BLE_HCI_FLAG_EVENT_STACK               TLK_BIT(26)
#define TLK_BLE_HCI_FLAG_ACL_BT_STD                TLK_BIT(27)
#define TLK_BLE_HCI_FLAG_BT_HCI_CMD                TLK_BIT(28)
#define TLK_BLE_HCI_FLAG_ISO_DATE_STD              TLK_BIT(29)
#define TLK_BLE_HCI_FLAG_CIS_DATA                  TLK_BIT(30)

/**
 * @brief      HCI event handler
 * @param[in]  h - HCI Event type
 * @param[in]  *para - the pointer of HCI event parameter
 * @param[in]  n - the length of HCI event parameter
 * @return     0
 */
typedef int (*tlk_ble_controller_hci_event_handler_t)(uint32_t h, uint8_t *para, int n);

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
tlk_ble_controller_status_t tlk_ble_controller_hci_initTxFifo(uint8_t *pTxbuf, int fifo_size, int fifo_number);

/**
 * @brief      for user to initialize HCI RX FIFO.
 * @param[in]  pRxbuf - RX FIFO buffer address.
 * @param[in]  fifo_size - RX FIFO size
 * @param[in]  fifo_number - RX FIFO number, can only be 4, 8, 16 or 32
 * @return     status, 0x00:  succeed
 *                     other: failed
 */
tlk_ble_controller_status_t tlk_ble_controller_hci_initRxFifo(uint8_t *pRxbuf, int fifo_size, int fifo_number);

/**
 * @brief      for user to initialize HCI RX ACL Data FIFO.
 * @param[in]  pRxbuf - RX FIFO buffer address (Attention: buffer size = fifo_size*fifo_num*conn_max_num).
 * @param[in]  fifo_size - RX FIFO size
 * @param[in]  fifo_number - RX FIFO number, can only be 4, 8, 16 or 32
 * @return     status, 0x00:  succeed
 *                     other: failed
 */
tlk_ble_controller_status_t tlk_ble_controller_hci_initAclDataFifo(uint8_t *pAclbuf, int fifo_size, int fifo_number);


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

#endif /* _TLK_BLE_CONTROLLER_HCI_H_ */
