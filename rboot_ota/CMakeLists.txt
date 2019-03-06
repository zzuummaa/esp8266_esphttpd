cmake_minimum_required(VERSION 3.10)
project(esphttp_test)

set(RBOOT_OTA_DIR ${ESP_OPEN_RTOS_DIR}/extras/rboot-ota)
set(RBOOT_OTA_SOURCES ${RBOOT_OTA_DIR}/rboot-api.c ${RBOOT_OTA_DIR}/ota-tftp.c)
add_library(rboot_ota ${RBOOT_OTA_SOURCES})

target_include_directories(rboot_ota PUBLIC
        ${RBOOT_OTA_DIR}
        ${ESP_OPEN_RTOS_DIR}/bootloader
        ${ESP_OPEN_RTOS_DIR}/bootloader/rboot
        ${ESP_OPEN_RTOS_DIR}/extras/rboot-ota
        ${ESP_OPEN_RTOS_DIR}/include
        ${ESP_OPEN_RTOS_DIR}/libc/xtensa-lx106-elf/include
        ${ESP_OPEN_RTOS_DIR}/core/include
        ${ESP_OPEN_RTOS_DIR}/open_esplibs/include
        ${FREE_RTOS}/include
        ${FREE_RTOS}/portable/esp8266
        ${LWIP_DIR}/include
        ${ESP_OPEN_RTOS_DIR}/lwip/include
        ${LWIP_DIR}/include/compat/posix
        ${LWIP_DIR}/include/ipv4
        ${LWIP_DIR}/include/ipv4/lwip
        ${LWIP_DIR}/include/lwip
        )