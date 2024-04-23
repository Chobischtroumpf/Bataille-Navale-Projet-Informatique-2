#!/bin/bash

packages=("libcpprest-dev" "qt6-base-dev" "libsodium-dev")

for package in $packages; do 
    rm -rf "output.txt"
    touch "output.txt"
    apt-cache depends --recurse --no-recommends --no-suggests --no-conflicts --no-breaks --no-replaces --no-enhances $package | grep "^\w" | sort -u >> output.txt
done

output=$(xargs apt list < output.txt | grep -v "installed" | grep -v "upgradable" | awk '{print $1}' | awk -F'/' '{print $1}' | sed 's/Listing...//')

echo $output > output.txt
xargs apt-get download  < output.txt