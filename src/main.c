/*
 * Copyright (c) 2014-2018 Cesanta Software Limited
 * All rights reserved
 *
 * Licensed under the Apache License, Version 2.0 (the ""License"");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an ""AS IS"" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mgos.h"
#include "mgos_dns_sd.h"
#include "mgos_rpc.h"
#include "mgos_rpc_channel_udp.h"
#include "mgos_wifi.h"
#include "rpc_plant.h"

#define LOGI(...) LOG(LL_INFO, (__VA_ARGS__))
#define LOGD(...) LOG(LL_DEBUG, (__VA_ARGS__))
#define LOGW(...) LOG(LL_WARN, (__VA_ARGS__))
#define LOGE(...) LOG(LL_ERROR, (__VA_ARGS__))

#define UDP_LISTENER "udp://0.0.0.0:10001"

static bool _appMainnWifiConnected = false;

static void timer_cb(void *arg) {
  char *err = NULL;
}

static void _app_main_wifi_event_cb(int ev, void *evd, void *arg) {
  switch(ev) {
    case MGOS_WIFI_EV_STA_CONNECTING:
      LOGD("WIFI STA connecting");
      _appMainnWifiConnected = false;
      break;

    case MGOS_WIFI_EV_STA_CONNECTED:
      LOGD("WIFI STA connected");
      _appMainnWifiConnected = false;
      break;

    case MGOS_WIFI_EV_STA_DISCONNECTED:
      LOGD("WIFI STA disconnected");
      _appMainnWifiConnected = false;
      break;

    case MGOS_WIFI_EV_STA_IP_ACQUIRED:
      LOGI("WIFI STA IP acquired");
      if (!_appMainnWifiConnected) {
        _appMainnWifiConnected = true;
      }
      break;

    case MGOS_WIFI_EV_AP_STA_CONNECTED:
    case MGOS_WIFI_EV_AP_STA_DISCONNECTED:
    default:
      LOGI("Nothing todo for event (%d)", ev);
      break;
  }
}

enum mgos_app_init_result mgos_app_init(void) {
#ifdef LED_PIN
  mgos_gpio_setup_output(LED_PIN, 0);
#endif
  mgos_set_timer(1000 /* ms */, MGOS_TIMER_REPEAT, timer_cb, NULL);
  char deviceId[32] = "myplant-";
  sprintf(deviceId, "myplant-%s", mgos_sys_ro_vars_get_mac_address());
  mgos_sys_config_set_device_id(deviceId);
  LOGI("App version: %s", mgos_sys_ro_vars_get_fw_version());
  LOGI("Mac address: %s", mgos_sys_ro_vars_get_mac_address());
  mgos_event_add_group_handler(MGOS_EVENT_GRP_WIFI, _app_main_wifi_event_cb, NULL);
  rpc_plant_init();
  return MGOS_APP_INIT_SUCCESS;
}
