#!/usr/bin/env bash
set -e

#apt update && apt install -y socat xorg 

# Prepare target env
CONTAINER_DISPLAY="0"
CONTAINER_HOSTNAME="xterm"
IMAGE_NAME=c766397c6d26

# Create a directory for the socket
rm -Rf ./display
mkdir -p display/socket
touch display/Xauthority

# Get the DISPLAY slot
DISPLAY_NUMBER=$(echo $DISPLAY | cut -d. -f1 | cut -d: -f2)

# Extract current authentication cookie
AUTH_COOKIE=$(xauth list | grep "^$(hostname)/unix:${DISPLAY_NUMBER} " | awk '{print $3}')

# Create the new X Authority file
xauth -f display/Xauthority add ${CONTAINER_HOSTNAME}/unix:${CONTAINER_DISPLAY} MIT-MAGIC-COOKIE-1 ${AUTH_COOKIE}

# Proxy with the :0 DISPLAY
socat UNIX-LISTEN:display/socket/X${CONTAINER_DISPLAY},fork TCP4:localhost:60${DISPLAY_NUMBER} &
socat_proc=$!
echo $socat_proc

# Launch the container
docker run -it --rm \
  --gpus all \
  -e DISPLAY=:${CONTAINER_DISPLAY} \
  -e XAUTHORITY=/tmp/.Xauthority \
  --network=host \
  --name=gui_container \
  -v ${PWD}/display/socket:/tmp/.X11-unix \
  -v ${PWD}/display/Xauthority:/tmp/.Xauthority \
  -v ${HOME}/.ssh:/root/.ssh \
  -v ${HOME}/work:/home/ \
  --hostname ${CONTAINER_HOSTNAME} \
  $IMAGE_NAME /bin/bash

kill -15 $socat_proc

