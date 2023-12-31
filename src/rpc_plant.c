#include "rpc_plant.h"
#include "mgos.h"
#include "mgos_rpc.h"

#define PLANT_GET "Plant.Get"
#define PLANT_DISCOVERY "Plant.Discovery"
static uint16_t _plant_get_moisture() {
  return 100;
}

static const char* _plant_get_mac_address() {
  return mgos_sys_config_get_device_mac();
}

static int _json_printf_sysinfo_callback(struct json_out *out, va_list *ap) {
  (void) ap;
  return mgos_print_sys_info(out);
}

static void _rpc_plant_get_handler(struct mg_rpc_request_info *ri, void *cb_arg,
                                  struct mg_rpc_frame_info *fi,
                                  struct mg_str args) {
  mg_rpc_send_responsef(ri, "{moisture: %d}", _plant_get_moisture());
  (void) cb_arg;
  (void) fi;
}

static void _rpc_plant_discovery_handler(struct mg_rpc_request_info *ri, void *cb_arg,
                                  struct mg_rpc_frame_info *fi,
                                  struct mg_str args) {
  LOG(LL_INFO, ("_rpc_plant_discovery_handler from: %.*s", ri->src.len, ri->src.p));
  mg_rpc_send_responsef(ri, "%M", _json_printf_sysinfo_callback);

  (void) cb_arg;
  (void) fi;
}

void rpc_plant_init() {
  LOG(LL_INFO, ("rpc_plant_init"));
  mg_rpc_add_handler(mgos_rpc_get_global(), PLANT_GET, "", _rpc_plant_get_handler, NULL);
  mg_rpc_add_handler(mgos_rpc_get_global(), PLANT_DISCOVERY, "", _rpc_plant_discovery_handler, NULL);
}