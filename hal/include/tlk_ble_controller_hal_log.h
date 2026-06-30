#ifndef TLK_BLE_HAL_LOG_H
#define TLK_BLE_HAL_LOG_H

#ifdef __ZEPHYR__

#include <zephyr/logging/log.h>

#define TLK_BLE_LOG_MODULE_REGISTER(name, level) LOG_MODULE_REGISTER(name, level)

#define TLK_BLE_LOG_ERR(...)       LOG_ERR(__VA_ARGS__)
#define TLK_BLE_LOG_WRN(...)       LOG_WRN(__VA_ARGS__)
#define TLK_BLE_LOG_INF(...)       LOG_INF(__VA_ARGS__)
#define TLK_BLE_LOG_DBG(...)       LOG_DBG(__VA_ARGS__)

#define TLK_BLE_LOG_ERR_ISR(msg)   LOG_ERR("%s", msg)
#define TLK_BLE_LOG_WRN_ISR(msg)   LOG_WRN("%s", msg)
#define TLK_BLE_LOG_INF_ISR(msg)   LOG_INF("%s", msg)
#define TLK_BLE_LOG_DBG_ISR(msg)   LOG_DBG("%s", msg)

#define TLK_BLE_LOG_HEXDUMP_DBG(data, len, str) LOG_HEXDUMP_DBG(data, len, str)

#else

#include "system/debug/log/tlk_log.h"

#define TLK_BLE_LOG_MODULE_REGISTER(name, level) \
    TLK_LOG_CREATE(s_tlk_ble_log, #name)

#define TLK_BLE_LOG_ERR(...)       TLK_LOG_ERROR(s_tlk_ble_log, __VA_ARGS__)
#define TLK_BLE_LOG_WRN(...)       TLK_LOG_WARN(s_tlk_ble_log, __VA_ARGS__)
#define TLK_BLE_LOG_INF(...)       TLK_LOG_INFO(s_tlk_ble_log, __VA_ARGS__)
#define TLK_BLE_LOG_DBG(...)       TLK_LOG_DEBUG(s_tlk_ble_log, __VA_ARGS__)

#define TLK_BLE_LOG_ERR_ISR(msg)   TLK_LOG_ERROR_ISR(s_tlk_ble_log, msg)
#define TLK_BLE_LOG_WRN_ISR(msg)   TLK_LOG_WARN_ISR(s_tlk_ble_log, msg)
#define TLK_BLE_LOG_INF_ISR(msg)   TLK_LOG_INFO_ISR(s_tlk_ble_log, msg)
#define TLK_BLE_LOG_DBG_ISR(msg)   TLK_LOG_DEBUG_ISR(s_tlk_ble_log, msg)

#define TLK_BLE_LOG_HEXDUMP_DBG(data, len, str)

#endif

#endif