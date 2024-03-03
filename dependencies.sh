#!/bin/bash

which apt >/dev/null 2>&1
if [ $? -eq 0 ]
then

# Updating the package list
echo "Updating the package list..."
sudo apt-get update

# Check if cpprestsdk is installed
if ! dpkg -s libcpprest-dev &> /dev/null; then
    echo "cpprestsdk not found. Attempting to install..."

    # Attempt to install cpprestsdk
    sudo apt-get install -y libcpprest-dev

    if [ $? -eq 0 ]; then
        echo "cpprestsdk installed successfully."
    else
        echo "Failed to install cpprestsdk. Please install it manually."
    fi
else
    echo "cpprestsdk is already installed."
fi

# Install SQLite3 SDK
if ! dpkg -s libsqlite3-dev &> /dev/null; then
    echo "SQLite3 SDK not found. Attempting to install..."

    sudo apt-get install -y libsqlite3-dev

    if [ $? -eq 0 ]; then
        echo "SQLite3 installed successfully."
    else
        echo "Failed to install SQLite3. Please install it manually."
    fi
else
    echo "SQLite3 is already installed."
fi

# Install CMake
if ! dpkg -s cmake &> /dev/null; then
    echo "cmake not found. Attempting to install..."

    sudo apt-get install -y cmake

    if [ $? -eq 0 ]; then
        echo "cmake installed successfully."
    else
        echo "Failed to install cmake. Please install it manually."
    fi
else
    echo "cmake is already installed."
fi

# Install Sodium
if ! dpkg -s libsodium-dev &> /dev/null; then
    echo "Sodium lib not found. Attempting to install..."

    sudo apt-get install -y libsodium-dev

    if [ $? -eq 0 ]; then
        echo "Sodium installed successfully."
    else
        echo "Failed to install sodium. Please install it manually."
    fi
else
    echo "sodium is already installed."
fi

else
    echo "This script is only for Ubuntu/Debian based systems."
    exit 1
fi