# N-body problem simulator

### project developers:
Stephen Thorsell \
Xiaohua Liu \
Shahzaib Memon  

This guide will help you set up the repository from scratch. I used a clean install of WSL with
Ubuntu 24.04 installed to develop this project but CMake should work no matter your operating 
system.

## Available Features:
To make sure you are running the correct commit hash for each submission, we have tagged commit
commit hashes at the end of each sprint.

For feature 1 run: 
```
    git checkout Feature_1
```
for feature 2:
```
    git checkout Feature_2
```
and finally for the final project submission:
```
    git checkout Final_submission
```

## Compilation and Run guide:
This is the preferred method to compile the code because of the ease of using CMake no matter the 
development environment. This method uses the template SFML project repository here:
- [SFML template repository](https://github.com/SFML/cmake-sfml-project)

There are plenty of tutorials out there that help you run Cmake in your IDE of choice: 
   - [VS Code](https://code.visualstudio.com) via the [CMake extension](https://code.visualstudio.com/docs/cpp/cmake-linux)
   - [Visual Studio](https://docs.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=msvc-170)
   - [CLion](https://www.jetbrains.com/clion/features/cmake-support.html)
   - [Qt Creator](https://doc.qt.io/qtcreator/creator-project-cmake.html)

Compiling the program in an Ubuntu environment within the linux terminal is very straightforward as well.
First install all SFML dependencies:
```
    sudo apt update

    sudo apt install g++ cmake make   # just in case you have a fresh install of WSL like me

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
Next source our custom alias file that we have created
```
    source bin/alias.sh
```
And use our custom alias to compile the program
```
    cmCP    # acronym stands for "CMake Compile Program"
```
Then simply run the test project executable
```
    ./nBodyProject
```
