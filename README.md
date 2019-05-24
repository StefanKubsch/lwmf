# lwmf
lightweight media framework - graphics, math, multithreading and more for games and gfx

With lwmf, you can easily:

  - create windowed or fullscreen, OpenGL-accelerated games or demos
  - use primitives, load and manipulate bitmaps/textures
  - run everything multithreaded
  - and more

Inplemented primitives:

  - SetPixel / GetPixel
  - SetPixelSafe (with boundary checks)
  - Flood/Boundary Fill (stack-implementation)
  - Line
  - Rectangle
  - Filled Rectangle
  - Circle 
  - Filled Circle
  - Polygon
  - Filled Polyon
  - Clear PixelBuffer

OpenGL:

  - load OpenGL extensions
  - set up shaders
  - render textures to screen
  
Color handling:

  - RGBA to INT
  - INT to RGBA
  
Images:

  - load 24bit BMP (Bitmap) into texture
  - load PNG into textures
  
Textures:
  - resize/scale/crop
  - blit / transparent blit
  
Text:

  - simple 8x8 hardcoded bitmap font, standard ASCII
  
Procedural:

  - Perlin noise generator
  
Other stuff:

  - fps counter
  - graphics related math functions
  - a multithread class with a threadpool
  - raw input device handling (mouse and keyboard)

My intention of this library:

  - lightweight/small/minimal
  - modern (C++ 17 and OpenGL shader)
  - really easy to use
  - fast
  - robust
  

How to use?

**Just #include "lwmf.hpp" in your code an go! No external libraries, no DLL, no bullshit.**

Have a look at the example, which draws some nice and smooth animated demo effects like Metaballs, realtime plasma, a dotted tunnel, a particle swarm and more! 
You can download "Release.zip" and run the included executable!

Also, I´m using lwmf in my own game engine "NARC" - see https://github.com/StefanKubsch/NARC
