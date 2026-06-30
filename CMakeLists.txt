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
set(BLE_CTRL_LIB_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

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

if(NOT IS_DIRECTORY ${CMAKE_SOURCE_DIR}/libraries)
    # =========================================================================
    # Dev mode — build the BLE controller static libraries from source
    # =========================================================================

    # --- BLE protocol stack library (chip-independent) ---
    add_library(telink-unisdk-blecontroller-stack STATIC EXCLUDE_FROM_ALL)
    set_target_properties(telink-unisdk-blecontroller-stack PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${BLE_CTRL_LIB_OUTPUT_DIR}
    )

    # --- BLE driver library (chip-specific, named per chip) ---
    add_library(${BLE_CTRL_DRV_TARGET} STATIC EXCLUDE_FROM_ALL)
    set_target_properties(${BLE_CTRL_DRV_TARGET} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${BLE_CTRL_LIB_OUTPUT_DIR}
    )

    # Propagate compiler options from the main target to both libraries
    get_target_property(_ble_ctrl_opts ${TARGET_NAME} COMPILE_OPTIONS)
    if(_ble_ctrl_opts)
        target_compile_options(telink-unisdk-blecontroller-stack PRIVATE ${_ble_ctrl_opts})
        target_compile_options(${BLE_CTRL_DRV_TARGET} PRIVATE ${_ble_ctrl_opts})
    endif()

    # Collect source files via subdirectories.
    # Subdirectory CMakeLists.txt files use ${BLE_CTRL_DRV_TARGET} to add
    # sources to the chip-specific driver library.
    add_subdirectory(stack)            # → libtelink-unisdk-blecontroller-stack
    add_subdirectory(drivers)          # → ${BLE_CTRL_DRV_TARGET}
    add_subdirectory(hal)              # → ${BLE_CTRL_DRV_TARGET}

    # Propagate include directories from the main target to both libraries.
    # This is placed AFTER add_subdirectory() so that the subdirectories'
    # target_include_directories(${TARGET_NAME} ...) calls have already been
    # processed and their paths are captured.
    get_target_property(_ble_ctrl_includes ${TARGET_NAME} INCLUDE_DIRECTORIES)
    if(_ble_ctrl_includes)
        target_include_directories(telink-unisdk-blecontroller-stack PRIVATE ${_ble_ctrl_includes})
        target_include_directories(${BLE_CTRL_DRV_TARGET} PRIVATE ${_ble_ctrl_includes})
    endif()

    # Link both libraries into the main executable
    target_link_libraries(${TARGET_NAME} PRIVATE telink-unisdk-blecontroller-stack ${BLE_CTRL_DRV_TARGET})
else()
    # =========================================================================
    # Exported SDK mode — link the prebuilt static libraries directly
    # =========================================================================
    # Subdirectories are NOT added (sources have been removed from the export).
    # The prebuilt .a files were placed here by build_libs.sh.
    target_link_libraries(${TARGET_NAME} PRIVATE
        ${BLE_CTRL_LIB_OUTPUT_DIR}/libtelink-unisdk-blecontroller-stack.a
        ${BLE_CTRL_LIB_OUTPUT_DIR}/lib${BLE_CTRL_DRV_TARGET}.a)
endif()
