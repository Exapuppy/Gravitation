# include "stdafx.h"
# include "Gravitation.h"

# include "Main.h"
# include "Camera.h"
# include "Font.h"

# include "Control.h"

#pragma comment(lib,"htmlhelp.lib")

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#pragma comment(lib,"comctl32.lib")

#define MAX_LOADSTRING 100

#pragma region Глобальные переменные
#pragma region other
HINSTANCE hInst;
TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];
MSG msg;

//////////////////////////////////////////////
// Основной класс, содержащий				//
// все данные о текущем состоянии модели.	//
// Начальные значения:						//
// t = 0;									//
// V = {10, 0};								//
// r = {10000, 0};							//
// M = 1000e21;								//
// m = 1000e20;								//
// R = 2000;								//
//////////////////////////////////////////////
Physics::System *sys;

//////////////////////////////////////////////
//	Базовое расстояние между центрами тел	//
//////////////////////////////////////////////
int r_norm_base = 100;

//////////////////////
// Шаг разбиения.	//
//////////////////////
double k = 1e-4;

//////////////////////////////////
// Скорость течения времени.	//
// область значений [ 0 .. 40 ]	//
//////////////////////////////////
int timespeed = 1;

// Во время создания контролов Edit системой отсылается сообщение.
// Этот массив предназначен для того, чтобы это сообщение не обрабатывалось.
bool beg[6] = { false, false, false, false, false, false, };

// если происходят изменения некоторых параметров, то change = true;
// т.е. происходило изменение. это для проверки в режиме паузы.
// также применяется при ситуации столкновения и выхода за пределы зрения.
bool change = false;

//////////////////////////////////////////
// Траектория.							//
// Сюда вносятся координаты астероида	//
// частотой каждые 200 итераций.		//
//////////////////////////////////////////
Physics::Vector * traectory;
int mass_volume = 0;
bool ShowTraect = false;

//////////////////////////////////////////////
// когда Go = true, происходит ход времени.	//
//////////////////////////////////////////////
bool Go = false;

//////////////////////////////////////
// флаги захвата мыши и клавиатуры.	//
//////////////////////////////////////
bool flag;
bool flag2;
HCURSOR hCursor;

#pragma endregion

control *CTRL;
LPRECT hWndRect = new tagRECT();
CTRL_INTERFACE *ctrl_interface = new CTRL_INTERFACE();
//////////////////
//OGL parameter	//
//////////////////
#pragma region OGL

//////////////////////////////
//	Фиксированные ракурсы.	//
//////////////////////////////
bool Return_to_Planet = false;
bool Return_to_Asteroid = false;
bool Return_to_Platz = false;

// переключатели. true - отображается, false - не отображается.

//Сетка
bool network = false;

//Вектор скорости
bool SpeedVect = true;

//Фоновая текстура
bool TextureCheck = false;

//Скорость камеры
GLfloat kSpeed = 50.0f;

CCamera g_Camera;
CFont *Font;

// Структура, реализующая загрузку и хранение текстур.
TextureCtrl *TEXTURECTRL;

//////////////////////
//	Для графиков	//
//////////////////////

//Счетчик для графика.
unsigned char Schetchik = 0;
int Graph1_size = 0;

double a_max;

//структура для графика
struct Graph 
{
	GLfloat elemX_01	[ 30000 ];
	GLfloat elemX_02	[ 30000 ];
	GLfloat elemX_03	[ 30000 ];
	GLfloat elemX_04	[ 30000 ];
	GLfloat elemY		[ 30000 ];
} graph;
int u;
//

int		CoefGraph03_01 = 1;
int		CoefGraph03_02 = 1;
double	CoefGraph03_03 = 1.0f;

float alpha = 0.0f;

bool Graph1Open = false;
bool Graph2Open = false;
bool Graph3Open = false;
bool Graph4Open = false;
///////////////////////

GLUquadricObj *qobj;
GLfloat rotPlanet = 0.0f;
AUX_RGBImageRec *photo_image;

HWND		g_hWnd;
RECT		g_rRect;
HDC			g_hDC;
HGLRC		g_hRC;
HINSTANCE	g_hInstance;

//Grafic1
HWND		g_hWnd_Graph1;
RECT		g_rRect_Graph1;
HDC			g_hDC_Graph1;
HGLRC		g_hRC_Graph1;

//Grafic2
HWND		g_hWnd_Graph2;
RECT		g_rRect_Graph2;
HDC			g_hDC_Graph2;
HGLRC		g_hRC_Graph2;

//Grafic3
HWND		g_hWnd_Graph3;
RECT		g_rRect_Graph3;
HDC			g_hDC_Graph3;
HGLRC		g_hRC_Graph3;

//Grafic4
HWND		g_hWnd_Graph4;
RECT		g_rRect_Graph4;
HDC			g_hDC_Graph4;
HGLRC		g_hRC_Graph4;

#pragma endregion

//////////////////////////////////////////////////////
// Дескрипторы окон дочерних элементов управления	//
//////////////////////////////////////////////////////
#pragma region hwnd

//Logo
HWND hWndLogo			= NULL;

//Главное окно
HWND hWnd				= NULL;

// графики
HWND hWndGrafic1		= NULL;
HWND hWndGrafic2		= NULL;
HWND hWndGrafic3		= NULL;
HWND hWndGrafic4		= NULL;
HWND hWndGraficOGL1		= NULL;
HWND hWndGraficOGL2		= NULL;
HWND hWndGraficOGL3		= NULL;
HWND hWndGraficOGL4		= NULL;

//OGL
HWND hWndDraw			= NULL;
#pragma endregion

#pragma endregion
#pragma region Прототипы функций
//////////////////////////
// Function prototypes	//
//////////////////////////
ATOM				MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK	WndProc (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	WndProc2(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	WndProc4(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
WPARAM				MainLoop ( );
//////////////////////////

//////////////////////////////
// OGL function prototypes	//
//////////////////////////////
HWND CreateMyWindow (HWND, LPCWSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen, HINSTANCE hInstance );
bool bSetupPixelFormat ( HDC hdc );

void SizeOpenGLScreen ( int width, int height );
void SizeOpenGLScreen_Graph1 ( int width, int height );
void SizeOpenGLScreen_Graph2 ( int width, int height );
void SizeOpenGLScreen_Graph3 ( int width, int height );
void SizeOpenGLScreen_Graph4 ( int width, int height );

void InitializeOpenGL ( int width, int height );
void InitializeOpenGL_Graph1 ( int width, int height );
void InitializeOpenGL_Graph2 ( int width, int height );
void InitializeOpenGL_Graph3 ( int width, int height );
void InitializeOpenGL_Graph4 ( int width, int height );

void Init ( HWND hwnd );
void Init_Graph1 ( HWND hwnd );
void Init_Graph2 ( HWND hwnd );
void Init_Graph3 ( HWND hwnd );
void Init_Graph4 ( HWND hwnd );

void RenderScene ( );
void RenderScene_Graph1 ( );
void RenderScene_Graph2 ( );
void RenderScene_Graph3 ( );
void RenderScene_Graph4 ( );

void DeInit ( );
void DeInit_Graph1 ( );
void DeInit_Graph2 ( );
void DeInit_Graph3 ( );
void DeInit_Graph4 ( );
//////////////////////////////

#pragma endregion

int APIENTRY _tWinMain(	HINSTANCE hInstance, HINSTANCE hPrevInstance,
						LPTSTR    lpCmdLine, int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	HACCEL hAccelTable;

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GRAVITATION, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	hInst = hInstance;

	CTRL = new control();
	TEXTURECTRL = new TextureCtrl();
	//////////////////////////////////
	//	Window class registration	//
	//////////////////////////////////
	WNDCLASS wc = {0};
	wc.lpszClassName = L"Grafic";
	wc.lpfnWndProc = WndProc4;
	wc.hCursor	= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetSysColorBrush(COLOR_3DFACE);
	wc.hIcon	= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));
	RegisterClass(&wc);
	//////////////////////////////////

	hWnd = CreateWindow(	szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, 
							150, 150, 1000, 600, 
							NULL, NULL, hInstance, NULL);
	
	if (!hWnd) { return FALSE; }
	
	//Window size
	RECT rt;
	GetClientRect (hWnd, &rt);
	int maxX = rt.right, maxY = rt.bottom;

	// заставка
	hWndLogo = CreateWindow(L"Grafic", L"Проект Gravitation v1.0", WS_POPUPWINDOW, GetSystemMetrics(SM_CXSCREEN) / 2.0f - 295, GetSystemMetrics(SM_CYSCREEN) / 2.0f - 200,
							590, 400, NULL, NULL, hInst, 0);
	SetWindowLong(hWndLogo,GWL_WNDPROC,(LONG)WndProc4);
	// отображение заставки.
	ShowWindow(hWndLogo, nCmdShow);
	
	// замер времени
	LARGE_INTEGER b_start,b_stop,freq;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&b_start);

	// Отображение курсора.
	ShowCursor (true);

	// картинка
	DrawBackground( hWndLogo, IDB_LOGO, hInst);
	
	//	OGL window
	hWndDraw = CreateMyWindow(hWnd, NULL, 600, 542, WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE, 0, hInst);
	// Инициализация OpenGL		
	Init ( hWndDraw );	
		
	ctrl_interface->InitWndCtrls();

	SetFocus ( hWndLogo );

	while(1)
	{
		if ( PeekMessage ( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			if ( msg.message == WM_QUIT )				
				break;
			// выходит по нажатию клавиши(любой).
			if ( msg.message == WM_KEYDOWN || msg.message == WM_MBUTTONDOWN )
			{
				DestroyWindow(hWndLogo);
				break;
			}
			TranslateMessage ( &msg );
			DispatchMessage ( &msg );
		}
		// выходит после 10 секунд
		QueryPerformanceCounter(&b_stop);
		b_stop.QuadPart = (b_stop.QuadPart - b_start.QuadPart)/(freq.QuadPart);
		if(b_stop.QuadPart == 10)
		{
			DestroyWindow(hWndLogo);
			break;
		}
	}

	// Отображение основного окна.
	ShowWindow(hWnd, nCmdShow);
	// Обновление окна.
	UpdateWindow(hWnd);
	// Передача фокуса клавиатуры этому окну.
	SetFocus ( hWnd );

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GRAVITATION));

	//sys inizialisation
	Physics::Vector o1 = {600,0}, o2 = {100000,0}, o3 = {0,0};
	sys = new Physics::System(0, o2, o1, M_Mercury, M_Ceres, R_Mercury, R_Ceres, 0, 0, 0);

	return MainLoop ( );
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex = {0};
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GRAVITATION));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)GetSysColorBrush( COLOR_MENU );
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_GRAVITATION);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

// обработчик сообщений для главного окна программы
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	RECT rt;
	GetClientRect(hWnd, &rt);
	int maxX = rt.right, maxY = rt.bottom;
	
	switch (message)
	{
	case WM_DESTROY:	
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		ctrl_interface->Size_reaction(maxX, maxY);
		SetFocus(hWnd);
		break;
	case WM_KEYDOWN:
#pragma region WM_KEYDOWN
		
			//////////////////////////
			//Возврат камеры.		//
			//////////////////////////
			/*
			bool Return_to_Planet = false;
			bool Return_to_Asteroid = false;
			bool Return_to_Platz = false;
			*/
		switch(wParam)
		{
		case VK_RETURN://Enter - остановить, если запужен
			if(Go)	SendMessage(hWnd, WM_COMMAND, ID_BUTTON4, 0);
			break;
		case VK_SPACE://пробел - нажатие кнопки запуск
			SendMessage(hWnd, WM_COMMAND, ID_BUTTON2, 0);
			break;
		case 49://1
			Return_to_Planet = !Return_to_Planet;
			if(Return_to_Planet)
			{
				Return_to_Asteroid = false;
				Return_to_Platz = false;
			}
			break;
		case 50://2
			Return_to_Asteroid = !Return_to_Asteroid;
			if(Return_to_Asteroid)
			{
				Return_to_Planet = false;
				Return_to_Platz = false;
			}
			break;
		case 51://3
			Return_to_Platz = !Return_to_Platz;
			if(Return_to_Platz)
			{
				Return_to_Planet = false;
				Return_to_Asteroid = false;
			}
			break;
		default:
			break;
		}

		CTRL->keys[wParam] = true;
		break;
	case WM_KEYUP:
		CTRL->keys[wParam] = false;
		break;
#pragma endregion
	case WM_COMMAND:
# pragma region WM_COMMAND
			wmId    = LOWORD(wParam);
			wmEvent = HIWORD(wParam);
			switch (wmId)
			{
			
			case ID_BUTTON1:
				ctrl_interface->Button1_reaction();
				break;
			case ID_BUTTON2://запуск
				ctrl_interface->Button2_reaction();
				break;
			case ID_BUTTON3:
				ctrl_interface->Button3_reaction();
				break;
			case ID_BUTTON4:// остановить
				ctrl_interface->Button4_reaction();
				break;			
			case ID_RADIOBUTTON1:
				ctrl_interface->RadioButton1_reaction(wmEvent);
				break;
			case ID_RADIOBUTTON2:
				ctrl_interface->RadioButton2_reaction(wmEvent);
				break;
			case ID_RADIOBUTTON3:
				ctrl_interface->RadioButton3_reaction(wmEvent);
				break;
			case ID_RADIOBUTTON4:
				ctrl_interface->RadioButton4_reaction(wmEvent);
				break;
			case ID_COMBO1:
				ctrl_interface->ComboBox1_reaction(wmEvent);
				break;
			case ID_COMBO2:
				ctrl_interface->ComboBox2_reaction(wmEvent);
				break;
			case ID_EDIT1:				
				ctrl_interface->Edit1_reaction(wmEvent);
				break;
			case ID_EDIT3:				
				ctrl_interface->Edit3_reaction(wmEvent);
				break;
			case ID_EDIT5:				
				ctrl_interface->Edit5_reaction(wmEvent);
				break;
			case ID_EDIT6:				
				ctrl_interface->Edit6_reaction(wmEvent);
				break;
			case ID_EDIT7:				
				ctrl_interface->Edit7_reaction(wmEvent);
				break;
			case ID_EDIT8:				
				ctrl_interface->Edit8_reaction(wmEvent);
				break;

			case IDM_ABOUT:	DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);		break;
			case IDM_EXIT:	DestroyWindow(hWnd);												break;
			case ID_HELP:	HtmlHelp( hWnd, L"help.chm::/Index.htm", HH_DISPLAY_TOPIC, NULL);	break;

			#pragma region Grafics
			case ID_grafic1:
								
				hWndGrafic1 = CreateMyWindow(NULL, L"Зависимость скорости астероида от времени", 800, 600, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, 0, hInst);
				//переопределение функции обработки сообщений
				SetWindowLong(hWndGrafic1,GWL_WNDPROC,(LONG)WndProc4);
				MoveWindow ( hWndGrafic1,  200, 200, 680, 380, true );
				Init_Graph1 ( hWndGrafic1 );
				ShowWindow ( hWndGrafic1, SW_SHOWNORMAL );
				Graph1Open = true;
				break;

			case ID_grafic2:

				hWndGrafic2 = CreateMyWindow(NULL,  L"Зависимость расстояния между центрами тел от времени", 800, 600, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, 0, hInst);
				//переопределение функции обработки сообщений
				SetWindowLong(hWndGrafic2,GWL_WNDPROC,(LONG)WndProc4);
				MoveWindow ( hWndGrafic2,  200, 200, 695, 420, true );
				Init_Graph2 ( hWndGrafic2 );
				ShowWindow ( hWndGrafic2, SW_SHOWNORMAL );
				Graph2Open = true;
				break;

			case ID_grafic3:

				hWndGrafic3 = CreateMyWindow(NULL,  L"Зависимость ускорения астероида от врмени", 800, 600, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, 0, hInst);
				//переопределение функции обработки сообщений
				SetWindowLong(hWndGrafic3,GWL_WNDPROC,(LONG)WndProc4);
				MoveWindow ( hWndGrafic3,  200, 200, 680, 335, true );
				Init_Graph3 ( hWndGrafic3 );

				if(!ShowTraect)
				{
					register Physics::Vector o1 = {600,0}, o2 = {100000,0}, o3 = {0,0};
					Physics::System *sys2 = new Physics::System(0, o2, o1, M_Mercury, M_Ceres, R_Mercury, R_Ceres, 0, 0, 0);
					ctrl_interface->DataControl(sys2);
					traectory = new Physics::Vector[300000]();
					mass_volume = Traectory(traectory, *sys2, k);
				}
				
				a_max = GetMaxA(mass_volume, traectory, sys->M + sys->m, sys->R + sys->Ra)/3600;
				ShowWindow ( hWndGrafic3, SW_SHOWNORMAL );
				Graph3Open = true;
				break;

			case ID_grafic4:

				hWndGrafic4 = CreateMyWindow(NULL,  L"Зависимость угла (r^v) от времени", 800, 600, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, 0, hInst);
				//переопределение функции обработки сообщений
				SetWindowLong(hWndGrafic4,GWL_WNDPROC,(LONG)WndProc4);
				MoveWindow ( hWndGrafic4,  200, 200, 605, 335, true );
				Init_Graph4 ( hWndGrafic4 );
				ShowWindow ( hWndGrafic4, SW_SHOWNORMAL );
				Graph4Open = true;
				break;
			#pragma endregion

			default:	return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
# pragma endregion
	case WM_HSCROLL:
		ctrl_interface->AllTrackBar_reaction(lParam);
		break;
	default:		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// обработчик сообщений для окна Настройки
LRESULT CALLBACK WndProc2(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return ctrl_interface->SettingMsg_reaction(hWnd, msg, wParam, lParam);
}

// обработчик сообщений для окна "О программе"
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{ 
			EndDialog(hDlg, LOWORD(wParam));	
			return (INT_PTR)TRUE;}
		break;
	}
	return (INT_PTR)FALSE;
}

