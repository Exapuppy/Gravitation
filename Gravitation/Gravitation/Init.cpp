// Инициализация окон OpenGL	//
//////////////////////////////////

#include "stdafx.h"
#include "Main.h"

HWND CreateMyWindow (HWND hWndMut, LPCWSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen, HINSTANCE hInstance )
{
	HWND hWnd;
	WNDCLASS wndclass;
	
	memset ( &wndclass, 0, sizeof ( WNDCLASS ) );
	wndclass.style			= CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc	= WndProc3;
	wndclass.hIcon			= LoadIcon ( hInstance, MAKEINTRESOURCE(108) );
	wndclass.hCursor		= LoadCursor ( NULL, IDC_ARROW );
	wndclass.hbrBackground	= ( HBRUSH ) ( COLOR_WINDOW + 1 );
	wndclass.lpszClassName	= L"MyOpenWindow";
	RegisterClass ( &wndclass );

	g_hInstance = hInstance;

	RECT rWindow;
	rWindow.left	= 0;
	rWindow.right	= width;
	rWindow.top		= 0;
	rWindow.bottom	= height;

	AdjustWindowRect ( &rWindow, dwStyle, false );

	hWnd = CreateWindow ( L"MyOpenWindow", strWindowName, dwStyle, 0, 0, rWindow.right - rWindow.left, rWindow.bottom - rWindow.top, hWndMut, NULL, hInstance, NULL );

	if ( !hWnd )
		return NULL;

	//ShowWindow ( hWnd, SW_SHOWNORMAL );
	//UpdateWindow ( hWnd );

	return hWnd;
}

bool bSetupPixelFormat ( HDC hdc )
{
	PIXELFORMATDESCRIPTOR pfd;
	int pixelformat;

	pfd.nSize			= sizeof ( PIXELFORMATDESCRIPTOR );
	pfd.nVersion		= 1;
	pfd.dwFlags			= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.dwLayerMask		= PFD_MAIN_PLANE;
	pfd.iPixelType		= PFD_TYPE_RGBA;
	pfd.cColorBits		= SCREEN_DEPTH;
	pfd.cDepthBits		= SCREEN_DEPTH;
	pfd.cAccumBits		= 0;
	pfd.cStencilBits	= 0;

	if ( ( pixelformat = ChoosePixelFormat ( hdc, &pfd ) ) == false )
	{
		MessageBox ( NULL, L"ChosePixelFormat Failed", L"Error", MB_OK );
		return false;
	}

	if ( SetPixelFormat ( hdc, pixelformat, &pfd ) == false )
	{
		MessageBox ( NULL, L"SetupPixelFormat Failed", L"Error", MB_OK );
		return false;
	}
	return true;
}

void SizeOpenGLScreen ( int width, int height )
{
	if ( height == 0 )
		height = 1;
	glViewport ( 0, 0, width, height );
	glMatrixMode ( GL_PROJECTION );
	glLoadIdentity ( );
	gluPerspective ( 45.0f, (GLfloat)width / (GLfloat)height, .5f, 2000.0f );
	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity ( );
}

void SizeOpenGLScreen_Graph1 ( int width, int height )
{
	if ( height == 0 )
		height = 1;
	//glViewport ( 0, 0, width, height );
	glMatrixMode ( GL_PROJECTION );
	glLoadIdentity ( );	
	gluOrtho2D ( 0.0f, 600.0f, 380.0f, 0.0f );
	glViewport ( 0.0f, 0.0f, 680.0f, 420.0f );
	glDepthRange ( 0, 1 );
	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity ( );
}

void SizeOpenGLScreen_Graph2 ( int width, int height )
{
	if ( height == 0 )
		height = 1;
	glViewport ( 0, 0, width, height );
	glMatrixMode ( GL_PROJECTION );
	glLoadIdentity ( );	
	gluOrtho2D ( 0.0f, 600.0f, 450.0f, 0.0f );
	//glViewport ( 0.0f, 0.0f, 695.0f, 420.0f );
	glDepthRange ( 0, 1 );
	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity ( );
}

void SizeOpenGLScreen_Graph3 ( int width, int height )
{
	if ( height == 0 )
		height = 1;
	glViewport ( 0, 0, width, height );
	glMatrixMode ( GL_PROJECTION );
	glLoadIdentity ( );	
	gluOrtho2D ( 0.0f, 605.0f, 335.0f, 0.0f );
	//glViewport ( 0.0f, 0.0f, 680.0f, 300.0f );
	glDepthRange ( 0, 1 );
	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity ( );
}

void SizeOpenGLScreen_Graph4 ( int width, int height )
{
	if ( height == 0 )
		height = 1;
	glViewport ( 0, 0, width, height );
	glMatrixMode ( GL_PROJECTION );
	glLoadIdentity ( );	
	gluOrtho2D ( 0.0f, 605.0f, 215.0f, 0.0f );
	//glViewport ( 0.0f, 0.0f, 695.0f, 300.0f );
	glDepthRange ( 0, 1 );
	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity ( );
}

