#!/bin/bash

rm -f pixi.lock pixi.toml
rm -rf .pixi
pixi init . -c https://conda.modular.com/max-nightly/ -c conda-forge
pixi add modular mojo
pixi config set --local run-post-link-scripts insecure
