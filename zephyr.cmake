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
set(BLE_CTRL_BASE   ${CMAKE_CURRENT_LIST_DIR})

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
)

# ============================================================================
# 3. Source files and library linking
#
# Two modes:
#   Dev mode  (libraries/ does NOT exist): compile sources via zephyr_sources()
#   SDK mode  (libraries/     exists):     link prebuilt .a directly
# ============================================================================
if(NOT EXISTS "${BLE_CTRL_BASE}/libraries")
    include(${BLE_CTRL_BASE}/zephyr_src.cmake)

# ============================================================================
# SDK mode — link prebuilt BLE controller libraries
# ============================================================================
else()
    # Stack library (chip-independent)
    set(_stack_lib "${BLE_CTRL_BASE}/libraries/libblecontroller_stack.a")
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
        set(_drv_lib "${BLE_CTRL_BASE}/libraries/lib${_drv_lib_name}.a")
        if(EXISTS "${_drv_lib}")
            target_link_libraries(${ZEPHYR_CURRENT_MODULE_NAME}_interface INTERFACE
                "${_drv_lib}")
            message(STATUS "UniSDK SDK: BLE: Linking ${_drv_lib}")
        else()
            message(WARNING "UniSDK SDK: BLE driver lib not found: ${_drv_lib}")
        endif()
    endif()
endif()