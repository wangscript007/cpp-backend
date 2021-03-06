#!/usr/bin/env bash

EXTERNAL_DIR="external"

mkdir -p ${EXTERNAL_DIR}
mkdir -p modules/manbtc/${EXTERNAL_DIR}

if [ ! -d ${EXTERNAL_DIR}/libbtc ]; then
    echo "Setting up libbtc..."
    cd ${EXTERNAL_DIR}
    git clone https://github.com/libbtc/libbtc.git libbtc
    cd libbtc
    ./autogen.sh
    ./configure
    make check
    cd ../..
fi

#if [ ! -d modules/manbtc/${EXTERNAL_DIR}/bitcoin ]; then
#    echo "Setting up bitcoin..."
#    brew install -y automake berkeley-db4 libtool boost miniupnpc openssl pkg-config protobuf python qt libevent qrencode
#    cd modules/manbtc/${EXTERNAL_DIR}
#    git clone https://github.com/bitcoin/bitcoin.git bitcoin
#    cd bitcoin
#    ./autogen.sh
#    ./configure
#    make -j 4
#    cd ../../../..
#fi