// обработчик сообщений для окна запуска проекта.
LRESULT CALLBACK WndProc4(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(msg == WM_DESTROY)
	{
		CloseWindow(hWnd);
		if(hWnd == hWndGrafic1)
			Graph1Open = false;		
		else
		if(hWnd == hWndGrafic2)
			Graph2Open = false;
		else
		if(hWnd == hWndGrafic3)
			Graph3Open = false;
		else
		if(hWnd == hWndGrafic4)
			Graph4Open = false;
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);	
}

WPARAM MainLoop ( )
{
	flag = false;	
	RenderScene ( );	
	while ( 1 )
	{
		//собственно расчет траектории. 
		if(Go)
		{
			for(int i = timespeed; i > 0; i--)
			{
				Schetchik++;
				F_Ailer(k, *sys);
			}

			if(SpeedVect)
			{
				// угол между вектором скорости и осью oX
				if(sys->v.y > 0)
					sys->AlfaV = 180 - acos(sys->v.x / sys->v.norm())* 57,29577951308233;//последнее число (57,29..) - 180 / Pi - переход от радиан к градусам.
				else
					sys->AlfaV = 180 + acos(sys->v.x / sys->v.norm())* 57,29577951308233;
			}
			sys->a = ( (sys->M + sys->m) * G) / ( sys->r.norm() * sys->r.norm() );
			sys->F = sys->a * sys->m;

			//собственно внесение значения
			alpha = acos(((-sys->v.x * sys->r.x) - (sys->v.y * sys->r.y))/ (sys->v.norm() * sys->r.norm()))* 57,29577951308233;
			if(Schetchik >= 200 && Graph1_size < 30000)
			{
				graph.elemX_01 [Graph1_size] = sys->v.norm();
				graph.elemX_02 [Graph1_size] = sys->r.norm();
				graph.elemX_03 [Graph1_size] = sys->a;
				graph.elemX_04 [Graph1_size] = alpha;										
				graph.elemY [Graph1_size] = sys->t;
				Graph1_size++;
				Schetchik = 0;			
			}
		}

		if ( PeekMessage ( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			if ( msg.message == WM_QUIT )
				break;
			TranslateMessage ( &msg );
			DispatchMessage ( &msg );
		}
		else
		{	
			if(flag)
			{
				g_Camera.SetViewByMouse ( );
				g_Camera.Update ( kSpeed );
				hCursor = SetCursor(NULL);
				if(flag2)
				{
					ShowCursor(false);
					flag2 = false;
				}
			}	
			RenderScene ( );
			// ну и сюда всякие инициализации.
			if(Graph1Open)
			{							
				wglMakeCurrent ( g_hDC_Graph1, g_hRC_Graph1 );
				RenderScene_Graph1 ( );
				wglMakeCurrent ( g_hDC, g_hRC );
			}
			if(Graph2Open)
			{							
				wglMakeCurrent ( g_hDC_Graph2, g_hRC_Graph2 );
				RenderScene_Graph2 ( );
				wglMakeCurrent ( g_hDC, g_hRC );
			}
			if(Graph3Open)
			{							
				wglMakeCurrent ( g_hDC_Graph3, g_hRC_Graph3 );
				RenderScene_Graph3 ( );
				wglMakeCurrent ( g_hDC, g_hRC );
			}
			if(Graph4Open)
			{							
				wglMakeCurrent ( g_hDC_Graph4, g_hRC_Graph4 );
				RenderScene_Graph4 ( );
				wglMakeCurrent ( g_hDC, g_hRC );
			}
		}
	}

	DeInit ( );
	DeInit_Graph1 ( );
	DeInit_Graph2 ( );
	DeInit_Graph3 ( );
	DeInit_Graph4 ( );

	return (msg.wParam);
};

//////////////////////////
// for OGL function		//
//////////////////////////

// отрисовка сетки поверхности
void Draw3DGrid ( )
{
	glColor3ub ( 40, 40, 40 );
	
	for ( float i = -400;  i <= 400; i++ )
	{
		glBegin ( GL_LINES );
			glVertex3f ( -400, 0, i ); //X
			glVertex3f ( 400, 0, i );

			glVertex3f ( i, 0, -400 ); //Z
			glVertex3f ( i, 0, 400 );
		glEnd ( );
	}
}

// главное окно OpenGL графики
void RenderScene ( )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	g_Camera.Look ( );

	if(network)
		Draw3DGrid ( );

	// рисуем траекторию астероида
	if(ShowTraect)
		for (int i = 0; i <= mass_volume; i++)
		{
			glBegin ( GL_POINTS );
				glColor3f ( 0.0f, 255.0f, 0.0f );
				glVertex3f ( (GLfloat)((traectory[i].x) * 0.001f),	0, (GLfloat)((traectory[i].y) * 0.001f) );			
			glEnd ( );
		}

	//
	
	// формируем космическое пространство
	qobj = gluNewQuadric ( );
	if(TextureCheck)
	{
		gluQuadricTexture ( qobj, GL_TRUE );
		glBindTexture ( GL_TEXTURE_2D, TEXTURECTRL->textures [ COSMOS_ID ].TexID );
		gluQuadricDrawStyle ( qobj, GLU_FILL );
		glColor3d ( 255, 255, 255 );	
		glPushMatrix ( );
			glRotated ( 90, 1, 0, 0 );
			gluSphere ( qobj, 1000, 20, 10);
		glPopMatrix ( );
	}
	//

	// рисуем планету
	gluQuadricTexture ( qobj, GL_TRUE );
	glBindTexture ( GL_TEXTURE_2D, TEXTURECTRL->textures [ TEXTURECTRL->planet_texture_nummer ].TexID );
	gluQuadricDrawStyle ( qobj, GLU_FILL );
	glColor3d ( 255, 255, 255 );	
	glPushMatrix ( );
		if ( rotPlanet > 360.0f )
			rotPlanet = 0.0f;
		glRotated ( rotPlanet, 0, 1, 0 );
		rotPlanet += 0.05f;
		glRotated ( 90, 1, 0, 0 );
		gluSphere ( qobj, sys->R * 0.001f, 40, 40);
	glPopMatrix ( );	
	//

	// рисуем астеройд
	glTranslatef ( (GLfloat)(sys -> r.x * 0.001f), 0, (GLfloat)(sys -> r.y * 0.001f) );
	gluQuadricTexture ( qobj, GL_TRUE );
	glBindTexture ( GL_TEXTURE_2D, TEXTURECTRL->textures [ TEXTURECTRL->asteroid_texture_nummer ].TexID );
	gluQuadricDrawStyle ( qobj, GLU_FILL );
	glColor3d ( 255, 255, 255 );	
	glPushMatrix ( );
		glRotated ( 90, 1, 0, 0 );
		gluSphere ( qobj, sys->Ra * 0.001f, 100, 100);
	glPopMatrix ( );
	//

	// рисуем вектор скорости
	if(SpeedVect)
		if( sys -> v.norm ( ) != 0 )
			if( sys->r.norm() > sys->R + sys->Ra )
			{					
				glRotated ( ( sys -> AlfaV ), 0, 1, 0 );
				glBegin ( GL_QUADS );
					glColor3f ( 255.0f, 0.0f, 0.0f );
					glVertex3d ( 0, 0.03f, -0.1f );
					glVertex3d ( ( ( sys->v.norm ( ) * (-0.00025f) ) + sys->Ra*0.001f - 0.4f ), 0.03f, -0.1f );			
					glVertex3d ( ( ( sys->v.norm ( ) * (-0.00025f) ) + sys->Ra*0.001f - 0.4f ), 0.03f, 0.1f );
					glVertex3d ( 0, 0.03f, 0.1f );
				glEnd ( );
				glTranslated ( sys->v.norm ( ) * (-0.00025f) + sys->Ra * 0.001f - 0.4f, 0, 0 );
				glBegin ( GL_TRIANGLES );
					glColor3f ( 255.0f, 0.0f, 0.0f );
					glVertex3f ( 0.3f, 0.03f, -0.3f );
					glVertex3f ( 0.3f, 0.03f, 0.3f );
					glVertex3f ( -0.3f, 0.03f, 0 );
				glEnd ( );
			}
	//

	// установка камеры на планету
	if ( Return_to_Planet )
		g_Camera.PositionCamera ( (float)sys->R * ( - 0.01237f ), 8.97f, (float)sys->R * 0.00065f,		(float)sys -> r.x * 0.001f, - 2.5f, (float)sys -> r.y * 0.001f,		0, 1, 0 );
		
	// установка камеры на астероид
	if ( Return_to_Asteroid )
		g_Camera.PositionCamera ( (float)sys -> r.x * 0.00128f, 7.5f, (float)sys -> r.y * 0.0028f,		0.0f, -2.5f, 0.0,		0, 1, 0 );
		
	// установка камеры - вид сверху
	if ( Return_to_Platz )
		g_Camera.PositionCamera ( r_norm_base * 0.5f, r_norm_base * 1.7f, 0.0f,		0.0f, r_norm_base * ( - 50.0f ), 0.0f,		0, 1, 0 );
	
	// подготовка к выводу текста
	glMatrixMode ( GL_PROJECTION );
	glPushMatrix ( );
	glLoadIdentity ( );
	glOrtho ( 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 1 );
	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity ( );
	//

	// вывод текстовой информации на экран
	wchar_t Buffer [ 256 ];
	// расчет часов и дней
	glDisable ( GL_DEPTH_TEST );
		glColor3f ( 1.0f, 1.0f, 1.0f );												
		swprintf ( Buffer, sizeof ( Buffer ), L"Время: %d часов %d минут", (int)sys -> t, (int)((sys->t - (int)sys -> t) * 60) );
		Font -> Print ( 600, 20, Buffer );
	glEnable ( GL_DEPTH_TEST );
	
	// вывод на экран расстояния от центра астероида до центра планеты
	glDisable ( GL_DEPTH_TEST );
		glColor3ub ( 0, 216, 46 );												
		swprintf ( Buffer, sizeof ( Buffer ), L"r = %.2lf км", sys -> r.norm ( ) );
		Font -> Print ( 10, 20, Buffer );  
	glEnable ( GL_DEPTH_TEST );

	// вывод на экран скорости астроида
	glDisable ( GL_DEPTH_TEST );
		glColor3ub ( 200, 0, 0 );												
		swprintf ( Buffer, sizeof ( Buffer ), L"v = %.2lf км/с", sys -> v.norm() * 0.0166666666f );
		Font -> Print ( 10, 32, Buffer );  
	glEnable ( GL_DEPTH_TEST );	

	// вывод на экран силы действующей на астероид
	glDisable ( GL_DEPTH_TEST );
		glColor3f ( 1.0f, 1.0f, 1.0f );												
		swprintf ( Buffer, sizeof ( Buffer), L"F = %.2lf * 10^20 H", sys -> F * 1e-20 );
		Font -> Print ( 10, 44, Buffer );
	glEnable ( GL_DEPTH_TEST );	
	
	// вывод на экран ускорения астероида
	glDisable ( GL_DEPTH_TEST );
		glColor3ub ( 83, 156, 255 );												
		swprintf ( Buffer, sizeof ( Buffer ), L"a = %.2lf км/с^2.", sys -> a / 3600 );
		Font -> Print ( 10, 56, Buffer );
	glEnable ( GL_DEPTH_TEST );	

	// вывод на экран массы планеты
	glDisable ( GL_DEPTH_TEST );
		glColor3ub ( 255, 255, 255 );												
		swprintf ( Buffer, sizeof ( Buffer ), L"Масса планеты = %.0lf * 10^21 кг", sys -> M / 1e21 );
		Font -> Print ( 500, 570, Buffer );  
	glEnable ( GL_DEPTH_TEST );	
	
	// вывод на экран массы астероида
	glDisable ( GL_DEPTH_TEST );
		glColor3ub ( 255, 255, 255 );												
		swprintf ( Buffer, sizeof ( Buffer ), L"Масса астероида = %.0lf * 10^18 кг", sys -> m / 1e18 );
		Font -> Print ( 500, 582, Buffer );  
	glEnable ( GL_DEPTH_TEST );

	// условие столкновения астероида с планетой
	if(sys->r.norm() < sys->R + sys->Ra || sys->r.norm() > 400e3)
	{
		//столкновение с планетой или выход за пределы скайсферы.
		
		// Аналогия с "изменением параметров".
		change = true;
		glDisable ( GL_DEPTH_TEST );
			glColor3f ( 1.0f, 1.0f, 1.0f );
			if(sys->r.norm() < sys->R + sys->Ra)
				swprintf ( Buffer, sizeof ( Buffer ), L"Произошло столкновение с планетой");
			else
				swprintf ( Buffer, sizeof ( Buffer ), L"Выход за пределы пространства");
			Font -> Print ( 10, 90, Buffer );
		glEnable ( GL_DEPTH_TEST );
		if(Go)
		{
		// "записываем время смерти"
				graph.elemX_01 [Graph1_size] = sys->v.norm();
				graph.elemX_02 [Graph1_size] = sys->r.norm();
				graph.elemX_03 [Graph1_size] = sys->a;
				graph.elemX_04 [Graph1_size] = acos(((-sys->v.x * sys->r.x) - (sys->v.y * sys->r.y))/ (sys->v.norm() * sys->r.norm()))* 57,29577951308233;		
				graph.elemY [Graph1_size] = sys->t;
				Graph1_size++;	
		//
		}
		Go = false;
	}

	// подготовка к отключению окна OpenGL
	glMatrixMode ( GL_PROJECTION );
	glPopMatrix ( );
	glMatrixMode ( GL_MODELVIEW );

	// ждём завершения отрисовки
	glFlush();

	gluDeleteQuadric ( qobj );
	// сброс буфера отрисовки
	SwapBuffers ( g_hDC );
}

