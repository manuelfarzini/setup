### ManPages
export MANPATH="/opt/local/share/man:$MANPATH"
export MANPAGER="less -FXRS"

### Go
export GOPATH=$HOME/.go

### Java
export JAVA_HOME=/Library/Java/JavaVirtualMachines/zulu-17.jdk/Contents/Home

### Mojo
export MODULAR_HOME=$HOME/.modular

### Matlab
export MATLAB_HOME=/Applications/MATLAB_R2024b.app

### Faust
export FAUST_TOOLS=/usr/local/share/faust
export FAUST_HH=/usr/local/include/faust

### Arduino
export ARDU_CORES=$HOME/Library/Arduino15/packages/arduino/hardware/avr/1.8.6/cores/arduino

### LLVM
export LLVMHOME=/opt/homebrew/opt/llvm

### Postgres
export POSTGRESHOME="/opt/homebrew/opt/postgresql@17"

### C++ 
# NOTE: should be commented out on GRAME machine
# export CXX=/opt/homebrew/opt/llvm/bin/clang++
# export CC=/opt/homebrew/opt/llvm/bin/clang

### Vulkan
export VULKAN_SDK=/Users/manuelfarzini/Personal/dev/opt/vulkan-sdk/1.4.304.0/macOS
export PATH=$VULKAN_SDK/bin:$PATH
export DYLD_LIBRARY_PATH=$VULKAN_SDK/lib:$DYLD_LIBRARY_PATH
export VK_ICD_FILENAMES=$VULKAN_SDK/share/vulkan/icd.d/MoltenVK_icd.json
export VK_LAYER_PATH=$VULKAN_SDK/share/vulkan/explicit_layer.d

### Personal
export PERS=$HOME/Personal/dev

### PATH setup
export PATH=$PATH:opt/local/bin:usr/local/sbin:/opt/local/sbin:/Library/Apple/usr/bin
export PATH=$PATH:$LLVMHOME/bin
export PATH=$PATH:$JAVA_HOME/bin
export PATH=$PATH:$MATLAB_HOME/bin
export PATH=$PATH:$(npm prefix -g)/bin
export PATH=$PATH:$MODULAR_HOME/pkg/packages.modular.com_mojo/bin
export PATH=$PATH:$POSTGRESHOME/bin
export PATH=$PATH:$PERS/bin
export PATH=$PATH:$PERS/script

### Rust
export PATH="$PATH:$HOME/.cargo/bin"

### Neovim
export PATH=$PATH:$HOME/.local/share/nvim/mason/bin

### UnrealEngine
export UEHOME=~/personal/dev/opt/unreal_engine/UE_5.6/Engine

### Dotnet
export PATH=$PATH:/usr/local/share/dotnet

### Pixi
export PATH="/Users/manuelfarzini/.pixi/bin:$PATH"
