#include <math.h>

namespace Physics
{
	#define G 0.0000000000667259

#pragma region planet

	#define M_Mercury 0.32868e24 //����� � ������ ��������
	#define R_Mercury 2440

	#define M_Venus 4.81068e24 //����� � ������ ������
	#define R_Venus 6051

	#define M_Earth 5.976e24 //����� � ������ �����
	#define R_Earth 6378.137

	#define M_Mars 0.63345e24 //����� � ������ �����
	#define R_Mars 3396.9

	#define M_Uranus 86.0544e24 //����� � ������ �����
	#define R_Uranus 25560
	
	#define M_Neptune 101.592e24 //����� � ������ �������
	#define R_Neptune 25000
	
	#define M_Pluto 1.31e22 //����� � ������ �������
	#define R_Pluto 2306

	#define M_Haumea 4.2e21 //����� � ������ ������
	#define R_Haumea 1960

	#define M_Eris 1.67e22 //����� � ������ �����
	#define R_Eris 2397

	#define M_Makemake 3e21 //����� � ������ ��������
	#define R_Makemake 750

#pragma endregion
#pragma region asteroid

	#define M_Ceres			9.43e20	//������
	#define R_Ceres			454.7

	#define M_Pallas		9.43e20	//������� 
	#define R_Pallas		216
	
	#define M_Vesta			2.59e20	//�����
	#define R_Vesta			265

	#define M_Gigea			0.903e20	//�����
	#define R_Gigea			203
	
	#define M_Euphrosyne	0.169e20	//���������
	#define R_Euphrosyne	127.6
	        
#pragma endregion
	
// ����� ������ � �������� ��� ���
#pragma region Vector

	class Vector 
	{
	public:
		double x, y;
		
		inline double norm()
		{
			return sqrt((this->x)*(this->x) + (this->y)*(this->y));
		};
	};
	
	static Vector operator +(Vector lop, Vector rop) 
	{
		Vector t;
		t.x = lop.x + rop.x;
		t.y = lop.y + rop.y;
		return t;
	};
	static Vector operator -(Vector lop, Vector rop) 
	{
		Vector t;
		t.x = lop.x - rop.x;
		t.y = lop.y - rop.y;
		return t;
	};
	static Vector operator -(Vector a) 
	{
		Vector t;
		t.x = - a.x;
		t.y = - a.y;
		return t;
	};
	static Vector operator *(double lop, Vector rop) 
	{
		Vector t;
		t.x = lop * rop.x;
		t.y = lop * rop.y;
		return t;
	};
	static Vector operator /(Vector lop, double rop) 
	{
		Vector t;
		t.x = lop.x / rop;
		t.y = lop.y / rop;
		return t;
	};
	
	static Vector operator *(Vector lop, double rop) { return rop*lop; };
	static Vector operator *(int lop, Vector rop) { return (double)lop * rop; };
	static Vector operator *(Vector lop, int rop) { return (double)rop * lop; };
	static Vector operator /(Vector lop, int rop) { return lop / (double)rop; };
	
#pragma endregion

// �������� �����. ��������� � ��� ������ ��� ���������� ��������� � ������� ������ �������.
// �� ���������� ���� �������� ��������� ����� ������ �� ����� ������ � ���������� ������.
// ��� ����� ��� �� ���� ��������� �������.
	class System 
	{
	public:
		double t;//sec.

		// ������ ���������� ����� �������� ���
		Vector r;//km.
		// ������ ��������
		Vector v;//km/days.
		// ����� ���������
		double m;//tonn.
		//����� �������
		double M;//tonn.
		// ������ �������
		double R;//km.
		// ������ ���������
		double Ra;//km.
		// ���� ���������� ����� ������
		double F;//H.
		// ��������� ���������
		double a;//rm/days^2.
		// ���� ����� �������� �������� � ���� ��
		// ��������� ��� ����������� �������.
		double AlfaV;

		//������������
		System(double t, Vector r, Vector v, double M, double m, double R, double Ra, double AlfaV, double F, double a) 
		{
			this->t = t; 

			this->r.x = r.x; 
			this->r.y = r.y;

			this->v.x = v.x; 
			this->v.y = v.y;

			this->M = M; 
			this->m = m;

			this->R = R;
			this->Ra = Ra;

			this->AlfaV = AlfaV;

			this->F = F;
			this->a = a;
		};
		System();
	};
};