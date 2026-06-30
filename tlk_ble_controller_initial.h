/********************************************************************************************************
 * @file    tlk_ble_initial.h
 *
 * @brief   BLE stack initialization module.
 *
 * @author  BLE GROUP
 * @date    2026-04-28
 *
 * @par     Copyright (c) 2026, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "tlk_ble_controller_ble.h"

/********************* ACL connection LinkLayer TX & RX data FIFO allocation, Begin *******************************/

/**
 * @brief   ACL RX buffer size & number
 *          ACL RX buffer is shared by all connections to hold LinkLayer RF RX data.
 * usage limitation for TLK_BLE_LL_ACL_RX_FIFO_SIZE:
 * 1. must use TLK_BLE_LL_CAL_ACL_RX_FIFO_SIZE to calculate, user can not change !!!
 *
 * usage limitation for CONFIG_TLK_BLE_CONTROLLER_ACL_RX_PACKET_COUNT:
 * 1. must be: 2^n, (power of 2)
 * 2. at least 4; recommended value: 4, 8, 16
 */
#define TLK_BLE_LL_ACL_RX_FIFO_SIZE TLK_BLE_LL_CAL_ACL_RX_FIFO_SIZE(CONFIG_TLK_BLE_CONTROLLER_ACL_MAX_RX_OCTETS) //user can not change !!!                //user set value


/**
 * @brief   ACL TX buffer size & number
 *          ACL Central TX buffer is shared by all central connections to hold LinkLayer RF TX data.
*           ACL Peripheral TX buffer is shared by all peripheral connections to hold LinkLayer RF TX data.
 * usage limitation for ACL_xxx_TX_FIFO_SIZE:
 * 1. must use CAL_LL_ACL_TX_FIFO_SIZE to calculate, user can not change !!!
 *
 * usage limitation for ACL_xxx_TX_FIFO_NUM:
 * 1. must be: (2^n) + 1, (power of 2, then add 1)
 * 2. for B91m IC: at least 9; recommended value: 9, 17, 33; other value not allowed.
 * 3. for B85m IC: at least 8; recommended value: 8, 16, 32; other value not allowed.
 *
 * only for B91: usage limitation for size * (number - 1)
 * 1. (ACL_xxx_TX_FIFO_SIZE * (ACL_xxx_TX_FIFO_NUM - 1)) must be less than 4096 (4K)
 *    so when ACL TX FIFO size equal to or bigger than 256, ACL TX FIFO number can only be 9(can not use 17 or 33), cause 256*(17-1)=4096
 */
#if defined(CONFIG_TLK_BLE_CONTROLLER_ACL_CENTRAL_ENABLE)
#define TLK_BLE_ACL_CENTRAL_TX_FIFO_SIZE CAL_LL_ACL_TX_FIFO_SIZE(CONFIG_TLK_BLE_CONTROLLER_ACL_CENTRAL_MAX_TX_OCTETS) //user can not change !!!         //user set value
#endif

#if defined(CONFIG_TLK_BLE_CONTROLLER_ACL_PERIPHERAL_ENABLE)
#define TLK_BLE_ACL_PERIPHERAL_TX_FIFO_SIZE CAL_LL_ACL_TX_FIFO_SIZE(CONFIG_TLK_BLE_CONTROLLER_ACL_PERIPHERAL_MAX_TX_OCTETS) //user can not change !!!          //user set value
#endif


extern uint8_t tlk_ble_ll_acl_rx_fifo[];
extern uint8_t tlk_ble_ll_acl_cen_tx_fifo[];
extern uint8_t tlk_ble_ll_acl_per_tx_fifo[];
/******************** ACL connection LinkLayer TX & RX data FIFO allocation, End ***********************************/

/**
 * @brief   Initialize the BLE stack with configuration from Kconfig.
 *          This function performs LinkLayer, HCI, Host (GAP, L2CAP, ATT, GATT, SMP) initialization.
 *          It assumes that the necessary buffers (tlk_ble_ll_acl_rx_fifo, tlk_ble_ll_acl_cen_tx_fifo, etc.)
 *          have been defined in the application.
 *
 * @param   None
 * @return  0 on success, negative error code on failure.
 */
int tlk_ble_controller_initial(void);

#ifdef __cplusplus
}
#endif