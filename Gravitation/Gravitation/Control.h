# include <stdio.h>
# include <stdlib.h>
# include "Texture.h"
# include "PhysicCtrl.h"

//////////////////////////////////////////////////
// �������������� �������� ��������� ����������	//
//////////////////////////////////////////////////
#pragma region id

#define ID_BUTTON1		1 // ������� ����������
#define ID_BUTTON2		2 // ������
#define ID_BUTTON3		3 // �� ���� �����
#define ID_BUTTON4		4 // ����������
#define ID_RADIOBUTTON1	6 // �����. ��.
#define ID_RADIOBUTTON2	7 // ���� ��.
#define ID_RADIOBUTTON3	8 // �����. ��������
#define ID_RADIOBUTTON4	9 // ���� ���.
#define ID_COMBO1		13 // ���� �������
#define ID_COMBO2		14 // ���� ��������
#define ID_TrackBar1	15 // ����� �������
#define ID_TrackBar3	17 // ����� ���������
#define ID_TrackBar5	19 // ��������
#define ID_TrackBar6	20 // ���� �
#define ID_TrackBar7	21 // ����������
#define ID_TrackBar8	22 // �������� �������
#define ID_EDIT1		23// ����� �������
#define ID_EDIT3		25// ����� ���������
#define ID_EDIT5		27// ��������
#define ID_EDIT6		28// ���� �
#define ID_EDIT7		29// ����������
#define ID_EDIT8		30// �������� �������

#define ID_STATIC1_		140
#define ID_STATIC3_		141
#define ID_STATIC7_		142

#define ID_STATIC1		31// ����� �������
#define ID_STATIC3		33// ����� ���������
#define ID_STATIC5		35// ��������
#define ID_STATIC6		36// ���� �
#define ID_STATIC7		37// ����������
#define ID_STATIC8		38// �������� �������
//////////////////////////////////////////////////////////////////
// � ��������� ����� ���������� ���������� � ��������� 2 � 4.	//
//////////////////////////////////////////////////////////////////

// for Setting Panel
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
#define ID_BCancel		51
#pragma endregion

// �������, �������� �������� ��� �� �������� ����.
void DrawBackground(HWND hWndBGR, int ID_BGR, HINSTANCE hInst);

enum ASNUM	{
			AsteroidID = 0,
			CeresID = 13, PallasID, VestaID,
			GigeaID, EuphrosyneID
			};

enum PLNUM	{
			AbstractID = 1,
			MercuryID = 3, VenusID, EarthID, MarsID, UranusID, 
			NeptuneID, PlutoID, HaumeaID, ErisID, MakemakeID
			};

// ������� �������.
// ��� ������ "���� ��������" ��� "���� �������"
// ����� ������������ ��� ��������.
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


// �������� ������ �������, ������������, ����� ������� �� ���������� ������.
// ������������ ������� ���������� 0, ���� ���� �� ���� ������� � ������� ������ ������.
class control
{
public:

bool keys[256];

bool	keys_summ();

};

// �����, ���������� ������� ������� � ������� ��� ������ � ����.
class TextureCtrl
{
public:

CTexture *Texture;
TextureImage textures [ 18 ];

//��������� �������� ��������
bool TextureLoad [ 18 ];

PLNUM planet_texture_nummer;
ASNUM asteroid_texture_nummer;

TextureCtrl()
{
	Texture = new CTexture();
	for(int i = 18 ; i >= 0; i-- ) TextureLoad[i] = false;
	planet_texture_nummer = MercuryID;
	asteroid_texture_nummer = CeresID;
};

void UpgateTexture();
void LoadFirstTexture();
void set_Planet_texture(char lpch_0, char lpch_1, char lpch_2);
void set_Asteroid_texture(char lpch_0);

};

