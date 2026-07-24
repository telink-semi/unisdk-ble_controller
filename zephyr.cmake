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
# 3. Open-source sources (compiled in both modes)
#
# tlk_ble_controller_initial.c is open-source and must be compiled in both
# Dev and SDK modes.  The prebuilt .a does not contain this symbol.
# ============================================================================
zephyr_sources(${BLE_CTRL_BASE}/tlk_ble_controller_initial.c)

# ============================================================================
# 4. Source files and library linking
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
    if(CONFIG_TLK_CORE)
        set(_blecontroller_lib "${BLE_CTRL_BASE}/libraries/libtelink-unisdk-blecontroller_${CONFIG_TLK_CORE}-zephyr.a")
        if(EXISTS "${_blecontroller_lib}")
            target_link_libraries(${ZEPHYR_CURRENT_MODULE_NAME}_interface INTERFACE
                "${_blecontroller_lib}")
            message(STATUS "UniSDK SDK: BLE: Linking ${_blecontroller_lib}")
        else()
            message(WARNING "UniSDK SDK: BLE controller lib not found: ${_blecontroller_lib}")
        endif()
    endif()
endif()