# dynammo

Isometric game written in C++ using
- openSSL
- openGL
- gtk
- "network" wrapper (https://github.com/domso/network)
- "mapgen" map-generator (https://github.com/domso/mapgen)
- "riscv_sim" Risc-V Simulator (https://github.com/domso/riscv_sim)
- "riscv_asm" Risc-V Assembler (https://github.com/domso/riscv_asm)

Project Structure:
- general
    - Shared code between Server and Client; mostly type-definitions and generic wrappers/helpers
- server
- client

Current State:
- working account-system with creation and login
- moveable character
- working chat

Next goal:
- build roadway

Screenshots:
<p float="left">
  <img src="/client/res/Screenshots/full.png" width="45%"/>
  <img src="/client/res/Screenshots/small.png" width="45%"/>
  <img src="/client/res/Screenshots/water.png" width="45%"/>
  <img src="/client/res/Screenshots/chat.png" width="45%"/>
  <img src="/client/res/Screenshots/close.png" width="45%"/>
</p>

Usage:
- Start the server in the root-path of the server-directory. (./bin/server)
- Start the client in the build-path of the client-directory. (./client)
- Create a new account
- Login with the new account
- Use wasd to move your character. (He is in the upper left corner of the map)
- Use WASD to move the camera.
- Use the mouse-wheel to zoom in/out.
- Press ENTER to open the chat box

Build:
```
sudo apt-get install g++ git cmake libssl-dev libgtkmm-3.0-dev
git clone https://github.com/domso/network
git clone https://github.com/domso/riscv_asm
git clone https://github.com/domso/riscv_sim
git clone https://github.com/domso/dynammo

cd dynammo
mkdir build
cd build
cmake ..
make
```