// отрисовка первого графика
void RenderScene_Graph1 ( )
{
	glClearColor ( 0.95f, 0.95f, 0.95f, 1 );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity();
	
	glTranslatef ( 0.0f, 0.0f, 0.0f );

	//сетка и цена деления
	for ( int i = 35;  i <= 580; i += 10 )
	{		
		glColor3ub ( 0, 0, 0 );
		glBegin ( GL_LINES );
			//сетка
			glVertex2i ( i, 365 );//Y
			glVertex2i ( i, 80 );
			if ( i >= 85 && i < 365 )
			{				
				glVertex2i ( 580, i );//X
				glVertex2i ( 35, i );
			}
			//
			// масштаб			
			if ( i >= 35 && i < 580 )
			{
				glVertex2i ( i, 363 ); //X
				glVertex2i ( i, 367 );
			}
			if ( i >= 85 && i < 365 )
			{				
				glVertex2i ( 33, i ); //Y
				glVertex2i ( 38, i );
			}
			//
		glEnd ( );
	}	
	//	
	glBegin ( GL_LINES );		
		// линия оси OY
		glVertex2i ( 35, 85 );	
		glVertex2i ( 35, 360 );
		// линия оси OX		
		glVertex2i ( 580, 365 );
		glVertex2i ( 35, 365 );		
	glEnd ( );
	glBegin ( GL_TRIANGLES );
		// стрелочка оси OY
		glVertex2i ( 31, 80 );
		glVertex2i ( 39, 80 );			
		glVertex2i ( 35, 70 );
		//
		// стрелочка оси OX
		glVertex2i ( 580, 361 );
		glVertex2i ( 580, 369 );				
		glVertex2i ( 590, 365 );
		//
	glEnd ( );

	//отрисовка графика
	glTranslatef ( 35.0f, 365.0f, 0.0f );
	glColor3ub ( 120, 0, 0 );
	glBegin ( GL_LINE_STRIP );
		for ( u = 0; u < Graph1_size; u++ )
		{
			glVertex2f ( graph.elemY [ u ] * 4, 0 );
			glVertex2f ( graph.elemY [ u ] * 4, graph.elemX_01 [ u ] * ( - 3.324269601455977e-4 ) );
		}
	glEnd ( );
	//

	glMatrixMode ( GL_PROJECTION );
	glPushMatrix ( );
	glLoadIdentity ( );
	glOrtho ( 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 1 );
	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity ( );

	wchar_t Buffer [ 256 ];	
	glDisable ( GL_DEPTH_TEST );
		glColor3f ( 0.0f, 0.0f, 0.0f );												
		swprintf ( Buffer, sizeof ( Buffer ), L"t [час]" );
		Font -> Print ( 755, 590, Buffer );  
	glEnable ( GL_DEPTH_TEST );

	glDisable ( GL_DEPTH_TEST );
		glColor3f ( 0.0f, 0.0f, 0.0f );												
		swprintf ( Buffer, sizeof ( Buffer ), L"v [км/с]" );
		Font -> Print ( 58, 120, Buffer );  
	glEnable ( GL_DEPTH_TEST );		

	glDisable ( GL_DEPTH_TEST );
		glColor3f ( 0.0f, 0.0f, 0.0f );												
		swprintf ( Buffer, sizeof ( Buffer ), L"Время: %d часов %d минут", (int)sys -> t, (int)((sys->t - (int)sys -> t) * 60) );
		Font -> Print ( 630, 116, Buffer );  
	glEnable ( GL_DEPTH_TEST );

	glDisable ( GL_DEPTH_TEST );
		glColor3ub ( 0, 0, 0 );												
		swprintf ( Buffer, sizeof ( Buffer ), L"v [км/с] = %.2lf", sys -> v.norm() * 0.0166666666f );
		Font -> Print ( 495, 116, Buffer );  
	glEnable ( GL_DEPTH_TEST );		

	// цена деления оси OX
	for ( int i = 0, iv = 45; i <= 130; i += 5 )
	{
		glDisable ( GL_DEPTH_TEST );
			glColor3f ( 0.0f, 0.0f, 0.0f );												
			swprintf ( Buffer, sizeof ( Buffer ), L"%d", i );
			Font -> Font -> FaceSize ( 10 );
			Font -> Print ( iv, 590, Buffer );
		glEnable ( GL_DEPTH_TEST );
		if ( i < 5 && i >= 0 )
			iv += 26;
		if ( i < 25 && i >= 5 )
			iv += 26;
		if ( i < 80 && i >= 25 )
			iv += 27;
		if ( i >= 80 )
			iv += 26;
	}
	// цена деления оси OY
	for ( int i = 14000, iv = 135; i > 0; i -= 500 )
	{
		glDisable ( GL_DEPTH_TEST );
			glColor3f ( 0.0f, 0.0f, 0.0f );												
			swprintf ( Buffer, sizeof ( Buffer ), L"%d", i );
			Font -> Font -> FaceSize ( 10 );
			Font -> Print ( 10, iv, Buffer );
		glEnable ( GL_DEPTH_TEST );
		if ( i <= 14008 && i > 13596 )
			iv += 15;
		if ( i <= 13596 && i > 13184 )
			iv += 16;
		if ( i <= 13184 && i > 11948 )
			iv += 16.6f;
		if ( i <= 11948 && i > 11536 )
			iv += 15.9f;
		if ( i <= 11536 && i > 9888 )
			iv += 16.6f;
		if ( i <= 9888 && i > 9064 )
			iv += 15.8f;
		if ( i <= 9064 && i > 7828 )
			iv += 16.2f;
		if ( i <= 7828 && i > 6180 )
			iv += 16.8f;
		if ( i <= 6180 && i > 5768 )
			iv += 15.1f;
		if ( i <= 5768 && i > 3296 )
			iv += 16.2f;
		if ( i <= 3296 && i > 2884 )
			iv += 15.8f;
		if ( i <= 2884 && i > 2060 )
			iv += 16.2f;
		if ( i <= 2060 && i > 824 )
			iv += 16.4f;
		if ( i <= 824 )
			iv += 15.8f;
	}

	glMatrixMode ( GL_PROJECTION );
	glPopMatrix ( );
	glMatrixMode ( GL_MODELVIEW );

	glFlush();
	
	SwapBuffers ( g_hDC_Graph1 );
}

// отрисовка второго графика
void RenderScene_Graph2 ( )
{
	glClearColor ( 0.95f, 0.95f, 0.95f, 1 );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity();
	
	glTranslatef ( 0.0f, 0.0f, 0.0f );

	//сетка и цена деления
	for ( float i = 35;  i <= 580; i += 10 )
	{		
		glColor3ub ( 0, 0, 0 );
		glBegin ( GL_LINES );
			//сетка
			glVertex2i ( i, 425 );//Y
			glVertex2i ( i, 20 );
			if ( i < 435 )
			{	
				if ( i == 35 )
				{
					glVertex2i ( 580, 25 );//X
					glVertex2i ( 35, 25 );
				}
				glVertex2i ( 580, i );//X
				glVertex2i ( 35, i );
			}
			//
			// масштаб			
			if ( i >= 35 && i < 580 )
			{
				glVertex2i ( i, 423 ); //X
				glVertex2i ( i, 427 );
			}
			if ( i < 430 )
			{
				if ( i == 35 )
				{
					glVertex2i ( 33, 25 );//X
					glVertex2i ( 38, 25 );
				}
				glVertex2i ( 33, i ); //Y
				glVertex2i ( 38, i );
			}
			//
		glEnd ( );
	}	
	//	
	glBegin ( GL_LINES );		
		// линия оси OY
		glVertex2i ( 35, 20 );	
		glVertex2i ( 35, 425 );
		// линия оси OX		
		glVertex2i ( 580, 365 );
		glVertex2i ( 35, 365 );
	glEnd ( );
	glBegin ( GL_TRIANGLES );
		// стрелочка оси OY
		glVertex2i ( 31, 20 );
		glVertex2i ( 39, 20 );			
		glVertex2i ( 35, 10 );
		//
		// стрелочка оси OX
		glVertex2i ( 580, 421 );
		glVertex2i ( 580, 429 );				
		glVertex2i ( 590, 425 );
		//
	glEnd ( );

	//отрисовка графика
	glTranslatef ( 35.0f, 425.0f, 0.0f );
	glColor3ub ( 0, 66, 0 );
	glBegin ( GL_LINE_STRIP );
		for ( u = 0; u < Graph1_size; u++ )
		{
			glVertex2f ( graph.elemY [ u ] * 4, 0 );
			glVertex2f ( graph.elemY [ u ] * 4, graph.elemX_02 [ u ] * ( - 1e-3 ) );
		}
	glEnd ( );
	//

	glMatrixMode ( GL_PROJECTION );
	glPushMatrix ( );
	glLoadIdentity ( );
	glOrtho ( 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 1 );
	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity ( );

	wchar_t Buffer [ 256 ];	
	glDisable ( GL_DEPTH_TEST );
		glColor3f ( 0.0f, 0.0f, 0.0f );												
		swprintf ( Buffer, sizeof ( Buffer ), L"t [час]" );
		Font -> Print ( 763, 592, Buffer );  
	glEnable ( GL_DEPTH_TEST );

	glDisable ( GL_DEPTH_TEST );
		glColor3f ( 0.0f, 0.0f, 0.0f );												
		swprintf ( Buffer, sizeof ( Buffer ), L"r [км]" );
		Font -> Print ( 58, 20, Buffer );  
	glEnable ( GL_DEPTH_TEST );		

	glDisable ( GL_DEPTH_TEST );
		glColor3f ( 0.0f, 0.0f, 0.0f );												
		swprintf ( Buffer, sizeof ( Buffer ), L"Время: %d часов %d минут", (int)sys -> t, (int)((sys->t - (int)sys -> t) * 60) );
		Font -> Print ( 630, 16, Buffer );  
	glEnable ( GL_DEPTH_TEST );

	glDisable ( GL_DEPTH_TEST );
		glColor3ub ( 0, 0, 0 );												
		swprintf ( Buffer, sizeof ( Buffer ), L"r [км] = %.2lf км", sys -> r.norm ( ) );
		Font -> Print ( 495, 16, Buffer );  
	glEnable ( GL_DEPTH_TEST );	

	// цена деления оси OX
	for ( int i = 0, iv = 45; i <= 130; i += 5 )
	{
		glDisable ( GL_DEPTH_TEST );
			glColor3f ( 0.0f, 0.0f, 0.0f );												
			swprintf ( Buffer, sizeof ( Buffer ), L"%d", i );
			Font -> Font -> FaceSize ( 10 );
			Font -> Print ( iv, 590, Buffer );
		glEnable ( GL_DEPTH_TEST );
		if ( i >= 0 && i < 5 )
			iv += 26;
		if ( i >= 5 && i < 25 )
			iv += 26;
		if ( i >= 25 && i < 80 )
			iv += 27;
		if ( i >= 80 )
			iv += 26;
	}
	// цена деления оси OY
	for ( int i = 400000, iv = 34; i > 0; i -= 10000 )
	{
		glDisable ( GL_DEPTH_TEST );
			glColor3f ( 0.0f, 0.0f, 0.0f );												
			swprintf ( Buffer, sizeof ( Buffer ), L"%d", i );
			Font -> Font -> FaceSize ( 10 );
			Font -> Print ( 4, iv, Buffer );
		glEnable ( GL_DEPTH_TEST );
		if ( i <= 400000 && i > 390000 )
			iv += 14;
		if ( i <= 390000 && i > 370000 )
			iv += 13;
		if ( i <= 370000 && i > 350000 )
			iv += 14;
		if ( i <= 350000 && i > 330000 )
			iv += 13;
		if ( i <= 330000 && i > 320000 )
			iv += 12;
		if ( i <= 320000 && i > 290000 )
			iv += 14;
		if ( i <= 290000 && i > 260000 )
			iv += 13;
		if ( i <= 260000 && i > 250000 )
			iv += 14;
		if ( i <= 250000 && i > 210000 )
			iv += 13;
		if ( i <= 210000 && i > 190000 )
			iv += 14;
		if ( i <= 190000 && i > 160000 )
			iv += 13;
		if ( i <= 160000 && i > 140000 )
			iv += 14;
		if ( i <= 140000 && i > 110000 )
			iv += 13;
		if ( i <= 110000 && i > 100000 )
			iv += 14;
		if ( i <= 100000 && i > 80000 )
			iv += 13;
		if ( i <= 80000 && i > 60000 )
			iv += 14;
		if ( i <= 60000 )
			iv += 13;
	}

	if( sys->r.norm() < sys->R + sys->Ra || sys->r.norm() > 400e3)
	{
		//столкновение с планетой или выход за пределы скайсферы.
		Go = false;
		// Аналогия с "изменением параметров".
		change = true;
	}

	glMatrixMode ( GL_PROJECTION );
	glPopMatrix ( );
	glMatrixMode ( GL_MODELVIEW );

	glFlush();
	
	SwapBuffers ( g_hDC_Graph2 );
}

