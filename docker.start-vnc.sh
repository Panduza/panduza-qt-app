#!/bin/bash

Xvfb :1 -screen 0 1280x1024x16 &
fluxbox &
x11vnc -forever &
