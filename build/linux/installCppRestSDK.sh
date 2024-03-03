#!/bin/bash

# Check if cpprestsdk is installed
if ! dpkg -s libcpprest-dev &> /dev/null; then
    echo "cpprestsdk not found. Attempting to install..."

    # Update package lists
    sudo apt-get update

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