#!/bin/bash


rm ./output.txt
touch ./output.txt

./download_deps.sh libcpprest-dev
./download_deps.sh qt6-base-dev
./download_deps.sh libsodium-dev

./extract_deps.sh

cp BoostDetectToolset-1.74.0.cmake ./usr/lib/x86_64-linux-gnu/cmake/BoostDetectToolset-1.74.0.cmake