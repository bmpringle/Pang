#!/usr/bin/env bash
cd SFML/build
make -j8
cd -
cd pong/build
make -j8
cd -