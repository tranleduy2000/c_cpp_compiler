#!/bin/bash

## Install gcc 4.9
sudo apt-get install python-software-properties
yes | sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install gcc-4.9 -y
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.9 50
sudo apt-get install g++-4.9 -y
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.9 50