// отрисовка третьего графика
void RenderScene_Graph3 ( )
{
	glClearColor ( 0.95f, 0.95f, 0.95f, 1 );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity();
	
	glTranslatef ( 0.0f, 0.0f, 0.0f );

	//сетка и цена деления
	for ( float i = 30;  i <= 585; i += 20 )
	{		
		glColor3ub ( 0, 0, 0 );
		glBegin ( GL_LINES );
			//сетка
			glVertex2i ( i + 5, 18 );//Y
			glVertex2i ( i + 5, 322 );
			if ( i < 320 )
			{				
				glVertex2i ( 33, i - 10 );//X
				glVertex2i ( 585, i - 10 );
			}					
		glEnd ( );
	}	
	//	
	glBegin ( GL_LINES );		
		// линия оси OY
		glVertex2i ( 35, 20 );	
		glVertex2i ( 35, 322 );
		// линия оси OX		
		glVertex2i ( 33, 320 );
		glVertex2i ( 585, 320 );		
	glEnd ( );
	glBegin ( GL_TRIANGLES );
		// стрелочка оси OY
		glVertex2i ( 31, 20 );
		glVertex2i ( 39, 20 );			
		glVertex2i ( 35, 10 );
		//
		// стрелочка оси OX
		glVertex2i ( 585, 316 );
		glVertex2i ( 585, 324 );				
		glVertex2i ( 595, 320 );
		//
	glEnd ( );

	//отрисовка графика
	glTranslatef ( 35.0f, 320.0f, 0.0f );
	glColor3ub ( 0, 0, 50 );
	glBegin ( GL_LINE_STRIP );
 	if ( a_max <= 3750 )
	{
		CoefGraph03_01 = 250;
		CoefGraph03_02 = 1;
		CoefGraph03_03 = 1.149872420828013e-5 * 2;
	}
	if ( a_max <= 1875 )
	{
		CoefGraph03_01 = 125;
		CoefGraph03_02 = 2;
		CoefGraph03_03 = 2.256683043200783e-5 * 2;
	}
	if ( a_max <= 750 )
	{
		CoefGraph03_01 = 50;
		CoefGraph03_02 = 5;
		CoefGraph03_03 = 5.835724764710872e-5 * 2;
	}
	if ( a_max <= 375 )
	{
		CoefGraph03_01 = 25;
		CoefGraph03_02 = 10;
		CoefGraph03_03 = 1.111136629816222e-4 * 2;
	}
		for ( u = 0; u < Graph1_size; u++ )
		{
			glVertex2f ( graph.elemY [ u ] * 4, 0 ) ;
			glVertex2f ( graph.elemY [ u ] * 4, graph.elemX_03 [ u ] * ( - CoefGraph03_03 ) );
		}
	glEnd ( );
	//

	glMatrixMode ( GL_PROJECTION );
	glPushMatrix ( );
	glLoadIdentity ( );
	glOrtho ( 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 1 );
	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity ( );

	wchar_t Buffer [ 256 ];	
	glDisable ( GL_DEPTH_TEST );
		glColor3f ( 0.0f, 0.0f, 0.0f );												
		swprintf ( Buffer, sizeof ( Buffer ), L"t [час]" );
		Font -> Print ( 762, 590, Buffer );  
	glEnable ( GL_DEPTH_TEST );

	glDisable ( GL_DEPTH_TEST );
		glColor3f ( 0.0f, 0.0f, 0.0f );												
		swprintf ( Buffer, sizeof ( Buffer ), L"a [км/с   ]" );
		Font -> Print ( 56, 20, Buffer );  
	glEnable ( GL_DEPTH_TEST );	

	glDisable ( GL_DEPTH_TEST );
		glColor3f ( 0.0f, 0.0f, 0.0f );												
		swprintf ( Buffer, sizeof ( Buffer ), L"2" );
		Font -> Print ( 96.5f, 17, Buffer );  
	glEnable ( GL_DEPTH_TEST );	

	glDisable ( GL_DEPTH_TEST );
		glColor3f ( 0.0f, 0.0f, 0.0f );												
		swprintf ( Buffer, sizeof ( Buffer ), L"Время: %d часов %d минут", (int)sys -> t, (int)((sys->t - (int)sys -> t) * 60) );
		Font -> Print ( 630, 19, Buffer );  
	glEnable ( GL_DEPTH_TEST );

	glDisable ( GL_DEPTH_TEST );
		glColor3ub ( 0, 0, 0 );												
		swprintf ( Buffer, sizeof ( Buffer ), L"a [км/с   ] = %.2lf", sys -> a / 3600 );
		Font -> Print ( 495, 19, Buffer );  
	glEnable ( GL_DEPTH_TEST );	

	glDisable ( GL_DEPTH_TEST );
		glColor3f ( 0.0f, 0.0f, 0.0f );												
		swprintf ( Buffer, sizeof ( Buffer ), L"2" );
		Font -> Print ( 535.5f, 16, Buffer );  
	glEnable ( GL_DEPTH_TEST );		

	// цена деления оси OX
	for ( int i = 0, iv = 45; i <= 130; i += 5 )
	{
		glDisable ( GL_DEPTH_TEST );
			glColor3f ( 0.0f, 0.0f, 0.0f );												
			swprintf ( Buffer, sizeof ( Buffer ), L"%d", i );
			Font -> Font -> FaceSize ( 10 );
			Font -> Print ( iv, 590, Buffer );
		glEnable ( GL_DEPTH_TEST );
		if ( i >= 0 && i < 5 )
			iv += 26;
		if ( i >= 5 && i < 25 )
			iv += 26;
		if ( i >= 25 && i < 80 )
			iv += 27;
		if ( i >= 80 )
			iv += 26;
	}
	// цена деления оси OY
	for ( int i = 3750 / CoefGraph03_02, iv = 41; i > 0; i -= CoefGraph03_01 )
	{
		glDisable ( GL_DEPTH_TEST );
			glColor3f ( 0.0f, 0.0f, 0.0f );												
			swprintf ( Buffer, sizeof ( Buffer ), L"%d", i );			
			Font -> Print ( 10, iv, Buffer );
		glEnable ( GL_DEPTH_TEST );
		if ( i <= 3750 / CoefGraph03_02 && i > 3250 / CoefGraph03_02 )
			iv += 34;
		if ( i <= 3250 / CoefGraph03_02 )
			iv += 36;
	}

	glMatrixMode ( GL_PROJECTION );
	glPopMatrix ( );
	glMatrixMode ( GL_MODELVIEW );

	glFlush();

	SwapBuffers ( g_hDC_Graph3 );
}

// отрисовка четвёртого графика
void RenderScene_Graph4 ( )
{
	glClearColor ( 0.95f, 0.95f, 0.95f, 1 );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity();
	
	glTranslatef ( 0.0f, 0.0f, 0.0f );

	//сетка и цена деления
	for ( float i = 35;  i <= 585; i += 20 )
	{		
		glColor3ub ( 0, 0, 0 );
		glBegin ( GL_LINES );
			//сетка
			glVertex2i ( i, 20 );//Y
			glVertex2i ( i, 207 );
			if ( i < 205 )
			{				
				glVertex2i ( 33, i - 10 );//X
				glVertex2i ( 585, i - 10 );
			}					
		glEnd ( );
	}	
	//	
	glBegin ( GL_LINES );		
		// линия оси OY
		glVertex2i ( 35, 20 );	
		glVertex2i ( 35, 207 );
		// линия оси OX	
		glVertex2i ( 33, 205 );
		glVertex2i ( 585, 205 );		
	glEnd ( );
	glBegin ( GL_TRIANGLES );
		// стрелочка оси OY
		glVertex2i ( 31, 20 );
		glVertex2i ( 39, 20 );			
		glVertex2i ( 35, 10 );
		//
		// стрелочка оси OX
		glVertex2i ( 585, 201 );
		glVertex2i ( 585, 209 );				
		glVertex2i ( 595, 205 );
		//
	glEnd ( );

	//отрисовка графика
	glTranslatef ( 35.0f, 205.0f, 0.0f );
	glColor3ub ( 170, 96, 0 );
	glBegin ( GL_LINE_STRIP );
		for ( u = 0; u < Graph1_size; u++ )
		{
			glVertex2i ( graph.elemY [ u ] * 4, 0 );
			glVertex2i ( graph.elemY [ u ] * 4, ( - 1 ) * ( graph.elemX_04 [ u ] ) );
		}
	glEnd ( );
	//

	glMatrixMode ( GL_PROJECTION );
	glPushMatrix ( );
	glLoadIdentity ( );
	glOrtho ( 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 1 );
	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity ( );

	wchar_t Buffer [ 256 ];	
	glDisable ( GL_DEPTH_TEST );
		glColor3f ( 0.0f, 0.0f, 0.0f );												
		swprintf ( Buffer, sizeof ( Buffer ), L"t [час]" );
		Font -> Print ( 762, 590, Buffer );  
	glEnable ( GL_DEPTH_TEST );

	glDisable ( GL_DEPTH_TEST );
		glColor3f ( 0.0f, 0.0f, 0.0f );												
		swprintf ( Buffer, sizeof ( Buffer ), L"alpha [  ]" );
		Font -> Print ( 62, 35, Buffer );
	glEnable ( GL_DEPTH_TEST );	

	glDisable ( GL_DEPTH_TEST );
		glColor3f ( 0.0f, 0.0f, 0.0f );												
		swprintf ( Buffer, sizeof ( Buffer ), L"o" );
		Font -> Print ( 105.5f, 31, Buffer );  
	glEnable ( GL_DEPTH_TEST );	

	glDisable ( GL_DEPTH_TEST );
		glColor3f ( 0.0f, 0.0f, 0.0f );										
		swprintf ( Buffer, sizeof ( Buffer ), L"Время: %d часов %d минут", (int)sys -> t, (int)((sys->t - (int)sys -> t) * 60) );
		Font -> Print ( 630, 35, Buffer );  
	glEnable ( GL_DEPTH_TEST );

	glDisable ( GL_DEPTH_TEST );
		glColor3f ( 0.0f, 0.0f, 0.0f );												
		swprintf ( Buffer, sizeof ( Buffer ), L"alpha [  ] = %.2f", alpha );
		Font -> Print ( 450, 35, Buffer );  
	glEnable ( GL_DEPTH_TEST );

	glDisable ( GL_DEPTH_TEST );
		glColor3f ( 0.0f, 0.0f, 0.0f );												
		swprintf ( Buffer, sizeof ( Buffer ), L"o" );
		Font -> Print ( 494.5f, 31, Buffer );  
	glEnable ( GL_DEPTH_TEST );	

	// цена деления оси OX
	for ( int i = 0, iv = 45; i <= 130; i += 5 )
	{
		glDisable ( GL_DEPTH_TEST );
			glColor3f ( 0.0f, 0.0f, 0.0f );												
			swprintf ( Buffer, sizeof ( Buffer ), L"%d", i );
			Font -> Font -> FaceSize ( 10 );
			Font -> Print ( iv, 590, Buffer );
		glEnable ( GL_DEPTH_TEST );
		if ( i >= 0 && i < 5 )
			iv += 26;
		if ( i >= 5 && i < 25 )
			iv += 26;
		if ( i >= 25 && i < 80 )
			iv += 27;
		if ( i >= 80 )
			iv += 26;
	}
	// цена деления оси OY
	for ( int i = 180, iv = 75; i > 0; i -= 20 )
	{
		glDisable ( GL_DEPTH_TEST );
			glColor3f ( 0.0f, 0.0f, 0.0f );												
			swprintf ( Buffer, sizeof ( Buffer ), L"%d", i );			
			Font -> Print ( 10, iv, Buffer );
		glEnable ( GL_DEPTH_TEST );
		if ( i <= 180 && i > 100 )
			iv += 55;
		if ( i <= 100 )
			iv += 56;
	}

	glMatrixMode ( GL_PROJECTION );
	glPopMatrix ( );
	glMatrixMode ( GL_MODELVIEW );

	glFlush();

	SwapBuffers ( g_hDC_Graph4 );
}

// инициализация главного OpenGL окна
void Init ( HWND hWnd )
{

	/*
	# define ASTEROID_ID			0
	# define PLANET_ABSTRACT_ID		1

	# define COSMOS_ID				2

	# define PLANET_MERCURY_ID		3
	# define PLANET_VENUS_ID		4
	# define PLANET_EARTH_ID		5
	# define PLANET_MARS_ID			6
	# define PLANET_URANUS_ID		7
	# define PLANET_NUPTUNE_ID		8
	# define PLANET_PLUTO_ID		9
	# define PLANET_HAUMEA_ID		10
	# define PLANET_ERIS_ID			11
	# define PLANET_MAKEMAKE_ID		12

	# define ASTEROID_CERES_ID		13
	# define ASTEROID_PALLAS_ID		14
	# define ASTEROID_VESTA_ID		15
	# define ASTEROID_GIGEA_ID		16
	# define ASTEROID_EUPHROSYNE_ID	17
	*/

	g_hWnd = hWnd;
	GetClientRect ( g_hWnd, &g_rRect );
	InitializeOpenGL ( g_rRect.right, g_rRect.bottom );
	
	TEXTURECTRL->UpgateTexture();
	TEXTURECTRL->LoadFirstTexture();

	// подключение шрифта
	Font = new CFont ( "datafiles\\Arial.ttf", 10, 10 );
	// установка камеры по заданным координатам и в заданное направление
	g_Camera.PositionCamera ( 0, 2.5f, 30,		0, 2.5f, 0,		0, 1, 0 );
}

// инициализация первого окна с графиком
void Init_Graph1 ( HWND hWnd )
{
	g_hWnd_Graph1 = hWnd;
	GetClientRect ( g_hWnd_Graph1, &g_rRect_Graph1 );
	InitializeOpenGL_Graph1 ( g_rRect_Graph1.right, g_rRect_Graph1.bottom );
}

// инициализация второго окна с графиком
void Init_Graph2 ( HWND hWnd )
{
	g_hWnd_Graph2 = hWnd;
	GetClientRect ( g_hWnd_Graph2, &g_rRect_Graph2 );
	InitializeOpenGL_Graph2 ( g_rRect_Graph2.right, g_rRect_Graph2.bottom );
}

// инициализация третьего окна с графиком
void Init_Graph3 ( HWND hWnd )
{
	g_hWnd_Graph3 = hWnd;
	GetClientRect ( g_hWnd_Graph3, &g_rRect_Graph3 );
	InitializeOpenGL_Graph3 ( g_rRect_Graph3.right, g_rRect_Graph3.bottom );
}

