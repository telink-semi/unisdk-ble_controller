/********************************************************************************************************
 * @file    tlk_ble_initial.c
 *
 * @brief   BLE stack initialization module implementation.
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

#include "tlk_ble_controller_initial.h"
#include "tlk_ble_controller_common.h"
#include "tlk_ble_controller_hci.h"
#include "core/include/tlk_stimer.h"
#include <string.h>
#include "hal/include/tlk_ble_controller_hal_log.h"

TLK_BLE_LOG_MODULE_REGISTER(ble_ctrl_init, CONFIG_BT_TLX_LOG_LEVEL);

/********************* ACL connection LinkLayer TX & RX data FIFO allocation, Begin *******************************/

#if TLK_IS_ENABLED(CONFIG_TLK_BLE_CONTROLLER_ACL_CENTRAL_ENABLE) || TLK_IS_ENABLED(CONFIG_TLK_BLE_CONTROLLER_ACL_PERIPHERAL_ENABLE)
/**
 * @brief   ACL RX buffer, shared by all connections to hold LinkLayer RF RX data.
 *          user should define and initialize this buffer if either ACL Central or ACL Peripheral is used.
 */
uint8_t tlk_ble_ll_acl_rx_fifo[TLK_BLE_LL_ACL_RX_FIFO_SIZE * CONFIG_TLK_BLE_CONTROLLER_ACL_RX_PACKET_COUNT] = {0};
#endif

#if TLK_IS_ENABLED(CONFIG_TLK_BLE_CONTROLLER_ACL_CENTRAL_ENABLE)
/**
 * @brief   ACL Central TX buffer, shared by all central connections to hold LinkLayer RF TX data.
 *          ACL Central TX buffer should be defined only when ACL connection central role is used.
 */
uint8_t tlk_ble_ll_acl_cen_tx_fifo[TLK_BLE_ACL_CENTRAL_TX_FIFO_SIZE * CONFIG_TLK_BLE_CONTROLLER_ACL_CENTRAL_TX_PACKET_COUNT * CONFIG_TLK_BLE_CONTROLLER_ACL_CENTRAL_MAX_NUM];
#endif

#if TLK_IS_ENABLED(CONFIG_TLK_BLE_CONTROLLER_ACL_PERIPHERAL_ENABLE)
/**
 * @brief   ACL Peripheral TX buffer, shared by all peripheral connections to hold LinkLayer RF TX data.
 *          ACL Peripheral TX buffer should be defined only when ACL connection peripheral role is used.
 */
uint8_t tlk_ble_ll_acl_per_tx_fifo[TLK_BLE_ACL_PERIPHERAL_TX_FIFO_SIZE * CONFIG_TLK_BLE_CONTROLLER_ACL_PERIPHERAL_TX_PACKET_COUNT * CONFIG_TLK_BLE_CONTROLLER_ACL_PERIPHERAL_MAX_NUM];
#endif
/******************** ACL connection LinkLayer TX & RX data FIFO allocation, End ***********************************/

/********************* HCI FIFO allocation, Begin *******************************/

/**
 * @brief   HCI TX FIFO buffer for host-to-controller HCI commands and data.
 */
static uint8_t tlk_ble_ll_hci_tx_fifo[TLK_BLE_HCI_ACL_IN_FIFO_SIZE(CONFIG_TLK_BLE_CONTROLLER_HCI_TX_DATA_SIZE) * CONFIG_TLK_BLE_CONTROLLER_HCI_TX_BUFFER_NUMBER];

/**
 * @brief   HCI RX FIFO buffer for controller-to-host HCI events.
 */
static uint8_t tlk_ble_ll_hci_rx_fifo[TLK_BLE_HCI_ACL_IN_FIFO_SIZE(CONFIG_TLK_BLE_CONTROLLER_HCI_RX_DATA_SIZE) * CONFIG_TLK_BLE_CONTROLLER_HCI_RX_BUFFER_NUMBER];

/**
 * @brief   HCI ACL data FIFO buffer for controller-to-host ACL data packets.
 */
static uint8_t tlk_ble_ll_hci_acl_fifo[TLK_BLE_CALCULATE_HCI_ACL_DATA_FIFO_SIZE(CONFIG_TLK_BLE_CONTROLLER_HCI_ACL_DATA_SIZE) * CONFIG_TLK_BLE_CONTROLLER_HCI_ACL_BUFFER_NUMBER];
/******************** HCI FIFO allocation, End ***********************************/

/**
 * @brief   Initialize the BLE stack with configuration from Kconfig.
 */
