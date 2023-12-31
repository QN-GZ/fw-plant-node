# author: Quang Nguyen (quang.nguyen@goalzero.com)
# brief: Makefile for the Yeti 4000 WMU project
# note: Update the SERIAL_PORT accordingly to your environment

# comment out whichever port settings don't suit your needs
# SERIAL_PORT=/dev/cu.usbserial-0001
# SERIAL_PORT=$$MOS_PORT
SERIAL_PORT=$$(ls /dev/cu.usbserial* | head -n 1)

SERIAL_PORT=/dev/cu.usbserial-0001
ifeq ($(VERSION),)
VERSION=9.9.9
endif

ifeq ($(OUTPUT_FILE),)
OUTPUT_FILE=build/wmu-v${VERSION}.zip
endif

# install-cicd-dependencies:
# 	# install mos CLI tool
# 	sudo add-apt-repository ppa:mongoose-os/mos
# 	sudo apt-get install mos

build: ./bin/build.sh
	VERSION=${VERSION} \
	CRC=false \
	./bin/build.sh

# build-ota: ./bin/build.sh
# 	VERSION=${VERSION} \
# 	MOCK_PCU=${MOCK_PCU} \
# 	OUTPUT_FILE=${OUTPUT_FILE} \
# 	./bin/build.sh

flash:
	mos flash --port $(SERIAL_PORT)

monitor:
	mos console --port $(SERIAL_PORT) --inverted-control-lines

console: 
	mos console --port $(SERIAL_PORT)

flash-monitor: flash monitor

all: build flash monitor

clean:
	time rm -r ./build

## DEBUGGING

#  mgos_sys_config_get_gz_app_mqtt_shadow_manager_loglevel
debug-mqtt:
	mos call Config.Set '{ "config": { "gzm_mqtt": { "loglevel": 4 }, "gz_app": { "mqtt_shadow_manager": { "loglevel": 4 }, "mqtt_jobs_manager": { "loglevel": 4 } } } }' --port ${SERIAL_PORT} --inverted-control-lines && mos console --port ${SERIAL_PORT} --inverted-control-lines

debug-notifications:
	mos call Config.Set '{ "config": { "gz_app": { "notification_manager": { "loglevel": 4 } } } }' --port ${SERIAL_PORT} --inverted-control-lines && mos console --port ${SERIAL_PORT} --inverted-control-lines

monitor-debug-mgos:
	mos call Config.Set '{ "config": { "debug": { "level": 3 } }, "save": true }' --port ${SERIAL_PORT} --inverted-control-lines && mos console --port ${SERIAL_PORT}

monitor-force-ota:
	mos call debug-start-update '{"action": "post", "body": { "isStarted":0}}' --port ${SERIAL_PORT} --inverted-control-lines && mos console --port ${SERIAL_PORT} --inverted-control-lines

monitor-modbus-silence:
	mos call rpc_yapi_modbus_silence '{"action": "post", "body": { "isSilenced":1}}' --port ${SERIAL_PORT} --inverted-control-lines && mos console --port ${SERIAL_PORT} --inverted-control-lines

monitor-modbus-unsilence:
	mos call rpc_yapi_modbus_silence '{"action": "post", "body": { "isSilenced":0}}' --port ${SERIAL_PORT} --inverted-control-lines && mos console --port ${SERIAL_PORT} --inverted-control-lines

debug-wmu-power:
	mos call Config.Set '{ "config": { "gz_app": { "yapi_wmu_power": { "loglevel": 4 } } } }' --port ${SERIAL_PORT} --inverted-control-lines && mos console --port ${SERIAL_PORT} --inverted-control-lines

monitor-debug-mqtt: debug-mqtt monitor

enable-mock-pcu:
	mos call Config.Set '{ "config": { "gz_app": { "mockPcu": { "mode": "emulated" } } }, "save": true }' --port ${SERIAL_PORT} --inverted-control-lines && mos console --port ${SERIAL_PORT} --inverted-control-lines

# default target
.PHONY: build
