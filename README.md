# TS3Tunnel

## Compiling

### Server on Linux

1. Install dependencies and tools: `apt-get install g++ make qtdeclarative5-dev qt5-default libpcap-dev`
2. Generate Makefile using `qmake`: `qmake TS3TunnelServer.pro`
3. Compile: `make`
4. Run **as root**

Tested with:
* Debian GNU/Linux 9.4 (stretch) AMD64
* GCC 6.3.0
* Qt 5.7.1
* libpcap 1.8.1


### Client on Windows

1. Install [Qt](https://www.qt.io/)
2. Install [PortAudio](http://portaudio.com/) using [vcpkg](https://github.com/Microsoft/vcpkg)
3. Compile using Visual Studio

Tested with:
* Windows 10 x64
* Visual Studio 2015 update 3 (compiling for x86)
* Qt 5.10.1
* PortAudio 19.0.6
