/*
********************************************************************
*                                                                  *
* lwmf_opengl - lightweight media framework - OpenGL library       *
*                                                                  *
* (C) 2019 - present by Stefan Kubsch                              *
*                                                                  *
********************************************************************
*/

#pragma once

#include <Windows.h>
#include <cstdint>
#include <vector>
#include <cstddef>
#include <algorithm>
#include <GL/gl.h>

#pragma comment (lib, "opengl32.lib")

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

namespace lwmf
{


	inline std::int_fast32_t FullscreenFlag{};
	inline GLuint TextureID{};

	inline HDC WindowHandle;
	inline HWND MainWindow;

	inline std::vector<std::int_fast32_t> PixelBuffer;
	inline std::int_fast32_t ViewportWidth{};
	inline std::int_fast32_t ViewportHeight{};
	inline std::int_fast32_t ViewportWidthMid{};
	inline std::int_fast32_t ViewportHeightMid{};

	// Stuff for OpenGL Loader
	constexpr std::int_fast32_t GL_ARRAY_BUFFER				{ 0x8892 };
	constexpr std::int_fast32_t GL_ELEMENT_ARRAY_BUFFER		{ 0x8893 };
	constexpr std::int_fast32_t GL_FRAGMENT_SHADER			{ 0x8B30 };
	constexpr std::int_fast32_t GL_STATIC_DRAW				{ 0x88E4 };
	constexpr std::int_fast32_t GL_VERTEX_SHADER			{ 0x8B31 };

	using GLchar = char;
	using GLsizeiptr = std::ptrdiff_t;
	using GLintptr = std::ptrdiff_t;

	#define OGLIST \
		OG(void,	glAttachShader,				GLuint program, GLuint shader) \
		OG(void,	glBindBuffer,				GLenum target, GLuint buffer) \
		OG(void,	glBindFragDataLocation,		GLuint program, GLuint colorNumber,	const char * name) \
		OG(void,	glBindVertexArray,			GLuint array) \
		OG(void,	glBufferData,				GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage) \
		OG(void,	glCompileShader,			GLuint shader) \
		OG(GLint,	glCreateProgram,			void) \
		OG(GLint,	glCreateShader,				GLenum type) \
		OG(void,	glDeleteShader,				GLuint shader) \
		OG(void,	glDetachShader,				GLuint program,	GLuint shader) \
		OG(void,	glEnableVertexAttribArray,	GLuint index) \
		OG(void,	glGenBuffers,				GLsizei n, GLuint *buffers) \
		OG(void,	glGenVertexArrays,			GLsizei n, GLuint *arrays) \
		OG(void,	glLinkProgram,				GLuint program) \
		OG(void,	glShaderSource,				GLuint shader, GLsizei count, const GLchar* const *string, const GLint *length) \
		OG(void,	glTexStorage2D,				GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height) \
		OG(void,	glUseProgram,				GLuint program) \
		OG(void,	glVertexAttribPointer,		GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer)

	#define OG(Return, Name, ...) typedef Return WINAPI Name##proc(__VA_ARGS__); extern Name##proc * Name;
		OGLIST
	#undef OG

	#define OG(Return, Name, ...) Name##proc * Name;
		OGLIST
	#undef OG

	//
	// Window & OpenGL context
	//

	inline void ResizeViewportAndPixelBuffer(const std::int_fast32_t Width, const std::int_fast32_t Height)
	{
		ViewportWidth = Width;
		ViewportHeight = Height;
		ViewportWidthMid = ViewportWidth >> 1;
		ViewportHeightMid = ViewportHeight >> 1;
		glViewport(0, 0, ViewportWidth, ViewportHeight);

		PixelBuffer.clear();
		PixelBuffer.shrink_to_fit();
		PixelBuffer.resize(static_cast<size_t>(ViewportWidth) * static_cast<size_t>(ViewportHeight), 0);
	}

	inline void CreateOpenGLWindow(const HINSTANCE hInstance, const std::int_fast32_t Width, const std::int_fast32_t Height, const LPCSTR WindowName, const bool Fullscreen)
	{
		// Create window

		WNDCLASS WindowClass{};
		WindowClass.lpfnWndProc = WndProc;
		WindowClass.hInstance = hInstance;
		WindowClass.lpszClassName = "lwmf";
		RegisterClass(&WindowClass);

		DWORD dwExStyle{ WS_EX_APPWINDOW | WS_EX_WINDOWEDGE };
		DWORD dwStyle{ WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN };

		if (Fullscreen)
		{
			DEVMODE ScreenSettings;
			memset(&ScreenSettings, 0, sizeof(ScreenSettings));
			ScreenSettings.dmSize = sizeof(ScreenSettings);
			ScreenSettings.dmPelsWidth = Width;
			ScreenSettings.dmPelsHeight = Height;
			ScreenSettings.dmBitsPerPel = 32;
			ScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

			if (ChangeDisplaySettings(&ScreenSettings, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL)
			{
				dwExStyle = WS_EX_APPWINDOW;
				dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
				ShowCursor(FALSE);

				FullscreenFlag = 1;
			}
		}

		RECT WinRect{ 0, 0, Width, Height };
		AdjustWindowRectEx(&WinRect, dwStyle, FALSE, dwExStyle);
		MainWindow = CreateWindowEx(dwExStyle, WindowClass.lpszClassName, WindowName, dwStyle, 0, 0, WinRect.right - WinRect.left, WinRect.bottom - WinRect.top, nullptr, nullptr, hInstance, nullptr);

		// Create OpenGL context

		const PIXELFORMATDESCRIPTOR PFD =
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA,
			32,
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			24,
			8,
			0,
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
		};

		WindowHandle = GetDC(MainWindow);
		SetPixelFormat(WindowHandle, ChoosePixelFormat(WindowHandle, &PFD), &PFD);
		wglMakeCurrent(WindowHandle, wglCreateContext(WindowHandle));
		ShowWindow(MainWindow, SW_SHOW);
		ResizeViewportAndPixelBuffer(Width, Height);
	}

