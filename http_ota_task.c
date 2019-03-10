//
// Created by zuma on 10.03.19.
//

#include "http_ota_task.h"

#include "espressif/esp_common.h"
#include "esp/uart.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

#include "unistd.h"
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

#include "http_client_ota.h"
#include "ssid_config.h"
#define MODULE "HTTP-OTA"

#if defined(DEBUG)
# ifndef MODULE
#  error "Module not define"
# endif

    # define DEBUG_PRINT(fmt, args ...) \
        printf("[%s]\t" fmt "\n", MODULE, ## args)
#else
# define DEBUG_PRINT(fmt, args ...) /* Don't do anything in release builds */
#endif

#define vTaskDelayMs(ms) vTaskDelay((ms) / portTICK_PERIOD_MS)

/*
 * How to test
 * cd test_file
 * python -m SimpleHTTPServer 8080
 * fill missing define SERVER and PORT, in your private_ssid_config.h
 * Ready for test.
 */


#define BINARY_PATH "/firmware.bin"
#define SHA256_PATH "/firmware.sha256"

// Default
#define SERVER "192.168.1.66"
#define PORT "8080"
#define VERSION_FILE_PATH "/version"

#ifndef SERVER
#error "Server address is not defined define it:`192.168.X.X`"
#endif

#ifndef PORT
#error "Port is not defined example:`8080`"
#endif

static inline void ota_error_handling(OTA_err err) {
    printf("Error:");

    switch(err) {
        case OTA_DNS_LOOKUP_FALLIED:
            printf("DNS lookup has fallied\n");
            break;
        case OTA_SOCKET_ALLOCATION_FALLIED:
            printf("Impossible allocate required socket\n");
            break;
        case OTA_SOCKET_CONNECTION_FALLIED:
            printf("Server unreachable, impossible connect\n");
            break;
        case OTA_SHA_DONT_MATCH:
            printf("Sha256 sum does not fit downloaded sha256\n");
            break;
        case OTA_REQUEST_SEND_FALLIED:
            printf("Impossible send HTTP request\n");
            break;
        case OTA_DOWLOAD_SIZE_NOT_MATCH:
            printf("Dowload size don't match with server declared size\n");
            break;
        case OTA_ONE_SLOT_ONLY:
            printf("rboot has only one slot configured, impossible switch it\n");
            break;
        case OTA_FAIL_SET_NEW_SLOT:
            printf("rboot cannot switch between rom\n");
            break;
        case OTA_IMAGE_VERIFY_FALLIED:
            printf("Dowloaded image binary checsum is fallied\n");
            break;
        case OTA_UPDATE_DONE:
            printf("Ota has completed upgrade process, all ready for system software reset\n");
            break;
        case OTA_HTTP_OK:
            printf("HTTP server has response 200, Ok\n");
            break;
        case OTA_HTTP_NOTFOUND:
            printf("HTTP server has response 404, file not found\n");
            break;
    }
}

/**
 * @param req
 * @param resp
 * @param resp_size
 * @param pBody pointer to body in resp string.
 *              If body is empty contains pointer to end zero character in resp.
 *              Contains NULL when error.
 * @return
 */
bool http_get_task(const char* req, char* resp, size_t resp_size, char** pBody)
{
    *pBody = NULL;
    DEBUG_PRINT("HTTP get task starting...\r\n");

    const struct addrinfo hints = {
            .ai_family = AF_UNSPEC,
            .ai_socktype = SOCK_STREAM,
    };
    struct addrinfo *res;

    DEBUG_PRINT("Running DNS lookup for %s...\r\n", SERVER);
    int err = getaddrinfo(SERVER, PORT, &hints, &res);

    if (err != 0 || res == NULL) {
        DEBUG_PRINT("DNS lookup failed err=%d res=%p\r\n", err, res);
        if(res)
            freeaddrinfo(res);
        return false;
    }

#if LWIP_IPV6
    {
            struct netif *netif = sdk_system_get_netif(0);
            int i;
            for (i = 0; i < LWIP_IPV6_NUM_ADDRESSES; i++) {
                DEBUG_PRINT("  ip6 %d state %x\n", i, netif_ip6_addr_state(netif, i));
                if (!ip6_addr_isinvalid(netif_ip6_addr_state(netif, i)))
                    DEBUG_PRINT("  ip6 addr %d = %s\n", i, ip6addr_ntoa(netif_ip6_addr(netif, i)));
            }
        }
#endif

    struct sockaddr *sa = res->ai_addr;
    if (sa->sa_family == AF_INET) {
        DEBUG_PRINT("DNS lookup succeeded. IP=%s\r\n", inet_ntoa(((struct sockaddr_in *)sa)->sin_addr));
    }
#if LWIP_IPV6
    if (sa->sa_family == AF_INET6) {
            DEBUG_PRINT("DNS lookup succeeded. IP=%s\r\n", inet6_ntoa(((struct sockaddr_in6 *)sa)->sin6_addr));
        }
#endif

    int s = socket(res->ai_family, res->ai_socktype, 0);
    if(s < 0) {
        DEBUG_PRINT("... Failed to allocate socket.\r\n");
        freeaddrinfo(res);
        return false;
    }

    DEBUG_PRINT("... allocated socket\r\n");

    if(connect(s, res->ai_addr, res->ai_addrlen) != 0) {
        close(s);
        freeaddrinfo(res);
        DEBUG_PRINT("... socket connect failed.\r\n");
        return false;
    }

    DEBUG_PRINT("... connected\r\n");
    freeaddrinfo(res);

    if (write(s, req, strlen(req)) < 0) {
        DEBUG_PRINT("... socket send failed\r\n");
        close(s);
        return false;
    }
    DEBUG_PRINT("... socket send success\r\n");

    int r;
    char* pos;
    do {
        if (resp_size < 128) {
            bzero(resp, resp_size);
            r = read(s, resp, resp_size - 1);
        } else {
            bzero(resp, 128);
            r = read(s, resp, 127);
        }

        // find body
        if (*pBody == NULL) {
            pos = strstr(resp, "\r\n\r\n");
            if (pos != NULL) *pBody = pos + 4;
        }

        if(r > 0) {
            DEBUG_PRINT("%s", resp);
        }
        resp += r;
        resp_size -= r;
    } while(r > 0);

    close(s);

    if (resp_size == 0) {
        DEBUG_PRINT("\r\n... response buffer overflowed.\r\n");
        return false;
    } else {
        DEBUG_PRINT("\r\n... done reading from socket. Last read return=%d errno=%d\r\n", r, errno);
        return true;
    }
}

static void scanVersion(const char* strVer, firmware_version* fv) {
    const char* p = strVer;
    char* end;
    int i = 2;
    while (*p != '\0') {
        end = strstr(p, ".");
        fv->version[i--] = (uint8_t) atoi(p);

        if (end == NULL || i == -1) return;
        p = end + 1;
    }
}

static void ota_task(void *PvParameter)
{
    // Wait until we have joined AP and are assigned an IP *
    while (sdk_wifi_station_get_connect_status() != STATION_GOT_IP)
        vTaskDelayMs(100);

    const char *req =
            "GET "VERSION_FILE_PATH" HTTP/1.1\r\n"
            "Host: "SERVER"\r\n"
            "User-Agent: esp-open-rtos/0.1 esp8266\r\n"
            "Connection: close\r\n"
            "\r\n";

    char resp[512];
    char* pBody;

    firmware_version curFv;
    scanVersion(FIRMWARE_VERSION, &curFv);

    while (1) {
        // Get version of OTA update
        if (!http_get_task(req, resp, sizeof(resp), &pBody)) {
            vTaskDelayMs(4000);
            continue;
        }

        DEBUG_PRINT("Response body:\r\n%s\r\n", pBody);

        firmware_version newFv;
        scanVersion(pBody, &newFv);
        DEBUG_PRINT("new firmware version: %d.%d.%d\r\n", newFv.version[2], newFv.version[1], newFv.version[0]);

        if (memcmp(&newFv, &curFv, sizeof(firmware_version)) <= 0) {
            vTaskDelayMs(4000);
            continue;
        }
        printf("updating firmware to %d.%d.%d\r\n", newFv.version[2], newFv.version[1], newFv.version[0]);

        OTA_err err;
        // Remake this task until ota work
        err = ota_update((ota_info *) PvParameter);

        ota_error_handling(err);

        if(err != OTA_UPDATE_DONE) {
            vTaskDelayMs(1000);
            printf("\n\n\n");
            continue;
        }

        vTaskDelayMs(1000);
        printf("Delay 1\n");
        vTaskDelayMs(1000);
        printf("Delay 2\n");
        vTaskDelayMs(1000);
        printf("Delay 3\n");

        printf("Reset\n");
        sdk_system_restart();
    }
}

static ota_info info = {
        .server      = SERVER,
        .port        = PORT,
        .binary_path = BINARY_PATH,
        .sha256_path = SHA256_PATH,
};

void httpOTATaskInit(void)
{
    xTaskCreate(ota_task, "get_task", 5120, &info, 2, NULL);
}