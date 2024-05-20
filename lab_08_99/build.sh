#!/bin/bash

if [ ! -d "build" ]; then
  mkdir "build"
fi

( cd "./build" && cmake -DCMAKE_BUILD_TYPE="$1" -DCMAKE_RULE_MESSAGES=OFF .. && cmake --build . --target "$1")
if [ $1 == "release" ]; then
  if [ ! -d "out" ]; then
    mkdir "out"
  fi
  cp "./build/$1" "./out/$1.exe"
fi
