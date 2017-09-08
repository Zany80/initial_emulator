#!/bin/bash
pushd $HOME
if [ ! -d "./SFML" ]
then
	git clone https://github.com/SFML/SFML
else
	pushd SFML
	git pull
	popd
fi
cd SFML
mkdir -p build
cd build
cmake ..
make -j6
make install
cd ../..
if [ ! -d "./TGUI" ]
then
	git clone https://github.com/texus/TGUI
else
	pushd TGUI
	git pull
	popd
fi
cd TGUI
mkdir -p build
cd build
cmake ..
make -j6
make install
popd
