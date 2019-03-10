cmake_minimum_required(VERSION 3.10)
project(esphttp_test)


set(CMAKE_CXX_STANDARD 11)

file(GLOB LIB_http_client_ota_SRC ${ESP_OPEN_RTOS_DIR}/extras/http_client_ota/*.c)

add_library(http_client_ota STATIC ${LIB_http_client_ota_SRC})

target_compile_options(http_client_ota PUBLIC ${COMMON_FLAGS} )

target_include_directories(http_client_ota PUBLIC
            ${ESP_OPEN_RTOS_DIR}/extras/http_client_ota
            ${ESP_OPEN_RTOS_DIR}/extras/http_client_ota/include
            ${ESP_OPEN_RTOS_DIR}/include
            ${ESP_OPEN_RTOS_DIR}/libc/xtensa-lx106-elf/include
            ${FREE_RTOS}/include
            ${FREE_RTOS}/portable/esp8266
            ${LWIP_DIR}/include
            ${ESP_OPEN_RTOS_DIR}/lwip/include
            ${LWIP_DIR}/include/compat/posix
            ${LWIP_DIR}/include/ipv4
            ${LWIP_DIR}/include/ipv4/lwip
            ${LWIP_DIR}/include/lwip
            ${ESP_OPEN_RTOS_DIR}/core/include
            ${ESP_OPEN_RTOS_DIR}/open_esplibs/include
            ${ESP_OPEN_RTOS_DIR}/extras/mbedtls/mbedtls/include
            ${ESP_OPEN_RTOS_DIR}/bootloader/
            ${ESP_OPEN_RTOS_DIR}/bootloader/rboot
            ${ESP_OPEN_RTOS_DIR}/extras/rboot-ota
            )