/* Door Bell Demo

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <nvs_flash.h>
#include <sys/param.h>

#include "argtable3/argtable3.h"
#include "common.h"
#include "esp_cpu.h"
#include "esp_timer.h"
#include "esp_webrtc.h"
#include "media_lib_adapter.h"
#include "media_lib_netif.h"
#include "media_lib_os.h"
#include "settings.h"
#include "webrtc_utils_time.h"

static const char *TAG = "Webrtc_Test";

#define RUN_ASYNC(name, body)                                                                      \
    void run_async##name(void *arg) {                                                              \
        body;                                                                                      \
        media_lib_thread_destroy(NULL);                                                            \
    }                                                                                              \
    media_lib_thread_create_from_scheduler(NULL, #name, run_async##name, NULL);

static void thread_scheduler(const char *thread_name, media_lib_thread_cfg_t *thread_cfg) {
    if (strcmp(thread_name, "pc_task") == 0) {
        thread_cfg->stack_size = 25 * 1024;
        thread_cfg->priority = 18;
        thread_cfg->core_id = 1;
    }
    if (strcmp(thread_name, "start") == 0) {
        thread_cfg->stack_size = 6 * 1024;
    }
    if (strcmp(thread_name, "venc") == 0) {
#if CONFIG_IDF_TARGET_ESP32S3
        thread_cfg->stack_size = 20 * 1024;
#endif
        thread_cfg->priority = 10;
    }
#ifdef WEBRTC_SUPPORT_OPUS
    if (strcmp(thread_name, "aenc") == 0) {
        thread_cfg->stack_size = 40 * 1024;
        thread_cfg->priority = 10;
    }
#endif
}

static char *get_network_ip(void) {
    media_lib_ipv4_info_t ip_info;
    media_lib_netif_get_ipv4_info(MEDIA_LIB_NET_TYPE_STA, &ip_info);
    return media_lib_ipv4_ntoa(&ip_info.ip);
}

static int network_event_handler(bool connected) {
    if (connected) {
        // Enter into Room directly
        RUN_ASYNC(start, {
            start_webrtc(NULL);
            ESP_LOGI(TAG, "Use browser to enter https://%s/webrtc/test for test", get_network_ip());
        });
    } else {
        stop_webrtc();
    }
    return 0;
}

void app_main(void) {
    esp_log_level_set("*", ESP_LOG_INFO);
    media_lib_add_default_adapter();
    media_lib_thread_set_schedule_cb(thread_scheduler);
    init_board();
    media_sys_buildup();
    network_init(WIFI_SSID, WIFI_PASSWORD, network_event_handler);
    while (1) {
        media_lib_thread_sleep(2000);
        query_webrtc();
    }
}
