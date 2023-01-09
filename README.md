# lwmf
lightweight media framework - graphics, audio, math, multithreading, logging and much more for games, demos, engines etc.

Windows 64bit, C++ 17 (and later), Visual Studio 2022, OpenGL 4.5 Core (Shader & Direct State Access), SSE 4.2

This framework is "Public Domain" - do whatever you want with it!

Features:

	- OpenGL
		- window creation / create OpenGL context
		- wgl extension loader
		- shader class
		- load textures into GPU
		- render textures to screen
	
	- Primitives
		- SetPixel / GetPixel
		- SetPixelSafe / GetPixelSafe (with boundary checks)
		- Scanline Fill (stack-implementation)
		- Line (EFLA)
		- Anti-aliased line (Xiaolin Wu´s algorithm)
		- Rectangle
		- Filled Rectangle
		- Circle 
		- Filled Circle
		- Ellipse
		- Polygon
		- Filled Polyon
		- Clear PixelBuffer
		
	- Color handling
		- RGBA to INT
		- INT to RGBA
		- color blending (SSE/SIMD-accelerated)
		- color shading
  
	- Images
		- load 24bit BMP (Bitmap) to texture
		- load PNG to texture
  
	- Textures
		- create
		- resize
		- rotate
		- scale
		- crop
		- blitting
		- transparent blitting of full textures or only parts of it
  
	- Audio (still a lot of work!)
		- load and play MPEG1 MP3 (Samplerate, Bitrate and Number of Channels will be detected)
		- get length and current position of MP3
		- pause / restart
		- set volume
		
	- Gamepad
		- support for XBOX 360 controllers via Microsoft XInput
		
	- Math
		- Lerp (linear interpolation)
		- Euclidian distance
		- Chebyshev distance
		- Manhattan distance
		- fast atan2 approximation
		- XorShift32 (Random Number Generator)
  
	- Text
		- simple 8x8 hardcoded font, standard ASCII
  
	- Procedural
		- Perlin noise generator
  
	- Multithreading
		- dynamic threadpool
	
	- Raw Input Devices
		- register/unregister mouse and keyboard
		- catch mouse in current window

	- Logging and error handling
		- create logfile via class (one logfile is hardcoded in lwmf itself)
		- error/exception handling
		- switchable exception throwing

	- INI files
		- read and write values from/to standard INI files

	- Other stuff
		- fps (frames per second) counter

**My intention for this library**

  - lightweight/small/minimal
  - modern (C++ 17 and OpenGL shader)
  - really easy to use
  - fast
  - robust
  
  Though minimal and lightweight, there is extensive error checking, logging, exception handling and boundary checks.
 
**How to use?**

**Just #include "lwmf.hpp" in your code an go! No external libraries, no DLL, no bullshit.**

Have a look at the example, which draws some nice and smooth animated demo effects like Metaballs, raytracing, realtime plasma, a dotted tunnel, a particle swarm and much more!
There is also a number of throughput tests of various functions.
You can download "Release.zip" and run the included executable!

Here are some screenshots from the demo:

![DEMO_PIC1](https://github.com/StefanKubsch/lwmf/blob/master/Documentation/Screenshots/Demo1.png)

![DEMO_PIC2](https://github.com/StefanKubsch/lwmf/blob/master/Documentation/Screenshots/Demo2.png)

![DEMO_PIC3](https://github.com/StefanKubsch/lwmf/blob/master/Documentation/Screenshots/Demo3.png)

I´m using lwmf in my own game engine "NARC" - see https://github.com/StefanKubsch/NARC
