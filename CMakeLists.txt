# SPDX-License-Identifier: Apache-2.0
#
# BLE Controller library packaging
#
# Two categories of static libraries:
#   libtelink-unisdk-blecontroller-stack          — BLE protocol stack (stack/, chip-independent)
#   libtelink-unisdk-blecontroller-hal_<CHIP>     — Chip-specific drivers (drivers/<CHIP>/ + hal/)
#
# Examples: libtelink-unisdk-blecontroller-hal_TL721X, libtelink-unisdk-blecontroller-hal_TL321X, libtelink-unisdk-blecontroller-hal_B92
#
# This file handles two modes:
#   1) Dev mode  (libraries/ does NOT exist) — builds static libs from source
#   2) SDK mode  (libraries/     exists)     — links prebuilt .a directly
#
# The BLE controller packaging is completely independent of the core UniSDK
# library system — no coupling with telink_add_library() or library.cmake.

# Include paths are always needed (headers are always present)
target_include_directories(${TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Common archive output directory for all BLE controller libraries
set(BLE_CTRL_LIB_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libraries)

# Determine the chip-specific driver library name from Kconfig.
# CONFIG_TLK_CORE is set by the SoC Kconfig (e.g. "TL721X", "TL321X", "B92").
if(CONFIG_TLK_CORE)
    set(BLE_CTRL_DRV_TARGET "telink-unisdk-blecontroller-hal_${CONFIG_TLK_CORE}")
else()
    message(FATAL_ERROR "CONFIG_TLK_CORE must be set to a valid chip identifier (e.g. TL721X, TL321X, B92)")
endif()

# Open-source sources
target_sources(${TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/tlk_ble_controller_initial.c
)

if(IS_DIRECTORY ${BLE_CTRL_LIB_OUTPUT_DIR})
    # =========================================================================
    # SDK mode — link the prebuilt static libraries directly
    # =========================================================================
    # Subdirectories are NOT added (sources have been removed from the export).
    # The prebuilt .a files were placed here by build_libs.sh.
    target_link_libraries(${TARGET_NAME} PRIVATE
        ${BLE_CTRL_LIB_OUTPUT_DIR}/libtelink-unisdk-blecontroller-stack.a
        ${BLE_CTRL_LIB_OUTPUT_DIR}/lib${BLE_CTRL_DRV_TARGET}.a)

else()
    # =========================================================================
    # Dev mode — compile sources directly into ${TARGET_NAME}
    #
    # Same structure as lib mode, but without ARCHIVE_OUTPUT_DIRECTORY.
    # The intermediate .a artifacts end up in the build directory (not in
    # libraries/ in the source tree).
    # =========================================================================
    include(${CMAKE_CURRENT_SOURCE_DIR}/src.cmake)

endif()