// �����, �������������� ��������� ���������� ����������, �������� ������� ��� ������ � ����.
// ����� �������� ������� ��������� ��������� �� ���� ��������, � ��������� ����������, ����������� � ���� ����.
class CTRL_INTERFACE
{
private:
	wchar_t lpch[20];
	int par;
	//Window Font
	WPARAM FontWin32;
	//controls
	HWND hWndButton1;
	HWND hWndButton2;
	HWND hWndButton3;
	HWND hWndButton4;
	HWND hWndRadioButton1;
	HWND hWndRadioButton2;
	HWND hWndRadioButton3;
	HWND hWndRadioButton4;
	HWND hWndCombo1;
	HWND hWndCombo2;
	HWND hWndTrackBar1;
	HWND hWndTrackBar3;
	HWND hWndTrackBar5;
	HWND hWndTrackBar6;
	HWND hWndTrackBar7;
	HWND hWndTrackBar8;
	HWND hWndEdit1;
	HWND hWndEdit3;
	HWND hWndEdit5;
	HWND hWndEdit6;
	HWND hWndEdit7;
	HWND hWndEdit8;
	HWND hWndStatic1;
	HWND hWndStatic3;
	HWND hWndStatic5;
	HWND hWndStatic6;
	HWND hWndStatic7;
	HWND hWndStatic8;

	HWND hWndStatic1_;
	HWND hWndStatic3_;
	HWND hWndStatic7_;

	HWND hWndGroupButton1;
	HWND hWndGroupButton2;
	HWND hWndGroupButton3;

	//���������
	HWND hWndSetting;
	HWND hWndTBKam;
	HWND hWndTBPart;
	HWND hWndBSet;
	HWND hWndEdKam;
	HWND hWndEdPart;
	HWND hWndChNetwork;
	HWND hWndChSpeed;
	HWND hWndStKam;
	HWND hWndStPart;
	HWND hWndGrGrav;
	HWND hWndGrPhys;
	HWND hWndChTexture;
	HWND hWndBCancel;

public:
	CTRL_INTERFACE()
	{
		//controls
		hWndButton1		= NULL;
		hWndButton2		= NULL;
		hWndButton3		= NULL;
		hWndButton4		= NULL;
		hWndRadioButton1   = NULL;
		hWndRadioButton2   = NULL;
		hWndRadioButton3   = NULL;
		hWndRadioButton4   = NULL;
		hWndCombo1         = NULL;
		hWndCombo2         = NULL;
		hWndTrackBar1		= NULL;
		hWndTrackBar3		= NULL;
		hWndTrackBar5		= NULL;
		hWndTrackBar6		= NULL;
		hWndTrackBar7		= NULL;
		hWndTrackBar8		= NULL;
		hWndEdit1			= NULL;
		hWndEdit3			= NULL;
		hWndEdit5			= NULL;
		hWndEdit6			= NULL;
		hWndEdit7			= NULL;
		hWndEdit8			= NULL;
		hWndStatic1		= NULL;
		hWndStatic3		= NULL;
		hWndStatic5		= NULL;
		hWndStatic6		= NULL;
		hWndStatic7		= NULL;
		hWndStatic8		= NULL;

		hWndStatic1_		= NULL;
		hWndStatic3_		= NULL;
		hWndStatic7_		= NULL;

		hWndGroupButton1	= NULL;
		hWndGroupButton2	= NULL;
		hWndGroupButton3	= NULL;

		hWndSetting			= NULL;
		hWndTBKam			= NULL;
		hWndTBPart			= NULL;
		hWndBSet			= NULL;
		hWndEdKam			= NULL;
		hWndEdPart			= NULL;
		hWndChNetwork		= NULL;
		hWndChSpeed			= NULL;
		hWndStKam			= NULL;
		hWndStPart			= NULL;
		hWndGrGrav			= NULL;
		hWndGrPhys			= NULL;
		hWndChTexture		= NULL;
		hWndBCancel			= NULL;
	};

	void	Size_reaction(int, int);
	void	InitWndCtrls();
	void	DataControl(Physics::System * sys);
	void	CreateSettingPanel();
	//Button
	void	Button1_reaction();
	void	Button2_reaction();
	void	Button3_reaction();
	void	Button4_reaction();
	//RadioButton
	void	RadioButton1_reaction(int);
	void	RadioButton2_reaction(int);
	void	RadioButton3_reaction(int);
	void	RadioButton4_reaction(int);
	//ComboBox
	void	ComboBox1_reaction(int);
	void	ComboBox2_reaction(int);
	//Edit
	void	Edit1_reaction(int);
	void	Edit3_reaction(int);
	void	Edit5_reaction(int);
	void	Edit6_reaction(int);
	void	Edit7_reaction(int);
	void	Edit8_reaction(int);
	//TrackBar
	void	AllTrackBar_reaction(LPARAM);

	//Setting Panel
	LRESULT CALLBACK 	SettingMsg_reaction(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};