// инициализация четвёртого окна с графиком
void Init_Graph4 ( HWND hWnd )
{
	g_hWnd_Graph4 = hWnd;
	GetClientRect ( g_hWnd_Graph4, &g_rRect_Graph4 );
	InitializeOpenGL_Graph4 ( g_rRect_Graph4.right, g_rRect_Graph4.bottom );
}

// обработка сообщений 
LRESULT CALLBACK WndProc3 ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( uMsg )
	{
	case WM_SIZE:
		SizeOpenGLScreen ( LOWORD ( lParam ), HIWORD ( lParam ) );
		GetClientRect ( g_hWnd, &g_rRect );
		break;
	case WM_CLOSE:
		CloseWindow(hWnd);
		break;
	case WM_MOUSEACTIVATE:
		if(CTRL->keys_summ())
		{
			flag = !flag;
			if(flag == false)
			{
				SetCursor(hCursor);
				ShowCursor(true);
				flag2 = true;
			}
		}
		GetWindowRect(::hWnd, hWndRect);
		SetFocus(::hWnd);
		break;
	default:
		return DefWindowProc ( hWnd, uMsg, wParam, lParam );
		break;
	}

	return 0;
}


//////////////////////////////////////////
//	Control Interface class function	//
//////////////////////////////////////////

void CTRL_INTERFACE::InitWndCtrls()
{
	InitCommonControls();
	//Window size
	RECT rt;
	GetClientRect (hWnd, &rt);
	int maxX = rt.right, maxY = rt.bottom;

	hWndButton1 = CreateWindow(L"button",L"Расчет траектории",		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|BS_PUSHBUTTON,				maxX - 370, maxY - 110,	140, 40,	hWnd,(HMENU)ID_BUTTON1,hInst,NULL);
	hWndButton2 = CreateWindow(L"button",L"Запуск",					WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|BS_PUSHBUTTON,				maxX - 370, maxY - 60,	140, 40,	hWnd,(HMENU)ID_BUTTON2,hInst,NULL);
	hWndButton3 = CreateWindow(L"button",L"Настройки",				WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|BS_PUSHBUTTON,				maxX - 180, maxY - 110,	140, 40,	hWnd,(HMENU)ID_BUTTON3,hInst,NULL);
	hWndButton4 = CreateWindow(L"button",L"Остановить",				WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|BS_PUSHBUTTON|WS_DISABLED,	maxX - 180, maxY - 60,	140, 40,	hWnd,(HMENU)ID_BUTTON4,hInst,NULL);

	hWndRadioButton1 = CreateWindow(L"button", L"Готовая",	WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|BS_AUTORADIOBUTTON|WS_GROUP, maxX - 370, 30, 70, 20,	hWnd,(HMENU)ID_RADIOBUTTON1, hInst, NULL);
    hWndRadioButton2 = CreateWindow(L"button", L"Своя",		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|BS_AUTORADIOBUTTON,			 maxX - 370, 90, 50, 20,	hWnd,(HMENU)ID_RADIOBUTTON2, hInst, NULL);
	hWndRadioButton3 = CreateWindow(L"button", L"Готовый",	WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|BS_AUTORADIOBUTTON|WS_GROUP, maxX - 180, 30, 72, 20,	hWnd,(HMENU)ID_RADIOBUTTON3, hInst, NULL);
    hWndRadioButton4 = CreateWindow(L"button", L"Свой",		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|BS_AUTORADIOBUTTON,			 maxX - 180, 90, 50, 20,	hWnd,(HMENU)ID_RADIOBUTTON4, hInst, NULL);
	
	SendMessage(hWndRadioButton1, BM_SETCHECK, 1, 0l);
	SendMessage(hWndRadioButton3, BM_SETCHECK, 1, 0l);

	hWndCombo1 = CreateWindow(L"combobox",NULL,WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_VSCROLL|CBS_DROPDOWNLIST|CBS_AUTOHSCROLL,	maxX - 370, 60, 130, 300,	hWnd,(HMENU)ID_COMBO1,hInst,NULL);
	hWndCombo2 = CreateWindow(L"combobox",NULL,WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_VSCROLL|CBS_DROPDOWNLIST|CBS_AUTOHSCROLL,	maxX - 180, 60, 130, 300,	hWnd,(HMENU)ID_COMBO2,hInst,NULL);

	// Добавление пунктов
	SendMessage(hWndCombo1, CB_ADDSTRING, 0, (LPARAM)L"Mercury");
    SendMessage(hWndCombo1, CB_ADDSTRING, 0, (LPARAM)L"Venus");
    SendMessage(hWndCombo1, CB_ADDSTRING, 0, (LPARAM)L"Earth");
	SendMessage(hWndCombo1, CB_ADDSTRING, 0, (LPARAM)L"Mars");
	SendMessage(hWndCombo1, CB_ADDSTRING, 0, (LPARAM)L"Uranus");
	SendMessage(hWndCombo1, CB_ADDSTRING, 0, (LPARAM)L"Neptune");
	SendMessage(hWndCombo1, CB_ADDSTRING, 0, (LPARAM)L"Pluto");
	SendMessage(hWndCombo1, CB_ADDSTRING, 0, (LPARAM)L"Haumea");
	SendMessage(hWndCombo1, CB_ADDSTRING, 0, (LPARAM)L"Eris");
	SendMessage(hWndCombo1, CB_ADDSTRING, 0, (LPARAM)L"Makemake");

	SendMessage(hWndCombo2, CB_ADDSTRING, 0, (LPARAM)L"Ceres");
	SendMessage(hWndCombo2, CB_ADDSTRING, 0, (LPARAM)L"Pallas");
	SendMessage(hWndCombo2, CB_ADDSTRING, 0, (LPARAM)L"Vesta");
	SendMessage(hWndCombo2, CB_ADDSTRING, 0, (LPARAM)L"Gigea");
	SendMessage(hWndCombo2, CB_ADDSTRING, 0, (LPARAM)L"Euphrosyne");

    SendMessage(hWndCombo1, CB_SETCURSEL, 0, NULL);                 // Выбор текущего элемента списка
	SendMessage(hWndCombo2, CB_SETCURSEL, 0, NULL);                 // Выбор текущего элемента списка

	hWndTrackBar1 = CreateWindow(TRACKBAR_CLASS, NULL, WS_CHILD|WS_VISIBLE|TBS_HORZ|WS_DISABLED|TBS_TOOLTIPS,	maxX - 370, 140, 100, 30,	hWnd,(HMENU)ID_TrackBar1, hInst, NULL);
	hWndTrackBar3 = CreateWindow(TRACKBAR_CLASS, NULL, WS_CHILD|WS_VISIBLE|TBS_HORZ|WS_DISABLED|TBS_TOOLTIPS,	maxX - 180, 140, 100, 30,	hWnd,(HMENU)ID_TrackBar3, hInst, NULL);
	hWndTrackBar5 = CreateWindow(TRACKBAR_CLASS, NULL, WS_CHILD|WS_VISIBLE|TBS_HORZ|TBS_TOOLTIPS,				maxX - 370, 220, 310, 30,	hWnd,(HMENU)ID_TrackBar5, hInst, NULL);
	hWndTrackBar6 = CreateWindow(TRACKBAR_CLASS, NULL, WS_CHILD|WS_VISIBLE|TBS_HORZ|TBS_TOOLTIPS,				maxX - 370, 275, 310, 30,	hWnd,(HMENU)ID_TrackBar6, hInst, NULL);
	hWndTrackBar7 = CreateWindow(TRACKBAR_CLASS, NULL, WS_CHILD|WS_VISIBLE|TBS_HORZ|TBS_TOOLTIPS,				maxX - 370, 330, 310, 30,	hWnd,(HMENU)ID_TrackBar7, hInst, NULL);
	hWndTrackBar8 = CreateWindow(TRACKBAR_CLASS, NULL, WS_CHILD|WS_VISIBLE|TBS_HORZ|TBS_TOOLTIPS|TBS_AUTOTICKS,	maxX - 370, 385, 310, 30,	hWnd,(HMENU)ID_TrackBar8, hInst, NULL);
	
	SendMessage(hWndTrackBar1, TBM_SETRANGE,	(WPARAM) TRUE,	(LPARAM) MAKELONG(0, 10000));	// min. & max. positions
    SendMessage(hWndTrackBar1, TBM_SETPAGESIZE, 0,				(LPARAM) 10);					// скорость перетаскивания бегунка
	SendMessage(hWndTrackBar1, TBM_SETSEL,		(WPARAM) FALSE,	(LPARAM) MAKELONG(0, 10)); 
	SendMessage(hWndTrackBar1, TBM_SETPOS,		(WPARAM) TRUE,	(LPARAM) 1000);					// положение бегунка
	
	SendMessage(hWndTrackBar3, TBM_SETRANGE,	(WPARAM) TRUE,	(LPARAM) MAKELONG(0, 5000));	// min. & max. positions
    SendMessage(hWndTrackBar3, TBM_SETPAGESIZE, 0,				(LPARAM) 10);					// скорость перетаскивания бегунка
	SendMessage(hWndTrackBar3, TBM_SETSEL,		(WPARAM) FALSE,	(LPARAM) MAKELONG(0, 10)); 
	SendMessage(hWndTrackBar3, TBM_SETPOS,		(WPARAM) TRUE,	(LPARAM) 1000);					// положение бегунка

	SendMessage(hWndTrackBar5, TBM_SETRANGE,	(WPARAM) TRUE,	(LPARAM) MAKELONG(0, 400));		// min. & max. positions
    SendMessage(hWndTrackBar5, TBM_SETPAGESIZE,	0,				(LPARAM) 10);					// скорость перетаскивания бегунка
	SendMessage(hWndTrackBar5, TBM_SETSEL,		(WPARAM) FALSE,	(LPARAM) MAKELONG(0, 100)); 
	SendMessage(hWndTrackBar5, TBM_SETPOS,		(WPARAM) TRUE,	(LPARAM) 10);					// положение бегунка

	SendMessage(hWndTrackBar6, TBM_SETRANGE,	(WPARAM) TRUE,	(LPARAM) MAKELONG(0, 180));		// min. & max. positions
    SendMessage(hWndTrackBar6, TBM_SETPAGESIZE, 0,				(LPARAM) 5);					// скорость перетаскивания бегунка
	SendMessage(hWndTrackBar6, TBM_SETSEL,		(WPARAM) FALSE,	(LPARAM) MAKELONG(0, 10)); 
	SendMessage(hWndTrackBar6, TBM_SETPOS,		(WPARAM) TRUE,	(LPARAM) 0);					// положение бегунка

	SendMessage(hWndTrackBar7, TBM_SETRANGE,	(WPARAM) TRUE,	(LPARAM) MAKELONG(0, 400));	// min. & max. positions
    SendMessage(hWndTrackBar7, TBM_SETPAGESIZE, 0,				(LPARAM) 1);					// скорость перетаскивания бегунка
	SendMessage(hWndTrackBar7, TBM_SETSEL,		(WPARAM) FALSE,	(LPARAM) MAKELONG(0, 10)); 
	SendMessage(hWndTrackBar7, TBM_SETPOS,		(WPARAM) TRUE,	(LPARAM) 100);					// положение бегунка

	SendMessage(hWndTrackBar8, TBM_SETRANGE,	(WPARAM) TRUE,	(LPARAM) MAKELONG(0, 40));		// min. & max. positions
    SendMessage(hWndTrackBar8, TBM_SETPAGESIZE, 0,				(LPARAM) 1);					// скорость перетаскивания бегунка
	SendMessage(hWndTrackBar8, TBM_SETSEL,		(WPARAM) FALSE,	(LPARAM) MAKELONG(0, 10)); 
	SendMessage(hWndTrackBar8, TBM_SETPOS,		(WPARAM) TRUE,	(LPARAM) 1);					// положение бегунка

	hWndEdit1 = CreateWindow(L"edit", L"1000",	WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_BORDER|ES_LEFT|ES_AUTOHSCROLL|WS_DISABLED,	maxX - 260,	143, 40, 20,	hWnd,(HMENU)ID_EDIT1, hInst, NULL);
	hWndEdit3 = CreateWindow(L"edit", L"1000",	WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_BORDER|ES_LEFT|ES_AUTOHSCROLL|WS_DISABLED,	maxX - 70,	143, 40, 20,	hWnd,(HMENU)ID_EDIT3, hInst, NULL);
	hWndEdit5 = CreateWindow(L"edit", L"10",	WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_BORDER|ES_LEFT|ES_AUTOHSCROLL,				maxX - 59,	225, 40, 20,	hWnd,(HMENU)ID_EDIT5, hInst, NULL);
	hWndEdit6 = CreateWindow(L"edit", L"0",		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_BORDER|ES_LEFT|ES_AUTOHSCROLL,				maxX - 59,	280, 40, 20,	hWnd,(HMENU)ID_EDIT6, hInst, NULL);
	hWndEdit7 = CreateWindow(L"edit", L"100",	WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_BORDER|ES_LEFT|ES_AUTOHSCROLL,				maxX - 59,	335, 40, 20,	hWnd,(HMENU)ID_EDIT7, hInst, NULL);
	hWndEdit8 = CreateWindow(L"edit", L"1",		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_BORDER|ES_LEFT|ES_AUTOHSCROLL,				maxX - 59,	390, 40, 20,	hWnd,(HMENU)ID_EDIT8, hInst, NULL);

	hWndStatic1 = CreateWindow(L"static",L"Mасса,10****кг",								WS_CHILD|WS_VISIBLE|SS_CENTER|WS_DISABLED,	maxX - 370, 120, 90, 15,	hWnd,(HMENU)ID_STATIC1,hInst,NULL);
	hWndStatic3 = CreateWindow(L"static",L"Mасса,10****кг",								WS_CHILD|WS_VISIBLE|SS_CENTER|WS_DISABLED,	maxX - 180, 120, 90, 15,	hWnd,(HMENU)ID_STATIC3,hInst,NULL);
	hWndStatic5 = CreateWindow(L"static",L"Скорость астероида (v), км/с",				WS_CHILD|WS_VISIBLE|SS_CENTER,				maxX - 360, 201, 145,15,	hWnd,(HMENU)ID_STATIC5,hInst,NULL);
	hWndStatic6 = CreateWindow(L"static",L"Угол между вектором скорости и направлением на планету",WS_CHILD|WS_VISIBLE|SS_CENTER,	maxX - 360, 256, 310,15,	hWnd,(HMENU)ID_STATIC6,hInst,NULL);
	hWndStatic7 = CreateWindow(L"static",L"Расстояние между цетрами тел (r), 10**км",	WS_CHILD|WS_VISIBLE|SS_CENTER,				maxX - 360, 311, 220,15,	hWnd,(HMENU)ID_STATIC7,hInst,NULL);
	hWndStatic8 = CreateWindow(L"static",L"Cкорость течения времени",					WS_CHILD|WS_VISIBLE|SS_CENTER,				maxX - 360, 366, 140,15,	hWnd,(HMENU)ID_STATIC8,hInst,NULL);

	hWndStatic1_ = CreateWindow(L"static",L"21",	WS_CHILD|WS_VISIBLE|SS_CENTER|WS_DISABLED,	maxX - 314, 115, 15, 15,	hWnd,(HMENU)ID_STATIC1_,hInst,NULL);
	hWndStatic3_ = CreateWindow(L"static",L"18",	WS_CHILD|WS_VISIBLE|SS_CENTER|WS_DISABLED,	maxX - 124, 115, 15, 15,	hWnd,(HMENU)ID_STATIC3_,hInst,NULL);
	hWndStatic7_ = CreateWindow(L"static",L"3",		WS_CHILD|WS_VISIBLE|SS_CENTER,				maxX - 164, 306, 7, 15,		hWnd,(HMENU)ID_STATIC7_,hInst,NULL);

	hWndGroupButton1 = CreateWindow(L"button", L"Планета",	WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|BS_GROUPBOX,	maxX - 375, 10,	 180, 165,	hWnd,(HMENU)0, hInst, NULL);
	hWndGroupButton2 = CreateWindow(L"button", L"Астероид",	WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|BS_GROUPBOX,	maxX - 185, 10,	 180, 165,	hWnd,(HMENU)0, hInst, NULL);
	hWndGroupButton3 = CreateWindow(L"button", L"Параметры",WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|BS_GROUPBOX,	maxX - 375, 180, 370, 240,	hWnd,(HMENU)0, hInst, NULL);

	FontWin32 = (WPARAM)GetStockObject( ANSI_VAR_FONT ); 
	SendMessage( hWndButton1,		WM_SETFONT, FontWin32, 0 );
	SendMessage( hWndButton2,		WM_SETFONT, FontWin32, 0 );
	SendMessage( hWndButton3,		WM_SETFONT, FontWin32, 0 );
	SendMessage( hWndButton4,		WM_SETFONT, FontWin32, 0 );

	SendMessage( hWndRadioButton1,	WM_SETFONT, FontWin32, 0 );
	SendMessage( hWndRadioButton2,	WM_SETFONT, FontWin32, 0 );
	SendMessage( hWndRadioButton3,	WM_SETFONT, FontWin32, 0 );
	SendMessage( hWndRadioButton4,	WM_SETFONT, FontWin32, 0 );

	SendMessage( hWndCombo1,		WM_SETFONT, FontWin32, 0 );
	SendMessage( hWndCombo2,		WM_SETFONT, FontWin32, 0 );
	
	SendMessage( hWndEdit1,			WM_SETFONT, FontWin32, 0 );
	SendMessage( hWndEdit3,			WM_SETFONT, FontWin32, 0 );
	SendMessage( hWndEdit5,			WM_SETFONT, FontWin32, 0 );
	SendMessage( hWndEdit6,			WM_SETFONT, FontWin32, 0 );
	SendMessage( hWndEdit7,			WM_SETFONT, FontWin32, 0 );
	SendMessage( hWndEdit8,			WM_SETFONT, FontWin32, 0 );
	
	SendMessage( hWndStatic1,		WM_SETFONT, FontWin32, 0 );
	SendMessage( hWndStatic3,		WM_SETFONT, FontWin32, 0 );
	SendMessage( hWndStatic5,		WM_SETFONT, FontWin32, 0 );
	SendMessage( hWndStatic6,		WM_SETFONT, FontWin32, 0 );
	SendMessage( hWndStatic7,		WM_SETFONT, FontWin32, 0 );
	SendMessage( hWndStatic8,		WM_SETFONT, FontWin32, 0 );
	
	SendMessage( hWndStatic1_,		WM_SETFONT, FontWin32, 0 );
	SendMessage( hWndStatic3_,		WM_SETFONT, FontWin32, 0 );
	SendMessage( hWndStatic7_,		WM_SETFONT, FontWin32, 0 );

	SendMessage( hWndGroupButton1,	WM_SETFONT, FontWin32, 0 );
	SendMessage( hWndGroupButton2,	WM_SETFONT, FontWin32, 0 );
	SendMessage( hWndGroupButton3,	WM_SETFONT, FontWin32, 0 );
}

