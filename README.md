# dynammo

Isometric game written in C++ using
- openSSL
- openGL
- gtk
- "network" wrapper (https://github.com/domso/network)
- "mapgen" map-generator (https://github.com/domso/mapgen)

Project Structure:
- general
    - Shared code between Server and Client; mostly type-definitions and generic wrappers/helpers
- gameServer
- gameClient

Current State Server:
- Authentication of an User by his RSA-Signature
- Sending of the heightmap and some random objects
- Executing requested actions like moving

Current State Client:
- Rendering the received heightmap, water  and sprites
- Allowing Viewport-Changes (Zoom, Window-Rescale, Moving)
- Requesting actions to the server.
- Overlay-UI
- Animated Sprites
- Account Creation

Next goal:
- Full refactor to improve the
- Build small test-village

Screenshots:
<p float="left">
  <img src="/gameClient/res/Screenshots/full.png" width="45%"/>
  <img src="/gameClient/res/Screenshots/small.png" width="45%"/>
  <img src="/gameClient/res/Screenshots/water.png" width="45%"/>
</p>

Usage:
- Start the gameServer in the root-path of the gameServer-directory. (./bin/gameServer)
- Start the gameClient in the build-path of the gameClient-directory. (./gameClient)
- Type 'test' into the username-field and click start
- Use wasd to move your character. (He is in the upper left corner of the map)
- Use WASD to move the camera.
- Use the mouse-wheel to zoom in/out.

Build:
```
mkdir build
cd build
cmake ..
make
```

