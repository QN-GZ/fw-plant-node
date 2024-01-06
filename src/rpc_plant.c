#include "rpc_plant.h"
#include "mgos.h"
#include "mgos_rpc.h"
#include "plant_node.h"

#define PLANT_GET "Plant.Get"
#define PLANT_DISCOVERY "Plant.Discovery"
static uint16_t _plant_get_dryness() {
  return mgos_sys_config_get_plant_dryness();
}

static int _json_printf_sysinfo_callback(struct json_out *out, va_list *ap) {
  (void) ap;
  return mgos_print_sys_info(out);
}

static void _rpc_plant_get_handler(struct mg_rpc_request_info *ri, void *cb_arg,
                                  struct mg_rpc_frame_info *fi,
                                  struct mg_str args) {
  mg_rpc_send_responsef(ri, "{dryness: %d}", _plant_get_dryness());
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

static void rpc_plant_dryness_threshold_observer(int dryness) {
  LOG(LL_INFO, ("rpc_plant_dryness_threshold_observer: %d", dryness));
  // TODO: Send RPC to server???
}

void rpc_plant_init() {
  LOG(LL_INFO, ("rpc_plant_init"));
  mg_rpc_add_handler(mgos_rpc_get_global(), PLANT_GET, "", _rpc_plant_get_handler, NULL);
  mg_rpc_add_handler(mgos_rpc_get_global(), PLANT_DISCOVERY, "", _rpc_plant_discovery_handler, NULL);
  plant_node_set_dryness_threshold_observer(rpc_plant_dryness_threshold_observer);
}