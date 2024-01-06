#include "plant_node.h"
#include "mgos.h"
#include "mgos_adc.h"
#include "mgos_timers.h"
#include "log.h"

#define ADC_PIN 32

typedef struct observer_node {
  plant_node_dryness_threshold_observer observer;
  struct observer_node* next;
} observer_node_t;

static int _plant_dryness = 0;
static int _plant_max_dryness = 0;
static observer_node_t* _observer_head = NULL;


static void _timer_cb(void *arg) {
  // Moving average filter
  _plant_dryness = (2*_plant_dryness + mgos_adc_read(ADC_PIN))/3;
  LOGI("Dryness: %d", _plant_dryness);
  if (_plant_dryness > _plant_max_dryness) {
    LOGI("Plant is dry! Calling observers.");
    observer_node_t* observer = _observer_head;
    while (observer != NULL) {
      observer->observer(_plant_dryness);
      observer = observer->next;
    }
  }
}

void plant_node_init() {
  LOG(LL_INFO, ("plant_node_init"));
  _plant_max_dryness = mgos_sys_config_get_plant_dryness_max();
  if (mgos_adc_enable(ADC_PIN)) {
    LOGI("ADC enabled");
    mgos_set_timer(2000 /* ms */, MGOS_TIMER_REPEAT, _timer_cb, NULL);
  } else {
    LOGI("Failed: ADC not enabled");
  }
}

int plant_node_get_dryness() {
  return _plant_dryness;
}

int plant_node_set_max_dryness(int threshold) {
  _plant_max_dryness = threshold;
  mgos_sys_config_set_plant_dryness_max(threshold);
  save_cfg(&mgos_sys_config, NULL);
  return _plant_max_dryness;
}

int plant_node_get_max_dryness() {
  return _plant_max_dryness;
}

void plant_node_set_dryness_threshold_observer(plant_node_dryness_threshold_observer observer) {
  LOG(LL_INFO, ("plant_node_set_dryness_threshold_observer"));
  observer_node_t* new_observer = malloc(sizeof(observer_node_t));
  new_observer->observer = observer;
  new_observer->next = _observer_head;
  _observer_head = new_observer;
}
