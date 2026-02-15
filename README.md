# N-body problem simulator

### project developers:
Stephen Thorsell \
Xiaohua Liu \
Shahzaib Memon  

This guide will help you set up the repository from scratch. I used a clean install of WSL with
Ubuntu 24.04 installed to develop this project. Currently there are two ways to compile and run the 
program, mainly because of ongoing environment reconsiliation between developers in regards to our
graphics library SFML. There is a compilation path that uses SFML 2.6.1 which is the following:

## SFML 2.6.1 guide: 

For the sfml_2_6_1_demo.cpp to work type in the following commands:
```
    sudo apt update 

    sudo apt install g++ cmake make    # just in case you have a totally clean install of WSL like me

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

    sudo apt install libsfml-dev     # this is the graphics library that will install the 2.6.1 version of your code (assuming you're running Ubuntu 24.04)

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
or if you sourced our alias.sh file...
```
    ms
```




