#!/usr/bin/env bash

sudo apt update

# This script is used to set up the SDL2 library for the project.
sudo apt install libsdl2-dev libsdl2-2.0-0 -y 

# This script is used to set up the SDL2_image library for the project.
sudo apt install libjpeg-dev libwebp-dev libtiff5-dev libsdl2-image-dev libsdl2-image-2.0-0 -y

# This script is used to set up the SDL2_ttf library for the project.
sudo apt install libfreetype6-dev libsdl2-ttf-dev libsdl2-ttf-2.0-0 -y