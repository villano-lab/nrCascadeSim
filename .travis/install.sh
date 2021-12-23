#!/bin/bash

if [ $TRAVIS_OS_NAME = 'osx' ]; then

    # Install some custom requirements on macOS
    # e.g. brew install pyenv-virtualenv
    wget https://repo.anaconda.com/miniconda/Miniconda3-latest-MacOSX-x86_64.sh -O ~/miniconda.sh
    bash ~/miniconda.sh -b -p $HOME/miniconda
    export PATH="$HOME/miniconda/bin:$PATH"
    hash -r


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

    conda config --set always_yes yes --set changeps1 no
    conda update -q conda || true
    
    # Useful for debugging any issues with conda
    conda info -a
    
    #set up  conda environment
    #conda env create -n test-environment python=$TRAVIS_PYTHON_VERSION -f nrCascadeSim_env.yml 
    #source activate test-environment
    conda create -n piptest pip
    source activate piptest
    pip install -r example-usecase/requirements.txt
    conda list papermill
    #activate python env and continue testing


fi
echo "MOTHER"
