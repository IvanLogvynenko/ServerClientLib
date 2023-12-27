# ServerClientLib
It is a basic **Server-Client library**, instances of which you might find on the Internet in countless and various implementations. So here is one more) I need it for several projects, so I decided to make it a bit more universal and if you find it usefull in any kind of things you want to do, it is wonderfull. In src you would find `Server` and `Client` parts and `Server-` and `Client-main.cpp` with basic multiconnectional echo server. Merge requests are welcomed.
    Your's WR.

# Requirements
 - Any **linux distro** since I'm using linux sockets and not windows sockets here, because... Windows sucks(IMHO). I personaly
 recommend using [Fedora Workstation](https://fedoraproject.org/workstation/).

 - CMake/Make

    - Red Hat based: 
    ``` bash
    sudo dnf install cmake make
    ```

    - AArch based:
    ``` bash
    sudo pacman install cmake make
    ```

    - Debian based:  
    ``` bash
    sudo apt install cmake make
    ```

 - g++ (c++*11* or higher)
    Red Hat based: 
    ``` bash
    sudo dnf install gcc-c++
    ```

 - Brains (Preferably yours)
    Well you need to be born with them

# First Launch
If you have just cloned the repo, you won't have any binaries and staff. I've created a file `scripts/launch.sh`. 
``` bash
cd scripts
launch.sh
```
By running this file the project would be built. If you get some problems it is your problems. On my pc everything works.