# SPDX-License-Identifier: Apache-2.0
#
# BLE Controller library packaging
#
# A single combined static library per chip:
#   libtelink-unisdk-blecontroller_<CHIP>  — Combined BLE controller (stack + drivers + hal)
#
# Examples: libtelink-unisdk-blecontroller_TL721X, libtelink-unisdk-blecontroller_TL321X, libtelink-unisdk-blecontroller_B92
#
# This file handles two modes:
#   1) Dev mode  (libraries/ does NOT exist) — builds static libs from source
#   2) SDK mode  (libraries/     exists)     — links prebuilt .a directly
#
# The BLE controller packaging is completely independent of the core UniSDK
# library system — no coupling with telink_add_library() or library.cmake.

if(NOT CONFIG_TLK_BLE_CONTROLLER)
    return()
endif()

# Include paths are always needed (headers are always present)
target_include_directories(${TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Common archive output directory for all BLE controller libraries
set(BLE_CTRL_LIB_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libraries)

# Determine the combined BLE controller library name from Kconfig.
# CONFIG_TLK_CORE is set by the SoC Kconfig (e.g. "TL721X", "TL321X", "B92").
if(CONFIG_TLK_CORE)
    if(CONFIG_TLK_TOOLCHAIN_TYPE STREQUAL "zephyr")
        set(BLE_CTRL_DRV_TARGET "telink-unisdk-blecontroller_${CONFIG_TLK_CORE}-zephyr")
    else()
        set(BLE_CTRL_DRV_TARGET "telink-unisdk-blecontroller_${CONFIG_TLK_CORE}")
    endif()
else()
    message(FATAL_ERROR "CONFIG_TLK_CORE must be set to a valid chip identifier (e.g. TL721X, TL321X, B92)")
endif()

# Open-source sources
target_sources(${TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/tlk_ble_controller_initial.c
)

if(IS_DIRECTORY ${BLE_CTRL_LIB_OUTPUT_DIR})
    # =========================================================================
    # SDK mode — link the prebuilt combined static library directly
    # =========================================================================
    # Subdirectories are NOT added (sources have been removed from the export).
    # The prebuilt .a file was placed here by build_libs.sh.
    target_link_libraries(${TARGET_NAME} PRIVATE
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
