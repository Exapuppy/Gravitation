#include "Physics.h"

//Physic parameter
void toSysPlmass ( char lpch_0, char lpch_1, char lpch_2, Physics::System * sys);
void toSysAsmass ( char lpch, Physics::System * sys);
void toSysPlmassTB ( DWORD p, Physics::System * sys);
void toSysAsmassTB ( DWORD p, Physics::System * sys);
void toSysVA ( int V, int Alfa, Physics::System * sys);
void toSysR ( int radius, Physics::System * sys);

//Physik realization
Physics::Vector F1( Physics::Vector r, double M1, double M2);
void F_Ailer ( double h, Physics::System &pt);

//Traectory
int Traectory(
	Physics::Vector * traectory, // out
	Physics::System sys, // in
	double h); // in

//возвращает максимальное ускорение, возможное при полете по данной траектории.
double GetMaxA(int mass_volume, Physics::Vector * traectory, double SummaryMass, double SummaryRad);