void InitializeOpenGL ( int width, int height )
{
	g_hDC = GetDC ( g_hWnd );

	if ( !bSetupPixelFormat ( g_hDC ) )
		PostQuitMessage ( 0 );

	g_hRC = wglCreateContext ( g_hDC );
	wglMakeCurrent ( g_hDC, g_hRC );
	glEnable ( GL_TEXTURE_2D );
	glEnable ( GL_DEPTH_TEST );

	SizeOpenGLScreen ( width, height );
}

void InitializeOpenGL_Graph1 ( int width, int height )
{
	g_hDC_Graph1 = GetDC ( g_hWnd_Graph1 );

	if ( !bSetupPixelFormat ( g_hDC_Graph1 ) )
		PostQuitMessage ( 0 );

	g_hRC_Graph1 = wglCreateContext ( g_hDC_Graph1 );
	wglMakeCurrent ( g_hDC_Graph1, g_hRC_Graph1 );	
	glEnable ( GL_DEPTH_TEST );

	SizeOpenGLScreen_Graph1 ( width, height );
}

void InitializeOpenGL_Graph2 ( int width, int height )
{
	g_hDC_Graph2 = GetDC ( g_hWnd_Graph2 );

	if ( !bSetupPixelFormat ( g_hDC_Graph2 ) )
		PostQuitMessage ( 0 );

	g_hRC_Graph2 = wglCreateContext ( g_hDC_Graph2 );
	wglMakeCurrent ( g_hDC_Graph2, g_hRC_Graph2 );
	glEnable ( GL_DEPTH_TEST );

	SizeOpenGLScreen_Graph2 ( width, height );
}

void InitializeOpenGL_Graph3 ( int width, int height )
{
	g_hDC_Graph3 = GetDC ( g_hWnd_Graph3 );

	if ( !bSetupPixelFormat ( g_hDC_Graph3 ) )
		PostQuitMessage ( 0 );

	g_hRC_Graph3 = wglCreateContext ( g_hDC_Graph3 );
	wglMakeCurrent ( g_hDC_Graph3, g_hRC_Graph3 );	
	glEnable ( GL_DEPTH_TEST );

	SizeOpenGLScreen_Graph3 ( width, height );
}

void InitializeOpenGL_Graph4 ( int width, int height )
{
	g_hDC_Graph4 = GetDC ( g_hWnd_Graph4 );

	if ( !bSetupPixelFormat ( g_hDC_Graph4 ) )
		PostQuitMessage ( 0 );

	g_hRC_Graph4 = wglCreateContext ( g_hDC_Graph4 );
	wglMakeCurrent ( g_hDC_Graph4, g_hRC_Graph4 );	
	glEnable ( GL_DEPTH_TEST );

	SizeOpenGLScreen_Graph4 ( width, height );
}

void DeInit ( )
{
	if ( g_hRC )
	{
		wglMakeCurrent ( NULL, NULL );
		wglDeleteContext ( g_hRC );
	}

	if ( g_hDC )
		ReleaseDC ( g_hWnd, g_hDC );

	UnregisterClass ( L"MyOpenWindow", g_hInstance );
	PostQuitMessage ( 0 );
}

void DeInit_Graph1 ( )
{
	if ( g_hRC_Graph1 )
	{
		wglMakeCurrent ( NULL, NULL );
		wglDeleteContext ( g_hRC_Graph1 );
	}

	if ( g_hDC_Graph1 )
		ReleaseDC ( g_hWnd_Graph1, g_hDC_Graph1 );	
}

void DeInit_Graph2 ( )
{
	if ( g_hRC_Graph2 )
	{
		wglMakeCurrent ( NULL, NULL );
		wglDeleteContext ( g_hRC_Graph2 );
	}

	if ( g_hDC_Graph2 )
		ReleaseDC ( g_hWnd_Graph2, g_hDC_Graph2 );	
}

void DeInit_Graph3 ( )
{
	if ( g_hRC_Graph3 )
	{
		wglMakeCurrent ( NULL, NULL );
		wglDeleteContext ( g_hRC_Graph3 );
	}

	if ( g_hDC_Graph3 )
		ReleaseDC ( g_hWnd_Graph3, g_hDC_Graph3 );	
}

void DeInit_Graph4 ( )
{
	if ( g_hRC_Graph4 )
	{
		wglMakeCurrent ( NULL, NULL );
		wglDeleteContext ( g_hRC_Graph4 );
	}

	if ( g_hDC_Graph4 )
		ReleaseDC ( g_hWnd_Graph4, g_hDC_Graph4 );	
}