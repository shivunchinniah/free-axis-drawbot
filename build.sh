#!/bin/bash

arduino-cli compile -b arduino:megaavr:nona4809 --build-property "compiler.cpp.extra_flags=\"-Wall\"" --clean