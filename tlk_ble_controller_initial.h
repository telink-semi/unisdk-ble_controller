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
#include "tlk_ble_controller_common.h"

/********************* ACL connection LinkLayer TX & RX data FIFO allocation, Begin *******************************/

/**
 * @brief   ACL RX buffer size & number
 *          ACL RX buffer is shared by all connections to hold LinkLayer RF RX data.
 * usage limitation for TLK_BLE_LL_ACL_RX_FIFO_SIZE:
 * 1. must use TLK_BLE_HAL_CAL_ACL_RX_FIFO_SIZE to calculate, user can not change !!!
 *
 * usage limitation for CONFIG_TLK_BLE_CONTROLLER_ACL_RX_PACKET_COUNT:
 * 1. must be: 2^n, (power of 2)
 * 2. at least 4; recommended value: 4, 8, 16
 */
#if TLK_IS_ENABLED(CONFIG_TLK_BLE_CONTROLLER_ACL_CENTRAL_ENABLE) || TLK_IS_ENABLED(CONFIG_TLK_BLE_CONTROLLER_ACL_PERIPHERAL_ENABLE)
#define TLK_BLE_LL_ACL_RX_FIFO_SIZE TLK_BLE_CONTROLLER_LL_CAL_ACL_RX_FIFO_SIZE(CONFIG_TLK_BLE_CONTROLLER_ACL_MAX_RX_OCTETS) //user can not change !!!                //user set value
#endif

/**
 * @brief   ACL TX buffer size & number
 *          ACL Central TX buffer is shared by all central connections to hold LinkLayer RF TX data.
*           ACL Peripheral TX buffer is shared by all peripheral connections to hold LinkLayer RF TX data.
 * usage limitation for ACL_xxx_TX_FIFO_SIZE:
 * 1. must use TLK_BLE_HAL_CAL_LL_ACL_TX_FIFO_SIZE to calculate, user can not change !!!
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
#if TLK_IS_ENABLED(CONFIG_TLK_BLE_CONTROLLER_ACL_CENTRAL_ENABLE)
#define TLK_BLE_ACL_CENTRAL_TX_FIFO_SIZE TLK_BLE_CONTROLLER_LL_CAL_ACL_TX_FIFO_SIZE(CONFIG_TLK_BLE_CONTROLLER_ACL_CENTRAL_MAX_TX_OCTETS) //user can not change !!!         //user set value
#endif

#if TLK_IS_ENABLED(CONFIG_TLK_BLE_CONTROLLER_ACL_PERIPHERAL_ENABLE)
#define TLK_BLE_ACL_PERIPHERAL_TX_FIFO_SIZE TLK_BLE_CONTROLLER_LL_CAL_ACL_TX_FIFO_SIZE(CONFIG_TLK_BLE_CONTROLLER_ACL_PERIPHERAL_MAX_TX_OCTETS) //user can not change !!!          //user set value
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

/**
 * @brief      for user to initialize MCU
 * @param      none
 * @return     none
 */
void tlk_ble_ll_initBasicMCU(void);

/**
 * @brief      for user to initialize link layer Standby state
 * @param      none
 * @return     none
 */
void tlk_ble_ll_initStandby_module(void);

/**
 * @brief      for user to initialize legacy advertising module
 *             notice that only one module can be selected between legacy advertising module and extended advertising module
 * @param      none
 * @return     none
 */
void tlk_ble_ll_initLegacyAdvertising_module(void);

/**
 * @brief   Initialize the legacy (non-extended) scanning module (aligned with controller_v2).
 *          Wraps the existing legacy scanning initialization routine.
 * @param   none
 * @return  none
 */
void tlk_ble_ll_legacy_scan_initial(void);

/**
 * @brief      for user to initialize legacy initiating module
 *             notice that only one module can be selected between legacy initiating module and extended initiating module
 * @param      none
 * @return     none
 */
void tlk_ble_ll_initLegacyInitiating_module(void);

