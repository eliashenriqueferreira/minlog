#!/usr/bin/bash
docker run --rm -v "$(pwd)/../src:/usr/local/minlog/src" -v "$(pwd)/../inc:/usr/local/minlog/inc" -v "$(pwd)/../docker:/usr/local/minlog/docker" -w /usr/local/minlog gcc:7 make -C docker $1
