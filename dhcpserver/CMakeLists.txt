cmake_minimum_required(VERSION 3.10)
project(esphttp_test)


set(CMAKE_CXX_STANDARD 11)

file(GLOB LIB_dhcpserver_SRC ${ESP_OPEN_RTOS_DIR}/extras/dhcpserver/*.c)
add_library(dhcpserver STATIC ${LIB_dhcpserver_SRC})

target_compile_options(dhcpserver PUBLIC ${COMMON_FLAGS} )

target_include_directories(dhcpserver PUBLIC
            ${ESP_OPEN_RTOS_DIR}/extras/dhcpserver
            ${ESP_OPEN_RTOS_DIR}/extras/dhcpserver/include
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
            )