cmake_minimum_required(VERSION 3.10)
project(esphttp_test)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE 0)

# Flash size in megabits
# Valid values are same as for esptool.py - 512KB, 1MB, 2Mb, 4Mb, 8MB
SET(FLASH_SIZE 2MB)

# Flash mode, valid values are same as for esptool.py - qio,qout,dio.dout
SET(FLASH_MODE qio)

# Flash speed in MHz, valid values are same as for esptool.py - 80, 40, 26, 20
set(FLASH_SPEED 40)

set(ESPPORT /dev/ttyUSB0)

# set this to 0 if you don't need floating point support in printf/scanf
# this will save approx 14.5KB flash space and 448 bytes of statically allocated
# data RAM
#
# NB: Setting the value to 0 requires a recent esptool.py (Feb 2016 / commit ebf02c9)
set(PRINTF_SCANF_FLOAT_SUPPORT,0)

SET(USER_DEFFINITIONS -DFLASH_SIZE=2 -DLIBESPHTTPD_OTA_TAGNAME=\"generic\")
SET(COMMON_FLAGS -mlongcalls -mtext-section-literals -DGITSHORTREV=\"d0373af\" -ffunction-sections -fdata-sections -Wall -Wl,-EL -DLWIP_MDNS_RESPONDER=1 -DLWIP_NUM_NETIF_CLIENT_DATA=1 -DLWIP_NETIF_EXT_STATUS_CALLBACK=1 ${USER_DEFFINITIONS})

SET(CMAKE_CXX_FLAGS "-std=c++11")
#set(CMAKE_CXX_FLAGS_RELEASE "-g -O2")
SET(CMAKE_C_FLAGS "-std=gnu99")
#set(CMAKE_C_FLAGS_RELEASE "-g -O2")

set(ESP_OPEN_RTOS_DIR /home/zuma/esp/esp-open-rtos)
set(BOOT_FIRMWARE ${ESP_OPEN_RTOS_DIR}/bootloader/firmware_prebuilt/rboot.bin)
set(LWIP_DIR ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/)
set(LWIP_INCLUDE ${LWIP_DIR}/include ${ESP_OPEN_RTOS_DIR}/lwip/include)
set(FREE_RTOS ${ESP_OPEN_RTOS_DIR}/FreeRTOS/Source)
set(OPEN_ESPLISBS ${ESP_OPEN_RTOS_DIR}/open_esplibs)

# TODO build rboot OTA lib from rboot OTA dir
set(RBOOT_OTA_DIR ${ESP_OPEN_RTOS_DIR}/extras/rboot-ota)
set(RBOOT_OTA_SOURCES ${RBOOT_OTA_DIR}/rboot-api.c ${RBOOT_OTA_DIR}/ota-tftp.c)
set(PROGRAM_SRC mainUser.c cgi.c cgi.h cgi-test.c cgi-test.h io.c io.h ${RBOOT_OTA_SOURCES} http_ota_task.h http_ota_task.c)

add_subdirectory(dhcpserver)
add_subdirectory(rboot)
#add_subdirectory(rboot_ota)
add_subdirectory(libesphttpd)
add_subdirectory(http_client_ota)
add_subdirectory(mbedtls)

SET(LIB_MAIN_SOURCES
        ${ESP_OPEN_RTOS_DIR}/open_esplibs/libmain/user_interface.c
        ${ESP_OPEN_RTOS_DIR}/open_esplibs/libmain/ets_timer.c
        ${ESP_OPEN_RTOS_DIR}/open_esplibs/libmain/spi_flash.c
        ${ESP_OPEN_RTOS_DIR}/open_esplibs/libmain/os_cpu_a.c
        ${ESP_OPEN_RTOS_DIR}/open_esplibs/libmain/uart.c
        ${ESP_OPEN_RTOS_DIR}/open_esplibs/libmain/misc.c
        ${ESP_OPEN_RTOS_DIR}/open_esplibs/libmain/timers.c
        )
SET(LIB_MAIN_SOURCES_ASM
        ${ESP_OPEN_RTOS_DIR}/open_esplibs/libmain/xtensa_context.S
        )
SET_SOURCE_FILES_PROPERTIES(${LIB_MAIN_SOURCES_ASM} PROPERTIES LANGUAGE C)

SET(LIB_NET_SOURCES
        ${ESP_OPEN_RTOS_DIR}/open_esplibs/libnet80211/ieee80211_hostap.c
        ${ESP_OPEN_RTOS_DIR}/open_esplibs/libnet80211/wl_cnx.c
        ${ESP_OPEN_RTOS_DIR}/open_esplibs/libnet80211/ieee80211_ets.c
        ${ESP_OPEN_RTOS_DIR}/open_esplibs/libnet80211/ieee80211_input.c
        ${ESP_OPEN_RTOS_DIR}/open_esplibs/libnet80211/ieee80211_sta.c
        )
SET(LIB_PHY_SOURCES
        ${ESP_OPEN_RTOS_DIR}/open_esplibs/libphy/phy_chip_v6.c
        ${ESP_OPEN_RTOS_DIR}/open_esplibs/libphy/phy.c
        ${ESP_OPEN_RTOS_DIR}/open_esplibs/libphy/phy_sleep.c
        )

SET(LIB_PP_SOURCES
        ${ESP_OPEN_RTOS_DIR}/open_esplibs/libpp/pm.c
        ${ESP_OPEN_RTOS_DIR}/open_esplibs/libpp/if_hwctrl.c
        ${ESP_OPEN_RTOS_DIR}/open_esplibs/libpp/pp.c
        ${ESP_OPEN_RTOS_DIR}/open_esplibs/libpp/esf_buf.c
        ${ESP_OPEN_RTOS_DIR}/open_esplibs/libpp/lmac.c
        ${ESP_OPEN_RTOS_DIR}/open_esplibs/libpp/wdev.c
        )

SET(LIB_WPA_SOURCES
        ${ESP_OPEN_RTOS_DIR}/open_esplibs/libwpa/os_xtensa.c
        ${ESP_OPEN_RTOS_DIR}/open_esplibs/libwpa/wpa_main.c
        )

SET(FREE_RTOS_SOURCES
        ${ESP_OPEN_RTOS_DIR}/FreeRTOS/Source/queue.c
        ${ESP_OPEN_RTOS_DIR}/FreeRTOS/Source/croutine.c
        ${ESP_OPEN_RTOS_DIR}/FreeRTOS/Source/list.c
        ${ESP_OPEN_RTOS_DIR}/FreeRTOS/Source/stream_buffer.c
        ${ESP_OPEN_RTOS_DIR}/FreeRTOS/Source/event_groups.c
        ${ESP_OPEN_RTOS_DIR}/FreeRTOS/Source/tasks.c
        ${ESP_OPEN_RTOS_DIR}/FreeRTOS/Source/timers.c
        ${ESP_OPEN_RTOS_DIR}/FreeRTOS/Source/portable/esp8266/port.c
        )

SET(LWIP_SOURCES
        ${ESP_OPEN_RTOS_DIR}/lwip//sys_arch.c
        ${ESP_OPEN_RTOS_DIR}/lwip//esp_interface.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/api/netdb.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/api/api_lib.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/api/netifapi.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/api/tcpip.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/api/netbuf.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/api/if_api.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/api/err.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/api/api_msg.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/api/sockets.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/core/mem.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/core/inet_chksum.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/core/init.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/core/memp.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/core/sys.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/core/tcp.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/core/timeouts.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/core/dns.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/core/ip.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/core/raw.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/core/tcp_in.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/core/pbuf.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/core/netif.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/core/stats.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/core/altcp.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/core/def.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/core/altcp_tcp.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/core/udp.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/core/tcp_out.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/core/ipv4/ip4_frag.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/core/ipv4/dhcp.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/core/ipv4/etharp.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/core/ipv4/ip4_addr.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/core/ipv4/igmp.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/core/ipv4/ip4.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/core/ipv4/autoip.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/core/ipv4/icmp.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/core/ipv6/ip6_frag.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/core/ipv6/dhcp6.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/core/ipv6/inet6.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/core/ipv6/ip6_addr.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/core/ipv6/ip6.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/core/ipv6/nd6.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/core/ipv6/mld6.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/core/ipv6/ethip6.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/core/ipv6/icmp6.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/netif/slipif.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/netif/ethernet.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/netif/bridgeif_fdb.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/netif/lowpan6.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/netif/bridgeif.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/netif/ethernetif.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/apps/lwiperf/lwiperf.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/apps/snmp/snmp_threadsync.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/apps/snmp/snmp_scalar.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/apps/snmp/snmpv3.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/apps/snmp/snmp_mib2.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/apps/snmp/snmp_asn1.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/apps/snmp/snmp_pbuf_stream.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/apps/snmp/snmp_mib2_tcp.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/apps/snmp/snmp_mib2_system.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/apps/snmp/snmp_mib2_ip.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/apps/snmp/snmpv3_mbedtls.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/apps/snmp/snmp_netconn.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/apps/snmp/snmp_mib2_snmp.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/apps/snmp/snmp_traps.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/apps/snmp/snmp_mib2_interfaces.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/apps/snmp/snmp_core.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/apps/snmp/snmp_msg.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/apps/snmp/snmp_mib2_icmp.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/apps/snmp/snmp_raw.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/apps/snmp/snmp_table.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/apps/snmp/snmp_snmpv2_framework.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/apps/snmp/snmp_snmpv2_usm.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/apps/snmp/snmp_mib2_udp.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/apps/sntp/sntp.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/apps/netbiosns/netbiosns.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/apps/smtp/smtp.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/apps/mdns/mdns.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/apps/http/http_client.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/apps/http/fsdata.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/apps/http/httpd.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/apps/http/fs.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/apps/mqtt/mqtt.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/apps/tftp/tftp_server.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/apps/altcp_tls/altcp_tls_mbedtls_mem.c
        ${ESP_OPEN_RTOS_DIR}/lwip/lwip/src/apps/altcp_tls/altcp_tls_mbedtls.c
        )

SET(CORE_SOURCES ${ESP_OPEN_RTOS_DIR}/core/esp_interrupts.c
        ${ESP_OPEN_RTOS_DIR}/core/app_main.c
        ${ESP_OPEN_RTOS_DIR}/core/spiflash.c
        ${ESP_OPEN_RTOS_DIR}/core/esp_spi.c
        ${ESP_OPEN_RTOS_DIR}/core/sdk_compat.c
        ${ESP_OPEN_RTOS_DIR}/core/sysparam.c
        ${ESP_OPEN_RTOS_DIR}/core/esp_gpio.c
        ${ESP_OPEN_RTOS_DIR}/core/phy_info.c
        ${ESP_OPEN_RTOS_DIR}/core/esp_phy.c
        ${ESP_OPEN_RTOS_DIR}/core/newlib_syscalls.c
        ${ESP_OPEN_RTOS_DIR}/core/esp_iomux.c
        ${ESP_OPEN_RTOS_DIR}/core/esp_timer.c
        ${ESP_OPEN_RTOS_DIR}/core/esp_hwrand.c
        ${ESP_OPEN_RTOS_DIR}/core/debug_dumps.c
        ${ESP_OPEN_RTOS_DIR}/core/cplusplus_operators.cpp)
SET(CORE_SOURCES_ASM
        ${ESP_OPEN_RTOS_DIR}/core/exception_vectors.S
        ${ESP_OPEN_RTOS_DIR}/core/spiflash-cache-enable.S
        )

SET_SOURCE_FILES_PROPERTIES(${CORE_SOURCES_ASM} PROPERTIES LANGUAGE C)


add_library(program STATIC ${PROGRAM_SRC})
add_library(open_esplibs_libmain STATIC ${LIB_MAIN_SOURCES} ${LIB_MAIN_SOURCES_ASM})
add_library(open_esplibs_net STATIC ${LIB_NET_SOURCES})
add_library(open_esplibs_phy STATIC ${LIB_PHY_SOURCES})
add_library(open_esplibs_pp STATIC ${LIB_PP_SOURCES})
add_library(open_esplibs_wpa STATIC ${LIB_WPA_SOURCES})
add_library(freeRTOS STATIC ${FREE_RTOS_SOURCES})
add_library(lwip STATIC ${LWIP_SOURCES})
add_library(core STATIC ${CORE_SOURCES} ${CORE_SOURCES_ASM})

add_executable(esphttp_test FreeRTOSConfig.h)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/sdklib)

add_custom_target(remove)

add_custom_command(TARGET remove
        POST_BUILD
        COMMENT "Remove phase"
        COMMAND grep -v "^#" ${ESP_OPEN_RTOS_DIR}/libc/libc.remove | cat > libc.remove
        COMMAND cat ${ESP_OPEN_RTOS_DIR}/libc/xtensa-lx106-elf/lib/libc.a > libc.a
        COMMAND ${CMAKE_AR} d libc.a @libc.remove

        COMMAND grep -v "^#" ${ESP_OPEN_RTOS_DIR}/lib/libgcc.remove | cat > libgcc.remove
        COMMAND cat ${ESP_OPEN_RTOS_DIR}/lib/libgcc.a > libgcc.a
        COMMAND ${CMAKE_AR} d libgcc.a @libgcc.remove

        COMMAND grep -v "^#" ${ESP_OPEN_RTOS_DIR}/lib/libmain.remove | cat > sdklib/libmain.remove
        COMMAND cat ${ESP_OPEN_RTOS_DIR}/lib/libmain.a > sdklib/libmain_stage1.a
        COMMAND ${CMAKE_AR} d sdklib/libmain_stage1.a @sdklib/libmain.remove

        COMMAND grep -v "^#" ${ESP_OPEN_RTOS_DIR}/lib/libnet80211.remove | cat > sdklib/libnet80211.remove
        COMMAND cat ${ESP_OPEN_RTOS_DIR}/lib/libnet80211.a > sdklib/libnet80211_stage1.a
        COMMAND ${CMAKE_AR} d sdklib/libnet80211_stage1.a @sdklib/libnet80211.remove

        COMMAND grep -v "^#" ${ESP_OPEN_RTOS_DIR}/lib/libphy.remove | cat > sdklib/libphy.remove
        COMMAND cat ${ESP_OPEN_RTOS_DIR}/lib/libphy.a > sdklib/libphy_stage1.a
        COMMAND ${CMAKE_AR} d sdklib/libphy_stage1.a @sdklib/libphy.remove

        COMMAND grep -v "^#" ${ESP_OPEN_RTOS_DIR}/lib/libpp.remove | cat > sdklib/libpp.remove
        COMMAND cat ${ESP_OPEN_RTOS_DIR}/lib/libpp.a > sdklib/libpp_stage1.a
        COMMAND ${CMAKE_AR} d sdklib/libpp_stage1.a @sdklib/libpp.remove

        COMMAND grep -v "^#" ${ESP_OPEN_RTOS_DIR}/lib/libwpa.remove | cat > sdklib/libwpa.remove
        COMMAND cat ${ESP_OPEN_RTOS_DIR}/lib/libwpa.a > sdklib/libwpa_stage1.a
        COMMAND ${CMAKE_AR} d sdklib/libwpa_stage1.a @sdklib/libwpa.remove
        )

add_custom_target(rename)
add_custom_command(TARGET rename
        POST_BUILD
        COMMENT "Rename phase"
        COMMAND ${CMAKE_OBJCOPY} --redefine-syms ${ESP_OPEN_RTOS_DIR}/lib/allsymbols.rename --weaken sdklib/libmain_stage1.a sdklib/libmain.a
        COMMAND ${CMAKE_OBJCOPY} --redefine-syms ${ESP_OPEN_RTOS_DIR}/lib/allsymbols.rename --weaken sdklib/libnet80211_stage1.a sdklib/libnet80211.a
        COMMAND ${CMAKE_OBJCOPY} --redefine-syms ${ESP_OPEN_RTOS_DIR}/lib/allsymbols.rename --weaken sdklib/libphy_stage1.a sdklib/libphy.a
        COMMAND ${CMAKE_OBJCOPY} --redefine-syms ${ESP_OPEN_RTOS_DIR}/lib/allsymbols.rename --weaken sdklib/libpp_stage1.a sdklib/libpp.a
        COMMAND ${CMAKE_OBJCOPY} --redefine-syms ${ESP_OPEN_RTOS_DIR}/lib/allsymbols.rename --weaken sdklib/libwpa_stage1.a sdklib/libwpa.a
        )

add_custom_target(createImage)


add_custom_command(TARGET createImage
        POST_BUILD
        COMMENT "create image for ESP8266"
        COMMAND esptool.py elf2image --version=2 -fs ${FLASH_SIZE} -fm ${FLASH_MODE} -ff ${FLASH_SPEED}m esphttp_test -o esphttp_test.bin
        )

add_custom_target(flash)
add_custom_command(TARGET flash
        POST_BUILD
        COMMENT "Flash esp8266"
        COMMAND esptool.py -p ${ESPPORT} --baud 115200
        write_flash
        -fs ${FLASH_SIZE}
        -fm ${FLASH_MODE}
        -ff ${FLASH_SPEED}m
        0x0 ${BOOT_FIRMWARE}
        0x1000 ${ESP_OPEN_RTOS_DIR}/bootloader/firmware_prebuilt/blank_config.bin
        0x2000 esphttp_test.bin
        )

add_dependencies(rename remove)
add_dependencies(esphttp_test rename)
add_dependencies(esphttp_test fs_image)
add_dependencies(createImage esphttp_test)
add_dependencies(flash createImage)


SET(CMAKE_EXE_LINKER_FLAGS "-Wl,--no-check-sections -u call_user_start -Wl,-static -Wl,-Map=./esphttp_test.map -Wl,-gc-sections -T${ESP_OPEN_RTOS_DIR}/ld/program.ld -T${ESP_OPEN_RTOS_DIR}/ld/rom.ld")

if (PRINTF_SCANF_FLOAT_SUPPORT)
    SET(CMAKE_EXE_LINKER_FLAGS, "-u _printf_float -u _scanf_float")
endif (PRINTF_SCANF_FLOAT_SUPPORT)

target_link_libraries(esphttp_test
        -Wl,--no-whole-archive
        -Wl,--start-group
        program
        core
        ${CMAKE_CURRENT_BINARY_DIR}/libgcc.a
        ${CMAKE_CURRENT_BINARY_DIR}/libc.a
        hal

        open_esplibs_libmain
        open_esplibs_net
        open_esplibs_phy
        open_esplibs_pp
        open_esplibs_wpa
        freeRTOS
        lwip
        dhcpserver libesphttpd
        http_client_ota mbedtls

        ${CMAKE_CURRENT_BINARY_DIR}/sdklib/libmain.a
        ${CMAKE_CURRENT_BINARY_DIR}/sdklib/libnet80211.a
        ${CMAKE_CURRENT_BINARY_DIR}/sdklib/libphy.a
        ${CMAKE_CURRENT_BINARY_DIR}/sdklib/libpp.a
        ${CMAKE_CURRENT_BINARY_DIR}/sdklib/libwpa.a

        ${WEB_ESP_OBJ}
        -Wl,--end-group)

target_compile_options(open_esplibs_libmain PRIVATE "${COMMON_FLAGS}")
target_include_directories(open_esplibs_libmain PUBLIC
        ./
        ./include
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
target_compile_options(open_esplibs_net PRIVATE "${COMMON_FLAGS}")
target_include_directories(open_esplibs_net PUBLIC
        ./
        ./include
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

target_compile_options(open_esplibs_pp PRIVATE "${COMMON_FLAGS}")
target_include_directories(open_esplibs_pp PUBLIC
        ./
        ./include
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

target_compile_options(open_esplibs_phy PRIVATE "${COMMON_FLAGS}")
target_include_directories(open_esplibs_phy PUBLIC
        ./
        ./include
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

target_compile_options(open_esplibs_wpa PRIVATE "${COMMON_FLAGS}")
target_include_directories(open_esplibs_wpa PUBLIC
        ./
        ./include
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

target_compile_options(freeRTOS PRIVATE "${COMMON_FLAGS}")
target_include_directories(freeRTOS PUBLIC
        ./
        ./include
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

target_compile_options(lwip PRIVATE "${COMMON_FLAGS}")
target_include_directories(lwip PUBLIC
        ./
        ./include
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

target_compile_options(core PRIVATE "${COMMON_FLAGS}")
target_include_directories(core PUBLIC
        ./
        ./include
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

target_compile_options(program PRIVATE "${COMMON_FLAGS}")

target_link_libraries(program dhcpserver libesphttpd)


target_include_directories(program PUBLIC
        ./
        ${PROJECT_BINARY_DIR}
        ./include
        ${ESP_OPEN_RTOS_DIR}/extras
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
        ${ESP_OPEN_RTOS_DIR}/extras/http_client_ota
        ${ESP_OPEN_RTOS_DIR}/extras/http_client_ota/include
        ${ESP_OPEN_RTOS_DIR}/extras/dhcpserver
        ${ESP_OPEN_RTOS_DIR}/extras/dhcpserver/include
        )

target_compile_options(esphttp_test PRIVATE "${COMMON_FLAGS}")


target_include_directories(esphttp_test PUBLIC
        ./
        ./include
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
        ${ESP_OPEN_RTOS_DIR}/extras/rboot-ota
        )

#Get master directory
get_filename_component(MASTER_DIR ${CMAKE_SOURCE_DIR} DIRECTORY)
get_filename_component(MASTER_DIR ${MASTER_DIR} DIRECTORY)

#Script for version update
add_custom_target(inc_version
        COMMAND ${CMAKE_COMMAND}
        -DHEADER_DIR:PATH="${CMAKE_BINARY_DIR}"
        -DCACHE_DIR:PATH="${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}"
        -DLANGUAGE:STRING="C"
        -P "../build_number.cmake"
        COMMENT "Increment firmware version"
)

add_dependencies(program inc_version)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/CreateImage.sh
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
     FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

add_custom_command(
        TARGET createImage
        POST_BUILD
        COMMAND
        COMMAND export PROJECT_NAME=${PROJECT_NAME} && ./CreateImage.sh
        COMMENT "Generate OTA server content")