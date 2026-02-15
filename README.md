# N-body problem simulator

### project developers:
Stephen Thorsell \
Xiaohua Liu \
Shahzaib Memon  

This guide will help you set up the repository from scratch. I used a clean install of WSL with
Ubuntu 24.04 installed to develop this project. Currently there are two ways to compile and run the 
program, mainly because of ongoing environment reconsiliation between developers in regards to our
graphics library SFML. The primary compilation path we will use is with SFML 3.0 that uses Cmake.
We are currently in a transition period between this and the depreciated SFML 2.6.1 version because
the differences in the developers' environments were discovered in the middle of the first sprint, 
so both compilation methods are described here.

## SFML 3.0 guide:
TODO: put detailed instructions here!

## SFML 2.6.1 guide: 
SFML 2.6.1 is the version that Ubuntu 24.04 automatically installs with the sudo-apt update command
so this is how you compile the 2.6.1 version of our feature 1 program with this version (assuming 
you are using Ubuntu 24.04 here).

For the sfml_2_6_1_demo.cpp to work type in the following commands:
```
    sudo apt update
```
Just in case you have a fresh install of WSL like me, install the C++ compiler (we are using gcc) and
the supporiting compilation packages
```
    sudo apt install g++ cmake make
```
Install SFML package dependencies:
```
    sudo apt install \
        libxrandr-dev \
        libxcursor-dev \
        libxi-dev \
        libudev-dev \
        libfreetype-dev \
        libflac-dev \
        libvorbis-dev \
        libgl1-mesa-dev \
        libegl1-mesa-dev \
        libfreetype-dev
```

Finally install the SFML graphics library (Ubuntu 24.04 will auto-install the 2.6.1 version)
```
    sudo apt install libsfml-dev
```

Now we can compile and run the code. All compilation and linking is done very simply with a Makefile.
In the root directory of this project simply call the "make" compilation command:
```
    make
```
Now you can simply run the test program:
```
    ./testProgram
```
In the terminal, enter the number of particles you want to generate and press enter. Once you are
done running the progam, delete all the compilation objects and files which you can just call the make command 
to do so:
```
    make spotless
```
or you can source our alias.sh file for less typing on your end...
```
    source bin/alias.sh
    ms                   # our custom alias for make spotless
```






