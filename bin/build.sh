#!/usr/bin/env bash
# this script is just a temporary solution to simplify the build command
# hopefully I'll get around to replacing this with a Makefile

SECONDS=0
# set variables only if not already set
VERSION=${VERSION:-"0.0.0"}
OUTPUT_FILE=${OUTPUT_FILE:-"build/my-plant-node-v${VERSION}.zip"}
CRC=${CRC:-false}

format_time() {
  # ((h=${1}/3600))
  ((m=(${1}%3600)/60))
  ((s=${1}%60))
  # printf "%02d:%02d:%02d\n" $h $m $s
  printf "%02dm %02ds\n" $m $s
}

# run the build
mos build \
  --verbose \
  --local \
  --platform esp32 \
  --repo lib/mos/mongoose-os \
  --libs-dir lib/mos/mongoose-os-libs \
  --libs-dir lib/gz/gz_mgos_libs \
  --binary-libs-dir lib/mos/mos-lib-binaries \
  --no-libs-update \
  --build-var="VERSION=${VERSION}" 

# if the build command failed, log the failure and run time, then exit
if [ $? -ne 0 ]; then
  echo ""
  echo "Build failed after $(format_time $SECONDS)"
  echo ""
  exit 1
fi

# create a CRC32 checksum of the file and append it to the filename
if [ "$CRC" = true ]; then
  echo "calculating CRC..."
  cp ./build/fw.zip ./build/fw-temp.zip
  ./lib/gz/gz_mos_libs/gz_mos_shared_libs/gz_shared_libraries/gz_build_tools/hex_combiner.sh \
    -c ./build/fw-temp.zip \
    -o ./build/wmu-v${VERSION}.zip
fi

cp ./build/fw.zip ${OUTPUT_FILE}

echo ""
echo "Build took $(format_time $SECONDS)"
echo ""
