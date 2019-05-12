# lwmf
lightweight media framework - C++ OpenGL/graphics and media lib for windows

You need a simple header-only library which allows you to draw pixel-based graphics and media in Windows, but in a better, easier and more performant way than using GDI+ ?

With lwmf, you can easily:

  - create a resizeable window or fullscreen mode
  - create an OpenGL context
  - load OpenGL extensions (no need for glew or glad!)
  - use vsync, or not
  - setup some vertex and fragment shaders
  - use primitives
  - copy a pixelbuffer to an OpenGL texture
  - and render it to the window

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
  
Color handling:

  - RGBA to INT
  - INT to RGBA
  
Images:

  - load 24bit BMP (Bitmap)
  - resize/scale/crop BMP
  - blit / transparent blit
  
Text:

  - simple 8x8 hardcoded bitmap font, standard ASCII
  
Procedural:

  - Perlin noise generator
  
Other stuff:

  - fps counter

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
