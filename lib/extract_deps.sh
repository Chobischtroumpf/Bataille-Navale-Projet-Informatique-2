#!/bin/bash

packages="$(ls -1 | grep ".deb")"

for package in $packages; do
    echo "extracting $package"
    dpkg -x $package ./
done