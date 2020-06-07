#include <iostream>
#include <cstdlib>
#include <math.h>
#include "rk4.h"

double t_0 = 0, t_k = 2, x_0 = 0, y_0 = 0; double pi = 4. * atan(1.);//warunki poczatkowe i czas lotu pocisku
double g = 9.80;//przyspieszenie ziemskie normalne

void rhs_fun(double t, double* X, double* F, double k, double a, double b)//stala laczaca inne stale i zmienne dotyczace rozkladu predkosci wiatru
{
	F[0] = X[1];//polozenie w osi x to predkosc * czas
	F[1] = -pow(b, 2) * k - 0.5 * k * pow(X[1], 2) + b * k * X[1] - 0.5 * pow(a, 2) * k * pow(X[2], 2) - a * b * k * X[2] + a * k * X[1] * X[2];//predkosc w osi x
	F[2] = X[3];//polozenie w osi y to predkosc * czas
	F[3] = -X[3] * X[3] * k / 2 - g;//predkosc w osi y
}

int main()
{
	double C, r, a_s, m, v0, q, a, b;
	double x = x_0, y = y_0, t = t_0;//przypisanie do wspolrzednych warunkow poczatkowych i czas zero
	double X[4], X1[4];//X - wartosci w chwili t, X1 - wartosci w chwili t+1

	printf("C="); scanf_s("%lf", &C);//wsp oporu aero, dla kuli - 0,47
	printf("r="); scanf_s("%lf", &r);//promien kuli [m]
	printf("a_s="); scanf_s("%lf", &a_s);//kat poczatkowy <0, 90>
	printf("m="); scanf_s("%lf", &m);//masa kuli [kg]
	printf("v0="); scanf_s("%lf", &v0);//predkosc poczatkowa kuli [m/s]
	printf("q="); scanf_s("%lf", &q);//gestosc powietrza normalna - 1.2 [kg/m^3]
	printf("a="); scanf_s("%lf", &a);//predkosc przyrostu predkosci wiatru
	printf("b="); scanf_s("%lf", &b);//predkosc wiatru przy powierzchni

	double a_r = a_s * pi / 180;//zamiana wartosci kata ze stopni na radiany
	
	double H = 0.01;//dlugosc kroku calkowania
	double S = pi * pow(r, 2); double k = q * S * C / m;//powierzchnia rzutu aero i stala laczaca stale

	X[0] = 0;//x0
	X[1] = v0 * cos(a_r);//vx0
	X[2] = 0;//y0
	X[3] = v0 * sin(a_r);//vy0

	FILE* x_t;
	errno_t err = fopen_s(&x_t, "x_t.txt", "a+");

	FILE* vx_t;
	errno_t err2 = fopen_s(&vx_t, "vx_t.txt", "a+");

	FILE* y_t;
	errno_t err3 = fopen_s(&y_t, "y_t.txt", "a+");

	FILE* vy_t;
	errno_t err4 = fopen_s(&vy_t, "vy_t.txt", "a+");

	for (double i = 0; i < t_k; i+=H)//C=0.47/r=0.01/a_s=30/m=0.05/v0=30/q=1.2/a=1/b=2
	{
		vrk4(0, X, H, 4, rhs_fun, X1, k, a, b);

		X[0] = X1[0];
		X[1] = X1[1];
		X[2] = X1[2];
		X[3] = X1[3];

		//point(X1[0], X1[2]); - wykres trajektorii y(x)

		printf("%.2lf\t", X1[0]); fprintf(x_t, "%.2lf\n", X1[0]);
		printf("%.2lf\t", X1[1]); fprintf(vx_t, "%.2lf\n", X1[1]);
		printf("%.2lf\t", X1[2]); fprintf(y_t, "%.2lf\n", X1[2]);
		printf("%.2lf\n", X1[3]); fprintf(vy_t, "%.2lf\n", X1[3]);
	}

	fclose(x_t); fclose(vx_t); fclose(y_t); fclose(vy_t);
}