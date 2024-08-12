# ServerClientLib
It is a basic **Server-Client library**, instances of which you might find on the Internet in countless and various implementations. So here is one more) I need it for several projects, so I decided to make it a bit more universal and if you find it usefull in any kind of things you want to do, it is wonderfull. In src you would find `Server` and `Client` parts and `Server-` and `Client-main.cpp` with basic multiconnectional echo server. Merge requests are welcomed.
    Your's WR.

# Requirements
 - Any **linux distro** since I'm using linux sockets and not windows sockets here, because... Windows sucks(IMHO).

 - CMake
 - Make
 - g++ (c++*11* or higher)
 - Brains (Preferably yours)
    Well you need to be born with them

# First Launch
If you have just cloned the repo, you won't have any binaries and staff. You can build them with cmake
``` cmake
cmake -S . -B build &
cmake --build build
```
If you get some problems it is your problems. On my PC everything works.