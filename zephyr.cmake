# SPDX-License-Identifier: Apache-2.0
#
# UniSDK Zephyr module — BLE controller build configuration
#
# This file is included from zephyr/CMakeLists.txt when CONFIG_BT_TLX
# is enabled.  It sets up include paths and source files for the Telink
# BLE controller stack, which runs on the same CPU as the Zephyr BT Host
# and communicates via internal HCI transport.
#
# The BLE controller is maintained as a separate subsystem and may be
# versioned independently of the core UniSDK.  Keeping its build logic
# in its own file makes the dependency explicit and simplifies swapping
# controller versions or implementations.
#
# Variables expected from the caller (set in zephyr/CMakeLists.txt):
#   UNISDK_BASE  - path to the unisdk root directory
#   UNISDK_GEN_DIR - path to the generated headers directory

# ============================================================================
# 1. Path variables
# ============================================================================
set(BLE_CTRL_BASE   ${UNISDK_BASE}/subsystem/ble/controller)
set(BLE_CTRL_STACK  ${BLE_CTRL_BASE}/stack)

# Map CONFIG_TLK_CORE to the BLE controller's driver/HAL directory name.
# The controller uses the bare core name (e.g. "TL721X", "TL321X").
set(BLE_CORE ${CONFIG_TLK_CORE})
string(TOLOWER ${BLE_CORE} BLE_CORE_LOWER)

# ============================================================================
# 2. Include paths
#
# The controller stack uses #include "stack/..." and
# #include "hal/..." relative to the controller/ root, so that
# directory must be on the path.  Additional SoC-specific paths
# are needed for the lower HAL and driver layers.
# ============================================================================
zephyr_include_directories(
    ${BLE_CTRL_BASE}                              # for "stack/*.h", "hal/*.h"
    ${BLE_CTRL_STACK}                             # for "tlk_ble_controller_common.h"
    ${BLE_CTRL_STACK}/hci                         # for "hci.h", "hci_const.h"
    ${BLE_CTRL_BASE}/hal/common                   # for "tlk_utility.h", "tlk_ble_controller_hal_driver.h"
    ${BLE_CTRL_BASE}/hal/hal_${BLE_CORE}          # for "tlk_ble_controller_hal_core.h"
    ${BLE_CTRL_BASE}/drivers/${BLE_CORE}          # for "lib/include/rf.h", "flash.h"
    ${BLE_CTRL_BASE}/drivers/${BLE_CORE}/lib/include  # for direct rf.h, ske/ includes
)

# ============================================================================
# 3. Source files and library linking
#
# Two modes:
#   Dev mode  (libraries/ does NOT exist): compile sources via zephyr_sources()
#   SDK mode  (libraries/     exists):     link prebuilt .a directly
# ============================================================================
if(NOT EXISTS "${UNISDK_BASE}/libraries")
    # --- Dev mode: compile sources ---

    # --- 3a. Controller core (Link Layer + HCI) ---
    #
    # We GLOB the controller/ and hci/ directories recursively,
    # deliberately excluding host/ and service/ to avoid symbol
    # clashes with the Zephyr BT Host stack.
    #
    # tlk_ble_initial.c is added explicitly - it provides the ACL
    # connection FIFO buffers and the tlk_ble_controller_initial()
    # entry point used by the HCI driver.
    #
    # ble.c provides tlk_mem_set / tlk_mem_cpy / tlk_ble_mem_cmp
    # used throughout the controller stack.
    file(GLOB_RECURSE BLE_CTRL_CORE_SRCS
        ${BLE_CTRL_STACK}/*.c
        ${BLE_CTRL_STACK}/hci/*.c
        ${BLE_CTRL_STACK}/os_sup/*.c
    )
    zephyr_sources(${BLE_CTRL_CORE_SRCS})
    zephyr_sources(
        ${BLE_CTRL_BASE}/tlk_ble_controller_initial.c
        ${BLE_CTRL_STACK}/tlk_ble_controller_ble.c
    )

    # --- 3b. HAL common sources ---
    set(BLE_CTRL_HAL_COMMON ${BLE_CTRL_BASE}/hal/common)
    zephyr_sources(
        ${BLE_CTRL_HAL_COMMON}/tlk_ble_controller_hal_rf.c
        ${BLE_CTRL_HAL_COMMON}/tlk_ble_controller_hal_driver.c
        ${BLE_CTRL_HAL_COMMON}/tlk_ble_controller_utility.c
        ${BLE_CTRL_HAL_COMMON}/tlk_ble_controller_hal_flash.c
    )

    # --- 3c. SoC-specific HAL sources ---
    set(BLE_CTRL_HAL_SOC ${BLE_CTRL_BASE}/hal/hal_${BLE_CORE})
    zephyr_sources(
        ${BLE_CTRL_HAL_SOC}/tlk_ble_controller_hal_${BLE_CORE_LOWER}.c
        ${BLE_CTRL_HAL_SOC}/ext_driver/driver_internal/ext_aes.c
        ${BLE_CTRL_HAL_SOC}/ext_driver/driver_internal/ext_rf.c
        ${BLE_CTRL_HAL_SOC}/ext_driver/driver_internal/ext_lib.c
    )

    # --- 3d. SoC-specific driver sources ---
    set(BLE_CTRL_DRV ${BLE_CTRL_BASE}/drivers/${BLE_CORE})
    zephyr_sources(
        ${BLE_CTRL_DRV}/flash.c
        ${BLE_CTRL_DRV}/stimer.c
    )

    # All lib/ sources (rf, ske, flash_base, crypto_common, etc.)
    file(GLOB_RECURSE BLE_CTRL_DRV_LIB_SRCS
        ${BLE_CTRL_DRV}/lib/src/*.c
    )
    zephyr_sources(${BLE_CTRL_DRV_LIB_SRCS})

# ============================================================================
# SDK mode — link prebuilt BLE controller libraries
# ============================================================================
else()
    # Stack library (chip-independent)
    set(_stack_lib "${BLE_CTRL_BASE}/libblecontroller_stack.a")
    if(EXISTS "${_stack_lib}")
        target_link_libraries(${ZEPHYR_CURRENT_MODULE_NAME}_interface INTERFACE
            "${_stack_lib}")
        message(STATUS "UniSDK SDK: BLE: Linking ${_stack_lib}")
    else()
        message(WARNING "UniSDK SDK: BLE stack lib not found: ${_stack_lib}")
    endif()

    # Chip-specific driver library (named per CONFIG_TLK_CORE)
    if(CONFIG_TLK_CORE)
        set(_drv_lib_name "blecontroller_drv_${CONFIG_TLK_CORE}")
        set(_drv_lib "${BLE_CTRL_BASE}/lib${_drv_lib_name}.a")
        if(EXISTS "${_drv_lib}")
            target_link_libraries(${ZEPHYR_CURRENT_MODULE_NAME}_interface INTERFACE
                "${_drv_lib}")
            message(STATUS "UniSDK SDK: BLE: Linking ${_drv_lib}")
        else()
            message(WARNING "UniSDK SDK: BLE driver lib not found: ${_drv_lib}")
        endif()
    endif()
endif()