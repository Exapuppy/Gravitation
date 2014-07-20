#include "stdafx.h"
#include "PhysicCtrl.h"

// функции, внос€щие значени€ массы, скорости и рассто€ни€ в класс sys
void toSysPlmass ( char lpch_0, char lpch_1, char lpch_2, Physics::System * sys)
{
	switch(lpch_0)
	{
	case 'M':
		switch(lpch_1)
		{
		case 'e':
			sys->M = M_Mercury;
			sys->R = R_Mercury;
			break;
		case 'a':
			if(lpch_2 == 'r')
			{
				sys->M = M_Mars;
				sys->R = R_Mars;
			}
			else
			{
				sys->M = M_Makemake;
				sys->R = R_Makemake;
			}
			break;
		default: break;
		}
		break;
	case 'V':
		sys->M = M_Venus;
		sys->R = R_Venus;
		break;
	case 'E':
		if(lpch_1 == 'r')
		{
			sys->M = M_Eris;
			sys->R = R_Eris;
		}
		else
		{
			sys->M = M_Earth;
			sys->R = R_Earth;
		}
		break;
	case 'U':
		sys->M = M_Uranus;
		sys->R = R_Uranus;
		break;
	case 'N':
		sys->M = M_Neptune;
		sys->R = R_Neptune;
		break;
	case 'P':
		sys->M = M_Pluto;
		sys->R = R_Pluto;
		break;
	case 'H':
		sys->M = M_Haumea;
		sys->R = R_Haumea;
		break;
	default: break;
	}
}
void toSysAsmass ( char lpch, Physics::System * sys)
{
	switch(lpch)
	{
	case 'C':
		sys->m = M_Ceres;
		sys->Ra = R_Ceres;
		break;
	case 'P':
		sys->m = M_Pallas;
		sys->Ra = R_Pallas;
		break;
	case 'V':
		sys->m = M_Vesta;
		sys->Ra = R_Vesta;
		break;
	case 'G':
		sys->m = M_Gigea;
		sys->Ra = R_Gigea;
		break;
	case 'E':
		sys->m = M_Euphrosyne;
		sys->Ra = R_Euphrosyne;
		break; 
	default: break;
	}
}
void toSysVA ( int V, int AlfaV, Physics::System * sys)
{
	sys->AlfaV = AlfaV;
	sys->v.x = - cos((double)AlfaV * 3.14159265358979323846 / 180.0) * (double)(V*60);
	sys->v.y = sin((double)AlfaV * 3.14159265358979323846 / 180.0) * (double)(V*60);
}
void toSysR ( int radius, Physics::System * sys)
{
	sys->r.x = (double) radius * 1e3;
	sys->r.y = 0;
}
void toSysPlmassTB ( DWORD p, Physics::System * sys)
{
	sys->M = (double) p * 1e21;
	sys->R = sys->M / 1e21;
}
void toSysAsmassTB ( DWORD p, Physics::System * sys)
{
	sys->m = (double) p * 1e18;
	sys->Ra = p >> 1;
}

// функции, реализующие метод Ёйлера
inline Physics::Vector F1( Physics::Vector r, double M1, double M2)
{
	return - (  ( (M1 + M2)/ r.norm() ) * ( G / r.norm() ) *  ( r  / r.norm() ) );
}
void F_Ailer ( double h, Physics::System &pt)
{
	register Physics::Vector dr;
	pt.v = pt.v + F1( pt.r, pt.M, pt.m) * h;
	pt.t += h;
	dr = h * pt.v;
	pt.r = pt.r + dr;
}

// функци€ расчета траектории полета астероида
int Traectory(
	Physics::Vector * traectory,	// out
	Physics::System sys,			// in
	double h) // in
{
	int mass_volume = 0;
	Physics::System *sys2 = new Physics::System(sys);
	Physics::Vector * p = traectory;
	double min_r = sys2->R + sys2->Ra;
	for(int i = 6000000; i >= 0; i--)
	{
		if(i % 200 == 0)
		{
			mass_volume ++;
			*p = sys2->r;
			p++;
		}
			
		if( sys2->r.norm() < min_r || sys2->r.norm() > 400e3)
		{
			mass_volume ++;
			*p = sys2->r;
			p++;
			break;
		}
		else
			F_Ailer(h, *sys2);
	}
	delete sys2;
	return mass_volume;
}

// функци€, возвращающа€ максимальное значение
double GetMaxA(int mass_volume, Physics::Vector * traectory, double SummaryMass, double SummaryRad)
{
	// находитс€ минимальное рассто€ние, которое возможно на данной траектории
	double min_r = traectory[mass_volume - 1].norm(); 
	if(min_r > SummaryRad)
		for (int i = mass_volume - 2; i != -1; i--)
		{
			if(traectory[i].norm() < min_r)
				min_r = traectory[i].norm();
		}
	// и высчитываетс€ ускорение в этой точке.
	return (SummaryMass * G) / ( min_r * min_r );
}