void CTRL_INTERFACE::DataControl(Physics::System * sys) // получает данные oт всех контролов.
{
		wchar_t lpch[20];

		sys->t = 0;
		if(SendMessage(hWndRadioButton1, BM_GETCHECK, 0, 0) == BST_CHECKED)
		{
			GetWindowText(hWndCombo1, reinterpret_cast<LPWSTR>(lpch), 10);
					
			toSysPlmass ((char)lpch[0],(char)lpch[1], (char)lpch[2], sys);
		}
		else
		{
			toSysPlmassTB(	SendMessage (hWndTrackBar1, TBM_GETPOS, 0, 0),
							sys);
		}

		if(SendMessage(hWndRadioButton3, BM_GETCHECK, 0, 0) == BST_CHECKED)
		{
			GetWindowText(hWndCombo2, reinterpret_cast<LPWSTR>(lpch), 10);
					
			toSysAsmass ((char)lpch[0], sys);
		}
		else
		{
			toSysAsmassTB(	SendMessage (hWndTrackBar3, TBM_GETPOS, 0, 0),
							sys);
		}

		toSysVA(		SendMessage (hWndTrackBar5, TBM_GETPOS, 0, 0),
						SendMessage (hWndTrackBar6, TBM_GETPOS, 0, 0),
						sys);

		toSysR(			SendMessage (hWndTrackBar7, TBM_GETPOS, 0, 0),
						sys);
}

void CTRL_INTERFACE::Button1_reaction()
{
		#pragma region Enable
		EnableWindow(hWndButton1,		false);
		EnableWindow(hWndButton2,		false);

		if (SendMessage(hWndRadioButton1,BM_GETCHECK,0,0l) != 0)
		{
			EnableWindow(hWndCombo1,	false);
		}
		else
		{
			EnableWindow(hWndEdit1,		false);
			EnableWindow(hWndTrackBar1, false);
			EnableWindow(hWndStatic1,	false);
			EnableWindow(hWndStatic1_,	false);
		}

		if (SendMessage(hWndRadioButton3,BM_GETCHECK,0,0l) != 0)
		{
			EnableWindow(hWndCombo2,	false);
		}
		else
		{
			EnableWindow(hWndEdit3,		false);
			EnableWindow(hWndTrackBar3, false);
			EnableWindow(hWndStatic3,	false);
			EnableWindow(hWndStatic3_,	false);
		}

		EnableWindow(hWndEdit5,			false);
		EnableWindow(hWndEdit6,			false);
		EnableWindow(hWndEdit7,			false);
		EnableWindow(hWndRadioButton1,	false);
		EnableWindow(hWndRadioButton2,	false);
		EnableWindow(hWndRadioButton3,	false);
		EnableWindow(hWndRadioButton4,	false);
		EnableWindow(hWndTrackBar5,		false);
		EnableWindow(hWndTrackBar6,		false);
		EnableWindow(hWndTrackBar7,		false);
		EnableWindow(hWndStatic5,		false);
		EnableWindow(hWndStatic6,		false);
		EnableWindow(hWndStatic7,		false);
		EnableWindow(hWndStatic7_,		false);
		#pragma endregion
		traectory = new Physics::Vector[300000]();
		DataControl(sys);
		mass_volume = Traectory(traectory, *sys, k);
		ShowTraect = true;
		if(Graph1_size >= 0)
			ZeroMemory(&graph, sizeof(graph));
		change = false;
		#pragma region Enable
		EnableWindow(hWndButton1,		true);
		EnableWindow(hWndButton2,		true);

		if (SendMessage(hWndRadioButton1,BM_GETCHECK,0,0l) != 0)
		{
			EnableWindow(hWndCombo1,	true);
		}
		else
		{
			EnableWindow(hWndEdit1,		true);
			EnableWindow(hWndTrackBar1, true);
			EnableWindow(hWndStatic1,	true);
			EnableWindow(hWndStatic1_,	true);
		}

		if (SendMessage(hWndRadioButton3,BM_GETCHECK,0,0l) != 0)
		{
			EnableWindow(hWndCombo2,	true);
		}
		else
		{
			EnableWindow(hWndEdit3,		true);
			EnableWindow(hWndTrackBar3, true);
			EnableWindow(hWndStatic3,	true);
			EnableWindow(hWndStatic3_,	true);
		}

		EnableWindow(hWndEdit5,			true);
		EnableWindow(hWndEdit6,			true);
		EnableWindow(hWndEdit7,			true);
		EnableWindow(hWndRadioButton1,	true);
		EnableWindow(hWndRadioButton2,	true);
		EnableWindow(hWndRadioButton3,	true);
		EnableWindow(hWndRadioButton4,	true);
		EnableWindow(hWndTrackBar5,		true);
		EnableWindow(hWndTrackBar6,		true);
		EnableWindow(hWndTrackBar7,		true);
		EnableWindow(hWndStatic5,		true);
		EnableWindow(hWndStatic6,		true);
		EnableWindow(hWndStatic7,		true);
		EnableWindow(hWndStatic7_,		true);
		#pragma endregion
}

void CTRL_INTERFACE::Button2_reaction()
{
	GetWindowText(hWndButton2,  lpch, 10);

	if(!Go) 
	{
		if(lpch[0] == L'З' || lpch[0] == L'П' && lpch[1] == L'а')
		{
#pragma region Enable
		//EnableWindow(hWndButton3,		false);
		EnableWindow(hWndButton4,		true);
		EnableWindow(hWndButton1,		false);
		//EnableWindow(hWndButton2,		false);

		if (SendMessage(hWndRadioButton1,BM_GETCHECK,0,0l) != 0)
		{
			EnableWindow(hWndCombo1,	false);
		}
		else
		{
			EnableWindow(hWndEdit1,		false);
			EnableWindow(hWndTrackBar1, false);
			EnableWindow(hWndStatic1,	false);
			EnableWindow(hWndStatic1_,	false);
		}

		if (SendMessage(hWndRadioButton3,BM_GETCHECK,0,0l) != 0)
		{
			EnableWindow(hWndCombo2,	false);
		}
		else
		{
			EnableWindow(hWndEdit3,		false);
			EnableWindow(hWndTrackBar3, false);
			EnableWindow(hWndStatic3,	false);
			EnableWindow(hWndStatic3_,	false);
		}

		EnableWindow(hWndEdit5,			false);
		EnableWindow(hWndEdit6,			false);
		EnableWindow(hWndEdit7,			false);
		EnableWindow(hWndRadioButton1,	false);
		EnableWindow(hWndRadioButton2,	false);
		EnableWindow(hWndRadioButton3,	false);
		EnableWindow(hWndRadioButton4,	false);
		EnableWindow(hWndTrackBar5,		false);
		EnableWindow(hWndTrackBar6,		false);
		EnableWindow(hWndTrackBar7,		false);
		EnableWindow(hWndStatic5,		false);
		EnableWindow(hWndStatic6,		false);
		EnableWindow(hWndStatic7,		false);
		EnableWindow(hWndStatic7_,		false);
#pragma endregion			
			if(change)
			{
				DataControl(sys);
				if(mass_volume && ShowTraect)
					mass_volume = Traectory(traectory, *sys, k);
				if(Graph3Open)
					a_max = GetMaxA(mass_volume, traectory, sys->M + sys->m, sys->R + sys->Ra)/3600;
				if(Graph1_size >= 0)
					ZeroMemory(&graph, sizeof(graph));
				change = false;
			}
		}
		SetWindowText(hWndButton2, L"Пауза");
			Go = true;
	}
	else
	{
		Go = false;
		SetWindowText(hWndButton2, L"Продолжить");
	}
}

void CTRL_INTERFACE::Button3_reaction()
{
	ctrl_interface->CreateSettingPanel();// окно настроек.
}

void CTRL_INTERFACE::Button4_reaction()
{
#pragma region Enable
	EnableWindow(hWndButton1,		true);
	EnableWindow(hWndButton2,		true);
	//EnableWindow(hWndButton3,		true);
	EnableWindow(hWndButton4,		false);

	if (SendMessage(hWndRadioButton1,BM_GETCHECK,0,0l) != 0)
	{
		EnableWindow(hWndCombo1,	true);
	}
	else
	{
		EnableWindow(hWndEdit1,		true);
		EnableWindow(hWndTrackBar1, true);
		EnableWindow(hWndStatic1,	true);
		EnableWindow(hWndStatic1_,	true);
	}

	if (SendMessage(hWndRadioButton3,BM_GETCHECK,0,0l) != 0)
	{
		EnableWindow(hWndCombo2,	true);
	}
	else
	{
		EnableWindow(hWndEdit3,		true);
		EnableWindow(hWndTrackBar3, true);
		EnableWindow(hWndStatic3,	true);
		EnableWindow(hWndStatic3_,	true);
	}

	EnableWindow(hWndEdit5,			true);
	EnableWindow(hWndEdit6,			true);
	EnableWindow(hWndEdit7,			true);
	EnableWindow(hWndRadioButton1,	true);
	EnableWindow(hWndRadioButton2,	true);
	EnableWindow(hWndRadioButton3,	true);
	EnableWindow(hWndRadioButton4,	true);
	EnableWindow(hWndTrackBar5,		true);
	EnableWindow(hWndTrackBar6,		true);
	EnableWindow(hWndTrackBar7,		true);
	EnableWindow(hWndStatic5,		true);
	EnableWindow(hWndStatic6,		true);
	EnableWindow(hWndStatic7,		true);
	EnableWindow(hWndStatic7_,		true);
#pragma endregion
	Go = false;
	SetWindowText(hWndButton2, L"Запустить");
}

void CTRL_INTERFACE::RadioButton1_reaction(int wmEvent)
{
	if(wmEvent == BN_CLICKED) 
		{
			EnableWindow(hWndCombo1,	true);
			EnableWindow(hWndEdit1,		false);
			EnableWindow(hWndTrackBar1, false);
			EnableWindow(hWndStatic1,	false);
			EnableWindow(hWndStatic1_,	false);
		}

		GetWindowText(hWndCombo1, reinterpret_cast<LPWSTR>(lpch), 10);
		toSysPlmass ((char)lpch[0],(char)lpch[1], (char)lpch[2], sys);

		TEXTURECTRL -> set_Planet_texture((char) lpch[0], (char) lpch[1], (char) lpch[2]);

		change = true;
}

void CTRL_INTERFACE::RadioButton2_reaction(int wmEvent)
{
	if(wmEvent == BN_CLICKED)
	{
		EnableWindow(hWndCombo1,	false);
		EnableWindow(hWndEdit1,		true);
		EnableWindow(hWndTrackBar1, true);
		EnableWindow(hWndStatic1,	true);
		EnableWindow(hWndStatic1_,	true);
	}
	toSysPlmassTB(
				SendMessage (hWndTrackBar1, TBM_GETPOS, 0, 0),
				sys);

	TEXTURECTRL -> set_Planet_texture(0,0,0);

	change = true;
}