/**
 * @brief      for user to initialize ACL connection peripheral role.
 * @param      none
 * @return     none
 */
void tlk_ble_ll_acl_peripheral_initial(void);
/**
 * @brief      for user to initialize ACL connection central role.
 * @param      none
 * @return     none
 */
void tlk_ble_ll_acl_central_initial(void);

/**
 * @brief      for user to initialize ACL connection module, this is must if user want use ACL master role or ACL slave role.
 * @param      none
 * @return     none
 */
void tlk_ble_ll_initAclConnection_module(void);

/**
 * @brief      This function is used to configure the number of master and slave connections that the protocol stack can support.
 * @param[in]  max_master_num - Number of master ACL connections supported.
 * @param[in]  max_slave_num - Number of slave ACL connections supported.
 * @return     status, 0x00:  succeed
 *                     other: failed
 */
tlk_ble_controller_status_t tlk_ble_ll_setMaxConnectionNumber(uint8_t max_master_num, uint8_t max_slave_num);

/**
 * @brief      for user to initialize LinkLayer ACL connection RX FIFO.
 *             all connection will share the FIFO.
 * @param[in]  pRxbuf - RX FIFO buffer address.
 * @param[in]  fifo_size - RX FIFO size
 * @param[in]  fifo_number - RX FIFO number, can only be 4, 8, 16 or 32
 * @return     status, 0x00:  succeed
 *                     other: failed
 */
tlk_ble_controller_status_t tlk_ble_ll_initAclConnRxFifo(uint8_t *pRxbuf, uint32_t fifo_size, uint8_t fifo_number);


/**
 * @brief      for user to initialize LinkLayer ACL connection central role TX FIFO.
 * @param[in]  pRxbuf - TX FIFO buffer address.
 * @param[in]  fifo_size - TX FIFO size
 * @param[in]  fifo_number - TX FIFO number, can only be 4, 8, 16 or 32
 * @param[in]  tlk_ble_conn_number - Number of supported ACL central connections
 * @return     status, 0x00:  succeed
 *                     other: failed
 */
tlk_ble_controller_status_t tlk_ble_ll_initAclCentralTxFifo(uint8_t *pTxbuf, uint32_t fifo_size, uint8_t fifo_number, uint8_t tlk_ble_conn_number);

/**
 * @brief      set ACL data length for Master and Slave.
 * @param[in]  maxRxOct - ACL max RX Oct.
 * @param[in]  maxTxOct_master - ACL master max TX Oct (Range:27 - 251)
 * @param[in]  maxTxOct_slave - ACL slave max TX Oct  (Range:27 - 251)
 * @return     status, 0x00 :  succeed
 *                     other: failed
 */
tlk_ble_controller_status_t tlk_ble_ll_setAclConnMaxOctetsNumber(uint8_t maxRxOct, uint8_t maxTxOct_master, uint8_t maxTxOct_slave);

/**
 * @brief      for user to initialize LinkLayer ACL connection peripheral role TX FIFO.
 * @param[in]  pRxbuf - TX FIFO buffer address.
 * @param[in]  fifo_size - TX FIFO size
 * @param[in]  fifo_number - TX FIFO number, can only be 4, 8, 16 or 32
 * @param[in]  tlk_ble_conn_number - Number of supported ACL peripheral connections
 * @return     status, 0x00:  succeed
 *                     other: failed
 */
tlk_ble_controller_status_t tlk_ble_ll_initAclPeripheralTxFifo(uint8_t *pTxbuf, uint32_t fifo_size, uint8_t fifo_number, uint8_t tlk_ble_conn_number);

/**
 * @brief      this function is used check if any controller buffer initialized by application incorrect.
 *             attention: this function must be called at the end of BLE LinkLayer Initialization.
 * @param      none
 * @return     status, 0x00:  succeed, no buffer error
 *                     other: buffer error code
 */
tlk_ble_controller_status_t tlk_ble_blc_contr_checkControllerInitialization(void);
#ifdef __cplusplus
}
#endif