int tlk_ble_controller_initial(void)
{
    TLK_BLE_LOG_INF("BLE controller initializing");
    //////////// LinkLayer Initialization  Begin /////////////////////////
    tlk_ble_ll_initBasicMCU();

    tlk_ble_ll_initStandby_module();

#if TLK_IS_ENABLED(CONFIG_TLK_BLE_CONTROLLER_LEGACY_ADV_ENABLE)
    tlk_ble_ll_initLegacyAdvertising_module();
#endif
#if TLK_IS_ENABLED(CONFIG_TLK_BLE_CONTROLLER_LEGACY_SCAN_ENABLE)
    tlk_ble_ll_legacy_scan_initial();
#if TLK_IS_ENABLED(CONFIG_TLK_BLE_CONTROLLER_ACL_CENTRAL_ENABLE)
    tlk_ble_ll_initLegacyInitiating_module();
#endif
#endif

#if TLK_IS_ENABLED(CONFIG_TLK_BLE_CONTROLLER_ACL_CENTRAL_ENABLE) || TLK_IS_ENABLED(CONFIG_TLK_BLE_CONTROLLER_ACL_PERIPHERAL_ENABLE)
    tlk_ble_ll_initAclConnection_module();
    #if TLK_IS_ENABLED(CONFIG_TLK_BLE_CONTROLLER_ACL_CENTRAL_ENABLE)
    tlk_ble_ll_acl_central_initial();
    #endif

    #if TLK_IS_ENABLED(CONFIG_TLK_BLE_CONTROLLER_ACL_PERIPHERAL_ENABLE)
    tlk_ble_ll_acl_peripheral_initial();
    #endif
    tlk_ble_ll_setMaxConnectionNumber(CONFIG_TLK_BLE_CONTROLLER_ACL_CENTRAL_MAX_NUM, CONFIG_TLK_BLE_CONTROLLER_ACL_PERIPHERAL_MAX_NUM);
    tlk_ble_ll_setAclConnMaxOctetsNumber(CONFIG_TLK_BLE_CONTROLLER_ACL_MAX_RX_OCTETS, CONFIG_TLK_BLE_CONTROLLER_ACL_CENTRAL_MAX_TX_OCTETS, CONFIG_TLK_BLE_CONTROLLER_ACL_PERIPHERAL_MAX_TX_OCTETS);
    /* all ACL connection share same RX FIFO */
    tlk_ble_ll_initAclConnRxFifo(tlk_ble_ll_acl_rx_fifo, TLK_BLE_LL_ACL_RX_FIFO_SIZE, CONFIG_TLK_BLE_CONTROLLER_ACL_RX_PACKET_COUNT);
    #if CONFIG_TLK_BLE_CONTROLLER_ACL_CENTRAL_MAX_NUM
    /* ACL Central TX FIFO */
    tlk_ble_ll_initAclCentralTxFifo(tlk_ble_ll_acl_cen_tx_fifo, TLK_BLE_ACL_CENTRAL_TX_FIFO_SIZE, CONFIG_TLK_BLE_CONTROLLER_ACL_CENTRAL_TX_PACKET_COUNT, CONFIG_TLK_BLE_CONTROLLER_ACL_CENTRAL_MAX_NUM);
    #endif
    #if CONFIG_TLK_BLE_CONTROLLER_ACL_PERIPHERAL_MAX_NUM
    /* ACL Peripheral TX FIFO */
    tlk_ble_ll_initAclPeripheralTxFifo(tlk_ble_ll_acl_per_tx_fifo, TLK_BLE_ACL_PERIPHERAL_TX_FIFO_SIZE, CONFIG_TLK_BLE_CONTROLLER_ACL_PERIPHERAL_TX_PACKET_COUNT, CONFIG_TLK_BLE_CONTROLLER_ACL_PERIPHERAL_MAX_NUM);
    #endif
#endif

    //////////// LinkLayer Initialization  End /////////////////////////

    //////////// HCI FIFO Initialization  Begin /////////////////////////
    tlk_ble_controller_hci_initTxFifo(tlk_ble_ll_hci_tx_fifo,
                              TLK_BLE_HCI_ACL_IN_FIFO_SIZE(CONFIG_TLK_BLE_CONTROLLER_HCI_TX_DATA_SIZE),
                              CONFIG_TLK_BLE_CONTROLLER_HCI_TX_BUFFER_NUMBER);
    tlk_ble_controller_hci_initRxFifo(tlk_ble_ll_hci_rx_fifo,
                              TLK_BLE_HCI_ACL_IN_FIFO_SIZE(CONFIG_TLK_BLE_CONTROLLER_HCI_RX_DATA_SIZE),
                              CONFIG_TLK_BLE_CONTROLLER_HCI_RX_BUFFER_NUMBER);
    tlk_ble_controller_hci_initAclDataFifo(tlk_ble_ll_hci_acl_fifo,
                                   TLK_BLE_CALCULATE_HCI_ACL_DATA_FIFO_SIZE(CONFIG_TLK_BLE_CONTROLLER_HCI_ACL_DATA_SIZE),
                                   CONFIG_TLK_BLE_CONTROLLER_HCI_ACL_BUFFER_NUMBER);
    //////////// HCI FIFO Initialization  End /////////////////////////

    tlk_ble_controller_status_t ret = tlk_ble_blc_contr_checkControllerInitialization();
    if(ret != TLK_BLE_SUCCESS){
        TLK_BLE_LOG_ERR("BLE controller check initialization failed, err: 0x%02x", ret);
        return -1;
    }

    TLK_BLE_LOG_INF("BLE controller initialized successfully");
    return 0;
}