void CTRL_INTERFACE::RadioButton3_reaction(int wmEvent)
{
	if(wmEvent == BN_CLICKED) 
	{
		EnableWindow(hWndCombo2,	true);
		EnableWindow(hWndEdit3,		false);
		EnableWindow(hWndTrackBar3, false);
		EnableWindow(hWndStatic3,	false);
		EnableWindow(hWndStatic3_,	false);
	}

	GetWindowText(hWndCombo2, reinterpret_cast<LPWSTR>(lpch), 10);
	toSysAsmass ((char)lpch[0], sys);

	TEXTURECTRL -> set_Asteroid_texture((char) lpch[0]);

	change = true;
}

void CTRL_INTERFACE::RadioButton4_reaction(int wmEvent)
{
	if(wmEvent == BN_CLICKED) 
	{
		EnableWindow(hWndCombo2,	false);
		EnableWindow(hWndEdit3,		true);
		EnableWindow(hWndTrackBar3, true);
		EnableWindow(hWndStatic3,	true);
		EnableWindow(hWndStatic3_,	true);
	}
	toSysAsmassTB(
				SendMessage (hWndTrackBar3, TBM_GETPOS, 0, 0),
				sys);

	TEXTURECTRL ->  set_Asteroid_texture(0);

	change = true;
}

void CTRL_INTERFACE::ComboBox1_reaction(int wmEvent)
{
	if(wmEvent == LBN_SELCHANGE)//по нажатию левой кнопки мыши
	{
		GetWindowText(hWndCombo1, reinterpret_cast<LPWSTR>(lpch), 10);
		toSysPlmass ((char)lpch[0],(char)lpch[1], (char)lpch[2], sys);

		TEXTURECTRL -> set_Planet_texture((char) lpch[0], (char) lpch[1], (char) lpch[2]);
	}

	change = true;
}

void CTRL_INTERFACE::ComboBox2_reaction(int wmEvent)
{
	if(wmEvent == LBN_SELCHANGE)
	{
		GetWindowText(hWndCombo2, reinterpret_cast<LPWSTR>(lpch), 10);
		toSysAsmass ((char)lpch[0], sys);

		TEXTURECTRL -> set_Asteroid_texture((char) lpch[0]);
	}

	change = true;
}

void CTRL_INTERFACE::Edit1_reaction(int wmEvent)
{
	if(wmEvent==EN_CHANGE)
	{
		if(beg[0])																	//первый раз обрабатывается при инициализации окна
		{
			GetWindowText(hWndEdit1, lpch, 10);										//получение значения

			par = _wtoi(lpch);														// преобразование в int 

			if( par > (int)SendMessage(hWndTrackBar1, TBM_GETRANGEMAX, 0, 0))		// обработка выхода за пределы.
			{
				par = (int)SendMessage(hWndTrackBar1, TBM_GETRANGEMAX, 0, 0);

				_itow_s ( par, lpch, 10 );

				SetWindowText(hWndEdit1, lpch);
			}

			if( par < (int)SendMessage(hWndTrackBar1, TBM_GETRANGEMIN, 0, 0))
			{
				par = (int)SendMessage(hWndTrackBar1, TBM_GETRANGEMIN, 0, 0);

				_itow_s ( par, lpch, 10 );

				SetWindowText(hWndEdit1, lpch);
			}

			SendMessage(
									hWndTrackBar1, TBM_SETPOS,						//передача в trackbar значения
									(WPARAM) TRUE,(LPARAM) par);

			toSysPlmassTB(
									SendMessage (hWndTrackBar1, TBM_GETPOS, 0, 0),	// передача в sys
									sys);
		}
		else
			beg[0] = true;
	}

	change = true;
}

void CTRL_INTERFACE::Edit3_reaction(int wmEvent)
{
	if(wmEvent==EN_CHANGE)
	{
		if(beg[1])
		{
			GetWindowText(hWndEdit3, lpch, 10);

			par = _wtoi(lpch);

			if( par > (int)SendMessage(hWndTrackBar3, TBM_GETRANGEMAX, 0, 0))
			{
				par = (int)SendMessage(hWndTrackBar3, TBM_GETRANGEMAX, 0, 0);

				_itow_s ( par, lpch, 10 );

				SetWindowText(hWndEdit3, lpch);
			}

			if( par < (int)SendMessage(hWndTrackBar3, TBM_GETRANGEMIN, 0, 0))
			{
				par = (int)SendMessage(hWndTrackBar3, TBM_GETRANGEMIN, 0, 0);

				_itow_s ( par, lpch, 10 );

				SetWindowText(hWndEdit3, lpch);
			}

			SendMessage(
									hWndTrackBar3, TBM_SETPOS, 
									(WPARAM) TRUE,(LPARAM) par);

			toSysAsmassTB(
									SendMessage (hWndTrackBar3, TBM_GETPOS, 0, 0),
									sys);
		}
		else
			beg[1] = true;
	}

	change = true;
}

void CTRL_INTERFACE::Edit5_reaction(int wmEvent)
{
	if(wmEvent==EN_CHANGE)
	{
		if(beg[2])
		{
			GetWindowText(hWndEdit5, lpch, 10);

			par = _wtoi(lpch);

			if( par > (int)SendMessage(hWndTrackBar5, TBM_GETRANGEMAX, 0, 0))
			{
				par = (int)SendMessage(hWndTrackBar5, TBM_GETRANGEMAX, 0, 0);

				_itow_s ( par, lpch, 10 );

				SetWindowText(hWndEdit5, lpch);
			}

			if( par < (int)SendMessage(hWndTrackBar5, TBM_GETRANGEMIN, 0, 0))
			{
				par = (int)SendMessage(hWndTrackBar5, TBM_GETRANGEMIN, 0, 0);

				_itow_s ( par, lpch, 10 );

				SetWindowText(hWndEdit5, lpch);
			}

			SendMessage(
									hWndTrackBar5, TBM_SETPOS, 
									(WPARAM) TRUE,(LPARAM) par);

			toSysVA(
									SendMessage (hWndTrackBar5, TBM_GETPOS, 0, 0),
									SendMessage (hWndTrackBar6, TBM_GETPOS, 0, 0),
									sys);
		}
		else
			beg[2] = true;
	}

	change = true;
}

void CTRL_INTERFACE::Edit6_reaction(int wmEvent)
{
	if(wmEvent==EN_CHANGE)
	{
		if(beg[3])
		{
			GetWindowText(hWndEdit6, lpch, 10);

			par = _wtoi(lpch);

			if( par > (int)SendMessage(hWndTrackBar6, TBM_GETRANGEMAX, 0, 0))
			{
				par = (int)SendMessage(hWndTrackBar6, TBM_GETRANGEMAX, 0, 0);

				_itow_s ( par, lpch, 10 );

				SetWindowText(hWndEdit6, lpch);
			}

			if( par < (int)SendMessage(hWndTrackBar6, TBM_GETRANGEMIN, 0, 0))
			{
				par = (int)SendMessage(hWndTrackBar6, TBM_GETRANGEMIN, 0, 0);

				_itow_s ( par, lpch, 10 );

				SetWindowText(hWndEdit6, lpch);
			}

			SendMessage(
									hWndTrackBar6, TBM_SETPOS, 
									(WPARAM) TRUE,(LPARAM) par);
			toSysVA(
									SendMessage (hWndTrackBar5, TBM_GETPOS, 0, 0),
									SendMessage (hWndTrackBar6, TBM_GETPOS, 0, 0),
									sys);
		}
		else
			beg[3] = true;
	}

	change = true;
}

void CTRL_INTERFACE::Edit7_reaction(int wmEvent)
{
	if(wmEvent==EN_CHANGE)
	{
		if(beg[4])
		{
			GetWindowText(hWndEdit7, lpch, 10);

			par = _wtoi(lpch);

			if( par > (int)SendMessage(hWndTrackBar7, TBM_GETRANGEMAX, 0, 0))
			{
				par = (int)SendMessage(hWndTrackBar7, TBM_GETRANGEMAX, 0, 0);

				_itow_s ( par, lpch, 10 );

				SetWindowText(hWndEdit7, lpch);
			}

			if( par < (int)SendMessage(hWndTrackBar7, TBM_GETRANGEMIN, 0, 0))
			{
				par = (int)SendMessage(hWndTrackBar7, TBM_GETRANGEMIN, 0, 0);

				_itow_s ( par, lpch, 10 );

				SetWindowText(hWndEdit7, lpch);
			}

			SendMessage(
									hWndTrackBar7, TBM_SETPOS,
									(WPARAM) TRUE,(LPARAM) par);

			toSysR(
									SendMessage (hWndTrackBar7, TBM_GETPOS, 0, 0),
									sys);
		}
		else
			beg[4] = true;
	}

	change = true;
}

void CTRL_INTERFACE::Edit8_reaction(int wmEvent)
{
	if(wmEvent==EN_CHANGE)
	{
		if(beg[5])
		{
			GetWindowText(hWndEdit8, lpch, 10);

			par = _wtoi(lpch);

			if( par > (int)SendMessage(hWndTrackBar8, TBM_GETRANGEMAX, 0, 0))
			{
				par = (int)SendMessage(hWndTrackBar8, TBM_GETRANGEMAX, 0, 0);

				_itow_s ( par, lpch, 10 );

				SetWindowText(hWndEdit8, lpch);
			}

			if( par < (int)SendMessage(hWndTrackBar8, TBM_GETRANGEMIN, 0, 0))
			{
				par = (int)SendMessage(hWndTrackBar8, TBM_GETRANGEMIN, 0, 0);

				_itow_s ( par, lpch, 10 );

				SetWindowText(hWndEdit8, lpch);
			}

			SendMessage(
									hWndTrackBar8, TBM_SETPOS,
									(WPARAM) TRUE,(LPARAM) par);

			timespeed = (int) SendMessage (hWndTrackBar8, TBM_GETPOS, 0, 0);
		}
		else
			beg[5] = true;
	}
}

void CTRL_INTERFACE::AllTrackBar_reaction(LPARAM lParam)
{
		if( (HWND)lParam == GetDlgItem (hWnd, ID_TrackBar1) )					// 1
		{
				toSysPlmassTB(
								SendMessage (hWndTrackBar1, TBM_GETPOS, 0, 0),	//занесение значения в sys
								sys);

				_itow_s(
								SendMessage (hWndTrackBar1, TBM_GETPOS, 0, 0),	//перевод в строку
								lpch, 10);
			
				SendMessage( 
								hWndEdit1, WM_SETTEXT, (WPARAM) TRUE,			//помещение ее в edit.
								(LPARAM)lpch);
		}  
		else
		if( (HWND)lParam == GetDlgItem (hWnd, ID_TrackBar3) )					// 3
		{
				toSysAsmassTB(
								SendMessage (hWndTrackBar3, TBM_GETPOS, 0, 0),
								sys);

				_itow_s(
								SendMessage (hWndTrackBar3, TBM_GETPOS, 0, 0), 
								lpch, 10);
			
				SendMessage( 
								hWndEdit3, WM_SETTEXT, (WPARAM) TRUE, 
								(LPARAM)lpch);
		}  
		else
		if( (HWND)lParam == GetDlgItem (hWnd, ID_TrackBar5) )					// 5
		{
				toSysVA(
								SendMessage (hWndTrackBar5, TBM_GETPOS, 0, 0),
								SendMessage (hWndTrackBar6, TBM_GETPOS, 0, 0),
								sys);
				_itow_s(
								SendMessage (hWndTrackBar5, TBM_GETPOS, 0, 0), 
								lpch, 10);
			
				SendMessage( 
								hWndEdit5, WM_SETTEXT, (WPARAM) TRUE, 
								(LPARAM)lpch);
		}  
		else
		if( (HWND)lParam == GetDlgItem (hWnd, ID_TrackBar6) )					// 6
		{
				toSysVA(
								SendMessage (hWndTrackBar5, TBM_GETPOS, 0, 0),
								SendMessage (hWndTrackBar6, TBM_GETPOS, 0, 0),
								sys);

				_itow_s(
								SendMessage (hWndTrackBar6, TBM_GETPOS, 0, 0), 
								lpch, 10);
			
				SendMessage( 
								hWndEdit6, WM_SETTEXT, (WPARAM) TRUE, 
								(LPARAM)lpch);
		}  
		else
		if( (HWND)lParam == GetDlgItem (hWnd, ID_TrackBar7) )					// 7
		{
				r_norm_base = SendMessage (hWndTrackBar7, TBM_GETPOS, 0, 0);
				toSysR(
								r_norm_base,
								sys);

				_itow_s(
								SendMessage (hWndTrackBar7, TBM_GETPOS, 0, 0), 
								lpch, 10);
			
				SendMessage( 
								hWndEdit7, WM_SETTEXT, (WPARAM) TRUE, 
								(LPARAM)lpch);
		}
		else
		if( (HWND)lParam == GetDlgItem (hWnd, ID_TrackBar8) )					// 8
		{
				timespeed = SendMessage (hWndTrackBar8, TBM_GETPOS, 0, 0);

				_itow_s(
								SendMessage (hWndTrackBar8, TBM_GETPOS, 0, 0), 
								lpch, 10);
			
				SendMessage( 
								hWndEdit8, WM_SETTEXT, (WPARAM) TRUE, 
								(LPARAM)lpch);
		}
}

