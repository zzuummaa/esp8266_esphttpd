cmake_minimum_required(VERSION 3.10)
project(esphttp_test)

set(CMAKE_CXX_STANDARD 11)

file(GLOB LIB_libesphttpd_CORE_SRC ${ESP_OPEN_RTOS_DIR}/extras/libesphttpd/libesphttpd/core/*.c)
file(GLOB LIB_libesphttpd_ESPFS_SRC ${ESP_OPEN_RTOS_DIR}/extras/libesphttpd/libesphttpd/espfs/*.c)
file(GLOB LIB_libesphttpd_UTIL_SRC ${ESP_OPEN_RTOS_DIR}/extras/libesphttpd/libesphttpd/util/*.c)

add_library(libesphttpd STATIC ${LIB_libesphttpd_CORE_SRC} ${LIB_libesphttpd_ESPFS_SRC} ${LIB_libesphttpd_UTIL_SRC})

set(LIBESPHTTPD_MAX_CONNECTIONS  4)
set(LIBESPHTTPD_STACKSIZE 2048)

target_compile_options(libesphttpd PUBLIC ${COMMON_FLAGS}
        -DFREERTOS -DUSE_OPEN_SDK
        -DHTTPD_MAX_CONNECTIONS=${LIBESPHTTPD_MAX_CONNECTIONS}
        -DHTTPD_STACKSIZE=${LIBESPHTTPD_STACKSIZE} -DESPFS_HEATSHRINK -D__ets__ -DRBOOT_OTA  )

target_include_directories(libesphttpd PUBLIC
            ${ESP_OPEN_RTOS_DIR}/bootloader
            ${ESP_OPEN_RTOS_DIR}/bootloader/rboot
            ${ESP_OPEN_RTOS_DIR}/extras/rboot-ota
            ${ESP_OPEN_RTOS_DIR}/extras/libesphttpd/libesphttpd/include
            ${ESP_OPEN_RTOS_DIR}/extras/libesphttpd/libesphttpd/espfs
            ${ESP_OPEN_RTOS_DIR}/extras/libesphttpd/libesphttpd/lib/heatshrink
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

# make binary file with html resources
set(MKESPFSIMAGE_DIR ${ESP_OPEN_RTOS_DIR}/extras/libesphttpd/libesphttpd/espfs/mkespfsimage)
set(HTMLDIR ${PROJECT_SOURCE_DIR}/html)

file(GLOB_RECURSE WEB_FILES
     RELATIVE ${HTMLDIR}
     FOLLOW_SYMLINKS *.js *.html *.css *.tpl *.jpg *.png)

add_custom_target(fs_image
        COMMAND mkdir -p mkespfsimage
        COMMAND $(MAKE) V=1 -C ${MKESPFSIMAGE_DIR} CC=gcc GZIP_COMPRESSION=yes USE_HEATSHRINK=yes BUILD_DIR=${CMAKE_CURRENT_BINARY_DIR}/
        COMMAND cd ${HTMLDIR} && find ${WEB_FILES} | ${CMAKE_CURRENT_BINARY_DIR}/mkespfsimage/mkespfsimage > web.espfs.bin || rm -f ${CMAKE_CURRENT_BINARY_DIR}web.espfs.bin
        COMMAND mkdir -p build && mv ${HTMLDIR}/web.espfs.bin build
        COMMAND ${CMAKE_OBJCOPY} -I binary -O elf32-xtensa-le -B xtensa --rename-section .data=.irom.espfs build/web.espfs.bin build/web.espfs.o
        COMMAND ${CMAKE_OBJDUMP} -x build/web.espfs.o
        COMMENT "make file system image"
        )

set(WEB_ESP_OBJ ${PROJECT_BINARY_DIR}/build/web.espfs.o PARENT_SCOPE)