	inline void ResizeOpenGLWindow()
	{
		RECT WinRect{};
		GetClientRect(MainWindow, &WinRect);
		ResizeViewportAndPixelBuffer(WinRect.right, WinRect.bottom);
	}

	//
	// OpenGL Loader & VSync
	//

	inline void InitOpenGLLoader()
	{
		const HINSTANCE OpenGLDLL{ LoadLibraryA("opengl32.dll") };
		typedef PROC WINAPI wglGetProcAddressproc(LPCSTR lpszProc);
		wglGetProcAddressproc* wglGetProcAddress{ reinterpret_cast<wglGetProcAddressproc*>(GetProcAddress(OpenGLDLL, "wglGetProcAddress")) };

		#define OG(Return, Name, ...) Name = (Name##proc *)wglGetProcAddress(#Name);
			OGLIST
		#undef OG
	}

	inline void SetVSync(const std::int_fast32_t Sync)
	{
		typedef PROC(WINAPI * PFNWGLSWAPINTERVALFARPROC)(std::int_fast32_t);
		PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT{ reinterpret_cast<PFNWGLSWAPINTERVALFARPROC>(wglGetProcAddress("wglSwapIntervalEXT")) };
		wglSwapIntervalEXT(Sync);
	}

	//
	// Shader
	//

	inline void InitShader()
	{
		constexpr GLfloat Vertices[]
		{
			//  Position		Texcoords
				-1.0F,  1.0F,	0.0F, 0.0F,		// Top-left
				 1.0F,  1.0F,	1.0F, 0.0F,		// Top-right
				 1.0F, -1.0F,	1.0F, 1.0F,		// Bottom-right
				-1.0F, -1.0F,	0.0F, 1.0F		// Bottom-left
		};

		constexpr GLint Elements[]
		{
			0, 1, 2,
			2, 3, 0
		};

		GLuint ElementBufferObject{};
		GLuint VertexBufferObject{};
		GLuint VertexArrayObject{};

		const GLchar* VertexShaderString[]{
			"#version 450 core\n"
			"layout (location = 0) in vec2 position;\n"
			"layout (location = 1) in vec2 texcoord;\n"
			"out vec2 Texcoord;\n"
			"void main()\n"
			"{\n"
			"Texcoord = texcoord;\n"
			"gl_Position = vec4(position, 0.0f, 1.0f);\n"
			"}"
		};

		const GLchar* FragmentShaderString[]{
			"#version 450 core\n"
			"in vec2 Texcoord;\n"
			"out vec4 outColor;\n"
			"uniform sampler2D Texture;\n"
			"void main()\n"
			"{\n"
			"outColor = texture2D(Texture, Texcoord);\n"
			"}"
		};

		// Create buffers
		glGenBuffers(1, &VertexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

		glGenVertexArrays(1, &VertexArrayObject);
		glBindVertexArray(VertexArrayObject);

		glGenBuffers(1, &ElementBufferObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Elements), Elements, GL_STATIC_DRAW);

		// Create and compile the vertex shader
		const GLint VertexShader{ glCreateShader(GL_VERTEX_SHADER) };
		glShaderSource(VertexShader, 1, VertexShaderString, nullptr);
		glCompileShader(VertexShader);

		// Create and compile the fragment shader
		const GLint FragmentShader{ glCreateShader(GL_FRAGMENT_SHADER) };
		glShaderSource(FragmentShader, 1, FragmentShaderString, nullptr);
		glCompileShader(FragmentShader);

		// Link the vertex and fragment shader std::int_fast32_to a shader program
		const GLint ShaderProgram{ glCreateProgram() };
		glAttachShader(ShaderProgram, VertexShader);
		glAttachShader(ShaderProgram, FragmentShader);
		glBindFragDataLocation(ShaderProgram, 0, "outColor");
		glLinkProgram(ShaderProgram);
		glUseProgram(ShaderProgram);

		// Specify the layout of the vertex data
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) << 2, nullptr);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) << 2, reinterpret_cast<GLvoid*>(sizeof(GLfloat) << 1)); //-V566

		// Detach & delete shader
		glDetachShader(ShaderProgram, FragmentShader);
		glDetachShader(ShaderProgram, VertexShader);
		glDeleteShader(FragmentShader);
		glDeleteShader(VertexShader);

		// Bind a texture
		glGenTextures(1, &TextureID);
		glBindTexture(GL_TEXTURE_2D, TextureID);

		if (FullscreenFlag == 1)
		{
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA, ViewportWidth, ViewportHeight);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glClearColor(0.0F, 0.0F, 0.0F, 0.0F);
	}

	//
	// Pixelbuffer
	//

	inline void RenderPixelBuffer()
	{
		switch (FullscreenFlag)
		{
			case 1:
			{
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, ViewportWidth, ViewportHeight, GL_RGBA, GL_UNSIGNED_BYTE, PixelBuffer.data());
				break;
			}
			case 0:
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ViewportWidth, ViewportHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, PixelBuffer.data());
				break;
			}
			default: {};
		}

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}

	inline void SwapPixelBuffer()
	{
		SwapBuffers(WindowHandle);
	}

	inline void ClearPixelBuffer(const std::int_fast32_t Color)
	{
		std::fill(PixelBuffer.begin(), PixelBuffer.end(), Color);
	}


} // namespace lwmf