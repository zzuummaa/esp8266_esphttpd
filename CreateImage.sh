#!/bin/bash

CACHE_FILE="./CMakeFiles/CMakeBuildNumberCache.txt"

if [[ ! -f $CACHE_FILE ]]; then
    echo "Cache file not found in path $CACHE_FILE"
    FIRMWARE_VERSION=0.0.0
else
    FIRMWARE_VERSION=$(sed 's/;/./g' $CACHE_FILE)
fi

echo "FIRMWARE_VERSION: $FIRMWARE_VERSION"

if [ -z "$PROJECT_NAME" ]
then
      echo "Please set PROJECT_NAME variable for CreateImage.sh"
      exit 1;
fi

rm -f test_file/*
mkdir -p test_file
sha256sum $PROJECT_NAME.bin | cut -d ' ' -f1 > test_file/firmware.sha256
cp $PROJECT_NAME.bin test_file/firmware.bin
echo ${FIRMWARE_VERSION} > test_file/version