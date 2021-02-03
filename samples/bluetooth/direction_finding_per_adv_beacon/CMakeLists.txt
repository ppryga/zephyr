#
# Copyright (c) 2021 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause

cmake_minimum_required(VERSION 3.13.1)

if (NOT DF_MODE)
  message("DF_MODE is not being defined on the CMake command-line in the environment or by the app\n"
          "Setting DF_MODE to default value: AOA")
  set(DF_MODE "AOA")
endif()

if (DF_MODE STREQUAL "AOA")
  set(DF_CONF_FILE "${CMAKE_CURRENT_SOURCE_DIR}/df_aoa.conf")
elseif (DF_MODE STREQUAL "AOD")
  set(DF_CONF_FILE "${CMAKE_CURRENT_SOURCE_DIR}/df_aod.conf")
else()
  message(FATAL_ERROR "Selected unsupported DF_MODE: ${DF_MODE}.\n"
          "Please set DF_MODE parameter to one of following: AOA, AOD.")
endif()

if(NOT EXISTS ${DF_CONF_FILE})
  message(FATAL_ERROR "No such file or directory: DF_CONF_FILE: '${DF_CONF_FILE}'")
endif()
list(APPEND CONF_FILE ${DF_CONF_FILE})
list(APPEND CONF_FILE "prj.conf")

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(df_beacon)

target_sources(app PRIVATE
  src/main.c
)

if (CONFIG_BT_CTLR_DF_ANT_SWITCH_TX)
set(DTC_OVERLAY_FILE "${CMAKE_CURRENT_SOURCE_DIR}/boards/nrf52833dk_nrf52833.overlay")
endif()