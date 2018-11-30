# TS3Tunnel

TS3Tunnel is a proof of concept showing how easy it is to **spy on the private communications of the millions of TeamSpeak 3 users** without anyone noticing it. This PoC aims **to stress the urgent need for _end-to-end encryption_ as a default**.

> "_TeamSpeak is a proprietary voice-over-Internet Protocol (VoIP) application for audio communication between users on a chat channel, much like a telephone conference call._"
> 
> Source: [https://en.wikipedia.org/wiki/TeamSpeak](https://en.wikipedia.org/wiki/TeamSpeak)


## How it works

### TS3 decentralized infrastructure

TeamSpeak 3 works on a **decentralized infrastructure**: it means that _anyone_ can host their own server. People can then connect to the server using its public IP address.

The problem is, **users' voice data transit through these servers _unencrypted_**. Thus, it becomes very easy for the server provider **to capture the network traffic** and eavesdrop on _any_ conversation going on on their server. Actually, not only the server provider can do that, but also anyone having access to the network traffic between the TS3 client and server.


### Extracting the voice data

The captured network traffic must be **reprocessed** to extract only the voice data and produce a **working audio stream**: this PoC produces a **PCM audio stream**. It is trivially done by inspecting several samples of captured network traffic between a TS3 client and server.

The packets basically contain metadata such as:
* The **CODEC** used to compress/decompress the voice data ([Speex](https://www.speex.org/), [Opus](http://opus-codec.org/), [CELT](http://celt-codec.org/)): this PoC only supports the Opus CODEC (it is the default CODEC used by most TeamSpeak 3 servers) but it would be easy to add other CODECs
* Session ID
* Packet ID
* etc.


### PoC Setup

This PoC is made up of two parts:
1. The "_server_", which must be running on the same machine as the TS3 server (for the sake of simplicity): it **captures the network traffic** intended for the TS3 server and **sends it to**...
2. ... one or more "_clients_": it is a GUI program which **plays back the audio communications live** or **save them to a file**. It is possible to choose which sessions (i.e. users) to eavesdrop.


## Conclusion

This PoC shows it is trivial for anyone having access to the network traffic between a TS3 client and server (e.g. TS3 server providers, ISPs, anyone in your house having access to the home router) to record private communications, or even listen to them as they are happening. Furthermore, this can be done massively without anyone noticing it.

To enhance privacy and avoid any abuse, **we need end-to-end encryption as a default**.


## Compiling

### Server on Linux

1. Install dependencies and tools: `apt-get install g++ make qtdeclarative5-dev qt5-default libpcap-dev`
2. Generate Makefile using `qmake`: `qmake TS3TunnelServer.pro`
3. Compile: `make`
4. Run **as root** (needed for packets capture)

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
