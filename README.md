# dynammo

Isometric game written in C++ using
- openSSL
- openGL
- gtk
- "network" wrapper (https://github.com/domso/network)

The project is divided into 3 sub-projects:
- gameServer
- gameClient
- loginServer
  - This project comes from early versions and will be removed/replaced/rewritten someday

The current goal is to develop a "minimal viable product" with the following features:
- simple UI
- user-authentication
- moving a sprite-character
- region-layer with some static objects(e.g. trees)
- multiplayer

Currently all ressources (images, textures, etc.) are NOT provided by this repository.
