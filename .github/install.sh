#!/bin/bash

echo "OS Type: $OSTYPE."
if [[ "$OSTYPE" == "darwin"* ]]; then

    # Install some custom requirements on macOS
    # e.g. brew install pyenv-virtualenv
    #brew install tree
    if [[ "$OSTYPE" == "darwin20" ]]; then 
        export OLDXCODE="`xcode-select -p`"
        #sudo rm -rf /Library/Developer/CommandLineTools
        #xcode-select --install
        export PROCESS="`ps aux | grep xcode | awk '{ print $2 }'`"
        sw_vers
        #brew install llvm@12
        while [ -n "`ps aux | grep xcode | grep -v grep`" ];
        do
            sleep 1;
        done
        #export PATH="/usr/local/opt/llvm@12/bin:$PATH"
        #export LDFLAGS="-L/usr/local/opt/llvm@12/lib"
        #export CPPFLAGS="-I/usr/local/opt/llvm@12/include"
        #xcode-select -p
        #xcode-select -p | ls
        #ls /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/
        #ls /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/
        #ls /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/
        #tree /Library/Developer/CommandLineTools/SDKs/Macosx12.1.sdk
        #open /Library/Developer/CommandLineTools/Packages/macOS_SDK_headers_for_macOS_11.6.pkg
    fi
    clang --version

    echo "Installing md5sha1sum..."
    brew install md5sha1sum
    echo "Installing miniconda..."
    wget https://repo.anaconda.com/miniconda/Miniconda3-latest-MacOSX-x86_64.sh -O ~/miniconda.sh
    bash ~/miniconda.sh -b -p $HOME/miniconda
    export PATH="$HOME/miniconda/bin:$PATH"
    hash -r

    #get codecov program
    #curl -Os https://uploader.codecov.io/latest/macos/codecov
    #chmod +x codecov

else
    # Install some custom requirements on Linux
    #get apt-get and conda
    sudo apt-get update -y || true
    sleep 10
    sudo apt-get install -y make cmake g++ gcc binutils libx11-dev libxpm-dev libxft-dev libxext-dev openssl libssl-dev curl
    sleep 10
    wget https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh -O miniconda.sh;
    bash miniconda.sh -b -p $HOME/miniconda
    export PATH="$HOME/miniconda/bin:$PATH"
    hash -r

    #get codecov program
    curl -Os https://uploader.codecov.io/latest/linux/codecov

    chmod +x codecov

    #conda config --set always_yes yes --set changeps1 no
    #conda update -q conda || true
    #
    ## Useful for debugging any issues with conda
    #conda info -a
    #
    ##set up  conda environment
    #conda create -n piptest pip
    #source activate piptest
    #pip install -r example-usecase/requirements.txt
    #conda list papermill


fi
conda config --set always_yes yes --set changeps1 no
conda update -q conda || true
   
# Useful for debugging any issues with conda
conda info -a
    
#set up  conda environment
conda create -n piptest pip
source activate piptest
pip install -r example-usecase/requirements.txt
conda list papermill
