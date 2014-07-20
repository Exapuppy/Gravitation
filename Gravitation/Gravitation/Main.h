//////////////////////////////////////////////////////////////////////////////////////
// Подключение необходимых библиотек и объявление глобальных переменных для OpenGL	//
//////////////////////////////////////////////////////////////////////////////////////

# include <windows.h>
# include <stdio.h>
# include <math.h>
# include <time.h>
# include <Mmsystem.h>
# include <GL\GL.H>
# include <GL\GLU.H>

# pragma comment ( lib, "winmm.lib" )

# define SCREEN_WIDTH	800
# define SCREEN_HEIGHT	600
# define SCREEN_DEPTH	16

extern LPRECT hWndRect;

extern HWND		 g_hWnd;
extern RECT		 g_rRect;
extern HDC		 g_hDC;
extern HGLRC	 g_hRC;
extern HINSTANCE g_hInstance;

extern HWND		 g_hWnd_Graph1;
extern RECT		 g_rRect_Graph1;
extern HDC		 g_hDC_Graph1;
extern HGLRC	 g_hRC_Graph1;

extern HWND		 g_hWnd_Graph2;
extern RECT		 g_rRect_Graph2;
extern HDC		 g_hDC_Graph2;
extern HGLRC	 g_hRC_Graph2;

extern HWND		 g_hWnd_Graph3;
extern RECT		 g_rRect_Graph3;
extern HDC		 g_hDC_Graph3;
extern HGLRC	 g_hRC_Graph3;

extern HWND		 g_hWnd_Graph4;
extern RECT		 g_rRect_Graph4;
extern HDC		 g_hDC_Graph4;
extern HGLRC	 g_hRC_Graph4;

LRESULT CALLBACK WndProc3 ( HWND hwnd, UINT mesage, WPARAM wParam, LPARAM lParam );