#!/bin/bash
# Serve build directory on port 8080
cd build && python3 -m http.server 8080
