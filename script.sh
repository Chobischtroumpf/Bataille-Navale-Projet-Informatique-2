#!/bin/bash

# Check if an argument is provided
if [ $# -eq 0 ]; then
    echo "Please provide a package name as an argument."
    exit 1
fi

# Get the package name from the argument
package_name=$1

# Look up dependencies for the package
dependencies=$(apt-cache depends --no-recommends --no-suggests --no-conflicts --no-breaks --no-replaces --no-enhances $package_name | grep Depends | awk '{print $2}')

# Download the package and its dependencies
apt-get download $package_name $dependencies
