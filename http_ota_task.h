//
// Created by zuma on 10.03.19.
//

#ifndef ESPHTTP_TEST_HTTP_OTA_CLIENT_H
#define ESPHTTP_TEST_HTTP_OTA_CLIENT_H

//#define FIRMWARE_VERSION "0.0.1"

typedef struct firmware_version_t {
    char version[3];
} firmware_version;

void httpOTATaskInit(void);

#endif //ESPHTTP_TEST_HTTP_OTA_CLIENT_H
