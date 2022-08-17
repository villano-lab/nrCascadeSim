#!/bin/bash

echo "OS Type: $OSTYPE."
if [[ "$OSTYPE" == "darwin"* ]]; then

    # Install some custom requirements on macOS
    # e.g. brew install pyenv-virtualenv
    brew search llvm
    if [[ "$OSTYPE" == "darwin20" ]]; then 
        sw_vers
        brew install llvm@12
        export PATH="/usr/local/opt/llvm@12/bin:$PATH"
        #sudo rm -rf /Library/Developer/CommandLineTools
        #xcode-select --install
        xcode-select -p
        xcode-select -p | ls
        ls /Library/Developer/CommandLineTools/Packages
        open /Library/Developer/CommandLineTools/Packages/macOS_SDK_headers_for_macOS_11.6.pkg
    fi
    clang --version

    brew install md5sha1sum
    wget https://repo.anaconda.com/miniconda/Miniconda3-latest-MacOSX-x86_64.sh -O ~/miniconda.sh
    bash ~/miniconda.sh -b -p $HOME/miniconda
    export PATH="$HOME/miniconda/bin:$PATH"
    hash -r

    #get codecov program
    curl -Os https://uploader.codecov.io/latest/macos/codecov

    chmod +x codecov


    case "${TOXENV}" in
        py32)
            # Install some custom Python 3.2 requirements on macOS
            ;;
        py33)
            # Install some custom Python 3.3 requirements on macOS
            ;;
    esac
else
    # Install some custom requirements on Linux
    #get apt-get and conda
    sudo apt-get update || true
    sleep 10
    sudo apt-get install make cmake g++ gcc binutils libx11-dev libxpm-dev libxft-dev libxext-dev openssl libssl-dev
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