void CTRL_INTERFACE::Size_reaction(int maxX, int maxY)
{
		MoveWindow(hWndDraw,			0, 0, maxX - 400, maxY,		true);

		MoveWindow(hWndButton1,			maxX - 355, 432, 140, 40,	true);
		MoveWindow(hWndButton2,			maxX - 355, 482, 140, 40,	true);
		MoveWindow(hWndButton3,			maxX - 165, 432, 140, 40,	true);
		MoveWindow(hWndButton4,			maxX - 165, 482, 140, 40,	true);

		MoveWindow(hWndCombo1,			maxX - 370, 60, 130, 300,	true);
		MoveWindow(hWndCombo2,			maxX - 180, 60, 130, 300,	true);

		MoveWindow(hWndRadioButton1,	maxX - 370, 30, 70, 20,		true);
		MoveWindow(hWndRadioButton2,	maxX - 370, 90, 50, 20,		true);
		MoveWindow(hWndRadioButton3,	maxX - 180, 30, 72, 20,		true);
		MoveWindow(hWndRadioButton4,	maxX - 180, 90, 50, 20,		true);

		MoveWindow(hWndEdit1,			maxX - 260, 143, 40, 20,	true);
		MoveWindow(hWndEdit3,			maxX - 70,  143, 40, 20,	true);
		MoveWindow(hWndEdit5,			maxX - 59,  225, 40, 20,	true);
		MoveWindow(hWndEdit6,			maxX - 59,  280, 40, 20,	true);
		MoveWindow(hWndEdit7,			maxX - 59,  335, 40, 20,	true);
		MoveWindow(hWndEdit8,			maxX - 59,  390, 40, 20,	true);

		MoveWindow(hWndStatic1,			maxX - 370, 120, 90,  15,	true);
		MoveWindow(hWndStatic3,			maxX - 180, 120, 90,  15,	true);
		MoveWindow(hWndStatic5,			maxX - 360, 201, 150, 15,	true);
		MoveWindow(hWndStatic6,			maxX - 360, 256, 310,  15,	true);
		MoveWindow(hWndStatic7,			maxX - 360, 311, 220, 15,	true);
		MoveWindow(hWndStatic8,			maxX - 360, 366, 140, 15,	true);

		MoveWindow(hWndStatic1_,			maxX - 314, 115, 15, 15,	true);
		MoveWindow(hWndStatic3_,			maxX - 124, 115, 15, 15,	true);
		MoveWindow(hWndStatic7_,			maxX - 164, 306, 7, 15,		true);

		MoveWindow(hWndTrackBar1,		maxX - 370, 140, 100, 30,	true);
		MoveWindow(hWndTrackBar3,		maxX - 180, 140, 100, 30,	true);
		MoveWindow(hWndTrackBar5,		maxX - 370, 220, 310, 30,	true);
		MoveWindow(hWndTrackBar6,		maxX - 370, 275, 310, 30,	true);
		MoveWindow(hWndTrackBar7,		maxX - 370, 330, 310, 30,	true);
		MoveWindow(hWndTrackBar8,		maxX - 370, 385, 310, 30,	true);

		MoveWindow(hWndGroupButton1,	maxX - 375, 10,  180, 165,	true);
		MoveWindow(hWndGroupButton2,	maxX - 185, 10,  180, 165,	true);
		MoveWindow(hWndGroupButton3,	maxX - 375, 180, 370, 240,	true);	
}

void CTRL_INTERFACE::CreateSettingPanel()
{
	InitCommonControls();
	wchar_t lpch[20];
	/*
	#define ID_BUTTON_OK	39
	#define ID_TBKam		40
	#define ID_TBPart		41
	#define ID_EdKam		42
	#define ID_EdPart		43
	#define ID_ChNetwork	46
	#define ID_ChSpeed		47
	#define ID_StKam		48
	#define ID_StPart		49
	#define ID_ChTexture	50

	HWND hWndSetting		= NULL;
	HWND hWndTBKam			= NULL;
	HWND hWndTBPart			= NULL;
	HWND hWndBSet			= NULL;
	HWND hWndEdKam			= NULL;
	HWND hWndEdPart			= NULL;

	HWND hWndChNetwork		= NULL;
	HWND hWndChSpeed		= NULL;
	HWND hWndStKam			= NULL;
	HWND hWndStPart			= NULL;
	HWND hWndGrGrav         = NULL;
	HWND hWndGrPhys         = NULL;
	HWND hWndChTexture		= NULL;
	HWND hWndBCancel		= NULL;
	*/
		hWndSetting = CreateWindow(L"Grafic", L"Настройки", WS_OVERLAPPEDWINDOW, 400, 350, 514, 250, NULL, NULL, hInst,0);

		SetWindowLong(hWndSetting,GWL_WNDPROC,(LONG)WndProc2);

		//hWndTB&Ed
		hWndTBKam = CreateWindow(	TRACKBAR_CLASS, NULL, WS_CHILD|WS_VISIBLE|TBS_HORZ|TBS_TOOLTIPS,
									20, 150, 150, 30,	
									hWndSetting,(HMENU)ID_TBKam, hInst, NULL);
			SendMessage(hWndTBKam, TBM_SETRANGE,	(WPARAM) TRUE,	(LPARAM) MAKELONG(0, 500));	// min. & max. positions
			SendMessage(hWndTBKam, TBM_SETPAGESIZE, 0,				(LPARAM) 10);					// скорость перетаскивания бегунка
			SendMessage(hWndTBKam, TBM_SETSEL,		(WPARAM) FALSE,	(LPARAM) MAKELONG(0, 10));

		_itow_s((int) kSpeed, lpch, 10);

		hWndEdKam = CreateWindow(	L"edit", lpch,	WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_BORDER|ES_LEFT|ES_AUTOHSCROLL,	
									180, 150, 40, 20,	
									hWndSetting,(HMENU)ID_EdKam, hInst, NULL);
		ZeroMemory(lpch, sizeof(lpch));

		hWndTBPart = CreateWindow(	TRACKBAR_CLASS, NULL, WS_CHILD|WS_VISIBLE|TBS_HORZ|TBS_TOOLTIPS,
									270, 60, 150, 30,	
									hWndSetting,(HMENU)ID_TBPart, hInst, NULL);
			SendMessage(hWndTBPart, TBM_SETRANGE,	(WPARAM) TRUE,	(LPARAM) MAKELONG(0, 10000));	// min. & max. positions
			SendMessage(hWndTBPart, TBM_SETPAGESIZE, 0,				(LPARAM) 10);					// скорость перетаскивания бегунка
			SendMessage(hWndTBPart, TBM_SETSEL,		(WPARAM) FALSE,	(LPARAM) MAKELONG(0, 10));

		_itow_s((int)(k*1e7), lpch, 10);

		hWndEdPart = CreateWindow(	L"edit", lpch,	WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_BORDER|ES_LEFT|ES_AUTOHSCROLL,	
									430, 60, 40, 20,	
									hWndSetting,(HMENU)ID_EdPart, hInst, NULL);
		//~hWndTB&Ed

		//hWndB&Rb&ch
		hWndBSet = CreateWindow(	L"button",L"Оk",		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|BS_PUSHBUTTON,				
									260, 145, 110, 40,	
									hWndSetting,(HMENU)ID_BUTTON_OK,hInst,NULL);
		hWndBCancel = CreateWindow(	L"button",L"Отмена",		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|BS_PUSHBUTTON,				
									380, 145, 110, 40,	
									hWndSetting,(HMENU)ID_BCancel,hInst,NULL);
		hWndChNetwork = CreateWindow( L"button", L"Сетка",	WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS| BS_CHECKBOX, 
															20, 30, 200, 20, hWndSetting, (HMENU)ID_ChNetwork, hInst, NULL);
		hWndChTexture = CreateWindow( L"button", L"Фоновая текстура",	WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS| BS_CHECKBOX, 
															20, 60, 200, 20, hWndSetting, (HMENU)ID_ChTexture, hInst, NULL);
		hWndChSpeed =  CreateWindow( L"button", L"Вектор скорости",		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS| BS_CHECKBOX,
															20, 90, 200, 20, hWndSetting, (HMENU)ID_ChSpeed, hInst, NULL);
				
		if(SpeedVect)
			CheckDlgButton(hWndSetting, ID_ChSpeed, BST_CHECKED);
		if(TextureCheck)
			CheckDlgButton(hWndSetting, ID_ChTexture, BST_CHECKED);
		if(network)
			CheckDlgButton(hWndSetting, ID_ChNetwork, BST_CHECKED);
		//~hWndB&Rb&ch

		//Static
		hWndStKam  = CreateWindow(L"static",L"Скорость камеры", WS_CHILD|WS_VISIBLE|SS_CENTER,	
															20, 130, 100, 15,	hWndSetting,(HMENU)ID_StKam,hInst,NULL);
		hWndStPart = CreateWindow(L"static",L"Величина разбиения", WS_CHILD|WS_VISIBLE|SS_CENTER,	
															270, 40, 120, 15,	hWndSetting,(HMENU)ID_StPart,hInst,NULL);
		//~Static

		//Group
		hWndGrGrav = CreateWindow(L"button", L"Графика",	WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|BS_GROUPBOX,	
															10, 10,	 240, 190,	hWndSetting,(HMENU)0, hInst, NULL);
		hWndGrPhys = CreateWindow(L"button", L"Физика",		WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|BS_GROUPBOX,	
															260, 10, 230, 100,	hWndSetting,(HMENU)0, hInst, NULL);
		//~Group

		if(Go)
		{
			EnableWindow(hWndTBPart, false);
			EnableWindow(hWndEdPart, false);
			EnableWindow(hWndStPart, false);
		}

		SendMessage( hWndEdKam,		WM_SETFONT, FontWin32, 0 );
		SendMessage( hWndEdPart,	WM_SETFONT, FontWin32, 0 );
		SendMessage( hWndBSet,		WM_SETFONT, FontWin32, 0 );
		SendMessage( hWndBCancel,	WM_SETFONT, FontWin32, 0 );
		SendMessage( hWndChNetwork,	WM_SETFONT, FontWin32, 0 );
		SendMessage( hWndChTexture,	WM_SETFONT, FontWin32, 0 );
		SendMessage( hWndChSpeed,	WM_SETFONT, FontWin32, 0 );
		SendMessage( hWndStKam,		WM_SETFONT, FontWin32, 0 );
		SendMessage( hWndStPart,	WM_SETFONT, FontWin32, 0 );
		SendMessage( hWndGrGrav,	WM_SETFONT, FontWin32, 0 );
		SendMessage( hWndGrPhys,	WM_SETFONT, FontWin32, 0 );

		SendMessage(hWndTBPart,	TBM_SETPOS,		(WPARAM) TRUE,	(LPARAM) (int)(k*1e7));
		SendMessage(hWndTBKam,	TBM_SETPOS,		(WPARAM) TRUE,	(LPARAM) (DWORD)kSpeed);

		ShowWindow(hWndSetting, 5);
}

LRESULT CALLBACK CTRL_INTERFACE::SettingMsg_reaction(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	int par;
	wchar_t lpch[20];

	wmId    = LOWORD(wParam);
	wmEvent = HIWORD(wParam);
	switch (msg)
	{
	case WM_DESTROY:
		CloseWindow(hWnd);
		return 0;
	case WM_COMMAND:
			switch (wmId)
			{
			case ID_BUTTON_OK:
				// снятие всех значений
				kSpeed = (GLfloat)	SendMessage (hWndTBKam, TBM_GETPOS, 0, 0);
				if(!Go)
					k = (double)		SendMessage (hWndTBPart, TBM_GETPOS, 0, 0) / 10000000;

				network			= (bool)IsDlgButtonChecked(hWnd, ID_ChNetwork);
				SpeedVect		= (bool)IsDlgButtonChecked(hWnd, ID_ChSpeed);
				TextureCheck	= (bool)IsDlgButtonChecked(hWnd, ID_ChTexture);
				
				DestroyWindow(hWnd);
				return 0;

			// обеспечение синхронизации полей ввода и бегунков
			case ID_EdKam:		
				if(wmEvent==EN_CHANGE)
				{
						GetWindowText(hWndEdKam, lpch, 10);										//получение значения

						par = _wtoi(lpch);														// преобразование в int 

						if( par > (int)SendMessage(hWndTBKam, TBM_GETRANGEMAX, 0, 0))		// обработка выхода за пределы.
						{
							par = (int)SendMessage(hWndTBKam, TBM_GETRANGEMAX, 0, 0);

							_itow_s ( par, lpch, 10 );

							SetWindowText(hWndEdKam, lpch);
						}

						if( par < (int)SendMessage(hWndTBKam, TBM_GETRANGEMIN, 0, 0))
						{
							par = (int)SendMessage(hWndTBKam, TBM_GETRANGEMIN, 0, 0);

							_itow_s ( par, lpch, 10 );

							SetWindowText(hWndEdKam, lpch);
						}

						SendMessage(
												hWndTBKam, TBM_SETPOS,						//передача в trackbar значения
												(WPARAM) TRUE,(LPARAM) par);
				}
				break;
			case ID_EdPart:
				if(wmEvent==EN_CHANGE)
				{
						GetWindowText(hWndEdPart, lpch, 10);										//получение значения

						par = _wtoi(lpch);														// преобразование в int 

						if( par > (int)SendMessage(hWndTBPart, TBM_GETRANGEMAX, 0, 0))		// обработка выхода за пределы.
						{
							par = (int)SendMessage(hWndTBPart, TBM_GETRANGEMAX, 0, 0);

							_itow_s ( par, lpch, 10 );

							SetWindowText(hWndEdPart, lpch);
						}

						if( par < (int)SendMessage(hWndTBPart, TBM_GETRANGEMIN, 0, 0))
						{
							par = (int)SendMessage(hWndTBPart, TBM_GETRANGEMIN, 0, 0);

							_itow_s ( par, lpch, 10 );

							SetWindowText(hWndEdPart, lpch);
						}

						SendMessage(
												hWndTBPart, TBM_SETPOS,						//передача в trackbar значения
												(WPARAM) TRUE,(LPARAM) par);
				}
				break;
			case ID_ChNetwork:
				if((bool)IsDlgButtonChecked(hWnd, ID_ChNetwork))

					CheckDlgButton(hWndSetting, ID_ChNetwork, BST_UNCHECKED);
				else
					CheckDlgButton(hWndSetting, ID_ChNetwork, BST_CHECKED);

				break;
			case ID_ChSpeed:
				if((bool)IsDlgButtonChecked(hWnd, ID_ChSpeed))

					CheckDlgButton(hWndSetting, ID_ChSpeed, BST_UNCHECKED);
				else
					CheckDlgButton(hWndSetting, ID_ChSpeed, BST_CHECKED);

				break;
			case ID_ChTexture:

				if((bool)IsDlgButtonChecked(hWnd, ID_ChTexture))

					CheckDlgButton(hWndSetting, ID_ChTexture, BST_UNCHECKED);
				else
					CheckDlgButton(hWndSetting, ID_ChTexture, BST_CHECKED);

				break;
			case ID_BCancel:
				// просто выход, закрытие окна.
				DestroyWindow(hWnd);
				return 0;
			}
		break;
	case WM_HSCROLL:
		// обеспечение синхронизации полей ввода и бегунков
		if( (HWND)lParam == GetDlgItem (hWnd, ID_TBKam) )
		{

				_itow_s(
								SendMessage (hWndTBKam, TBM_GETPOS, 0, 0),	//перевод в строку
								lpch, 10);
			
				SendMessage( 
								hWndEdKam, WM_SETTEXT, (WPARAM) TRUE,		//помещение ее в edit.
								(LPARAM)lpch);
		}  
		else
		if( (HWND)lParam == GetDlgItem (hWnd, ID_TBPart) )
		{
				_itow_s(
								SendMessage (hWndTBPart, TBM_GETPOS, 0, 0), 
								lpch, 10);
			
				SendMessage( 
								hWndEdPart, WM_SETTEXT, (WPARAM) TRUE, 
								(LPARAM)lpch);
		}  
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}