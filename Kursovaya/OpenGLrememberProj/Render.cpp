

#include "Render.h"
#include <vector>
#include "MyVector3d.h"
#include <Windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>
#include <math.h>
#define M_PI       3.14159265358979323846

inline double fact(int n)		//(Н) Функция факториала
{
	int r = 1;
	for (int i = 2; i <= n; ++i)
		r *= i;
	return r;
}

inline double Bern(int i, int n, double u)		//(Н) Многочлен Бернштейна, в гайде есть ссылка на вики
{
	return (fact(n) / (fact(i) * fact(n - i))) * pow(u, i) * pow(1 - u, n - i);
}

Vector3 BCurve(Vector3* points, int n, double t)		
{
	Vector3 res(0, 0, 0);
	for (int i = 0; i < n; i++)
		res = res + points[i] * Bern(i, n - 1, t);
	return res;
}

Vector3 HCurve(Vector3 p1, Vector3 r1, Vector3 p4, Vector3 r4, double t)
{
	return p1 * (2 * t * t * t - 3 * t * t + 1) + p4 * (-2 * t * t * t + 3 * t * t) + r1 * (t * t * t - 2 * t * t + t) + r4 * (t * t * t - t * t);
}

void drawCube()			{

	Vector3 a(0, 0, 0), b(1, 0, 0), c(1, 1, 0), d(0, 1, 0);

	auto drawPlane = [&]()			
		{
			glPushMatrix();
			glTranslated(-.5, -.5, 0);
			glBegin(GL_QUADS);
			glNormal3d(0, 0, 1);
			glVertex3dv(a.toArray());
			glVertex3dv(b.toArray());
			glVertex3dv(c.toArray());
			glVertex3dv(d.toArray());
			glEnd();
			glPopMatrix();
		};

	glPushMatrix();					
	glTranslated(0, 0, -0.5);
	glRotated(180, 1, 0, 0);
	drawPlane();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0, 0.5);
	drawPlane();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, -0.5, 0);
	glRotated(90, 1, 0, 0);
	drawPlane();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0.5, 0);
	glRotated(-90, 1, 0, 0);
	drawPlane();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.5, 0, 0);
	glRotated(90, 0, 1, 0);
	drawPlane();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.5, 0, 0);
	glRotated(-90, 0, 1, 0);
	drawPlane();
	glPopMatrix();


	glBegin(GL_LINES);									
	glColor3d(1, 0, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(2, 0, 0);
	glColor3d(0, 1, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 2, 0);
	glColor3d(0, 0, 1);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, 2);
	glEnd();

}

float anim_h = 0.0008;	
double anim_t = anim_h;


void Render(double delta_time)
{   	
	glClearColor(0.184, 0.309, 0.309, 1.0);

	Vector3 p[4];

	p[0] = Vector3(0, 0, 0);
	p[1] = Vector3(10, -5, 10);
	p[2] = Vector3(33, 17, 10);
	p[3] = Vector3(-1, 0, -20);
	
	glPushMatrix();
	Vector3 pos = BCurve(p, 4, anim_t);
	Vector3 pre_pos = BCurve(p, 4, anim_t - anim_h);
	Vector3 dir = (pos - pre_pos).normolize();

	Vector3 orig(1, 0, 0);							
	Vector3 rotX(dir.X(), dir.Y(), 0);
	rotX = rotX.normolize();
	double cosU = (orig.X() * rotX.X()) + (orig.Y() * rotX.Y()) + (orig.Z() * rotX.Z());			
	Vector3 vecpr = orig.vectProisvedenie(rotX);

	double sinSign = vecpr.Z() / abs(vecpr.Z());
	double U = acos(cosU) * 180 / 3.141592 * sinSign;

	double cosZU = (0 * dir.X()) + (0 * dir.Y()) + (1 * dir.Z());	
	double ZU = acos(dir.Z()) * 180.0 / M_PI - 90;

	glTranslated(pos.X(), pos.Y(), pos.Z());
	glRotated(U, 0, 0, 1);
	glRotated(ZU, 0, 1, 0);
	drawCube();									
	glPopMatrix();

	anim_t += anim_h;									
	if (anim_t > 1) anim_h = -anim_h;
	if (anim_t < 0) anim_h = -anim_h;

	Vector3 p1[4];

	p1[0] = Vector3(0, -5, 0);
	p1[1] = Vector3(23, -13, 23);
	p1[2] = Vector3(-4, -19, 1);
	p1[3] = Vector3(14, 13, -13);

	glPushMatrix();
	pos = BCurve(p1, 4, anim_t);
	pre_pos = BCurve(p1, 4, anim_t - anim_h);
	dir = (pos - pre_pos).normolize();

	orig = { 1, 0, 0 };									
	rotX = { dir.X(), dir.Y(), 0 };
	rotX = rotX.normolize();
	cosU = (orig.X() * rotX.X()) + (orig.Y() * rotX.Y()) + (orig.Z() * rotX.Z());			
	vecpr = orig.vectProisvedenie(rotX);

	sinSign = vecpr.Z() / abs(vecpr.Z());
	U = acos(cosU) * 180 / 3.141592 * sinSign;

	cosZU = (0 * dir.X()) + (0 * dir.Y()) + (1 * dir.Z());	 
	ZU = acos(dir.Z()) * 180.0 / M_PI - 90;

	glTranslated(pos.X(), pos.Y(), pos.Z());
	glRotated(U, 0, 0, 1);
	glRotated(ZU, 0, 1, 0);

	drawCube();									
	glPopMatrix();

	glColor3d(1, 0.3, 0);

	glBegin(GL_LINE);
	glVertex3dv(pos.toArray());
	glVertex3dv((pos + dir.normolize() * 3).toArray());
	glEnd();

	anim_t += anim_h;									
	if (anim_t > 1) anim_h = -anim_h;
	if (anim_t < 0) anim_h = -anim_h;

	glColor3d(1, .5, .5);
	for (double t = 0; t <= 1; t += 0.01)
	{
		glVertex3dv(BCurve(p1, 4, t).toArray());
	}
	glEnd();

	//первый модуль
	double A[] = { 3, 3, 3 + sqrt(18) };
	double B[] = { -3,3,3 + sqrt(18) };
	double C[] = { -3,-3,3 + sqrt(18) };
	double D[] = { 3,-3,3 + sqrt(18) };

	double A1[] = { 3, 3, -3 - sqrt(18) };
	double B1[] = { -3,3,-3 - sqrt(18) };
	double C1[] = { -3,-3,-3 - sqrt(18) };
	double D1[] = { 3,-3,-3 - sqrt(18) };

	double V[] = { 3+sqrt(18), 3, 3};
	double V1[] = { 3+sqrt(18), 3, -3};
	double X[] = { 3+sqrt(18),-3,3};
	double X1[] = { 3+sqrt(18),-3,-3};
	double Y[] = { 3, 3 + sqrt(18), 3 };
	double Y1[] = { 3, 3 + sqrt(18), -3 };
	double Z[] = { -3, 3 + sqrt(18), 3 };
	double Z1[] = { -3, 3 + sqrt(18), -3 };

	double V2[] = { -3 - sqrt(18), 3, 3 };
	double V3[] = { -3 - sqrt(18), 3, -3 };
	double X2[] = { -3 - sqrt(18),-3,3 };
	double X3[] = { -3 - sqrt(18),-3,-3 };
	double Y2[] = { 3, -3 - sqrt(18), 3 };
	double Y3[] = { 3, -3 - sqrt(18), -3 };
	double Z2[] = { -3, -3 - sqrt(18), 3 };
	double Z3[] = { -3, -3 - sqrt(18), -3 };


	//второй модуль
	double A2[] = { 3, 3, 24 + sqrt(18) };
	double B2[] = { -3,3,24 + sqrt(18) };
	double C2[] = { -3,-3,24 + sqrt(18) };
	double D2[] = { 3,-3,24 + sqrt(18) };

	double A21[] = { 3, 3, 18 - sqrt(18) };
	double B21[] = { -3,3,18 - sqrt(18) };
	double C21[] = { -3,-3,18 - sqrt(18) };
	double D21[] = { 3,-3,18 - sqrt(18) };

	double V20[] = { 3 + sqrt(18), 3, 24 };
	double V21[] = { 3 + sqrt(18), 3, 18 };
	double X20[] = { 3 + sqrt(18),-3,24 };
	double X21[] = { 3 + sqrt(18),-3,18 };
	double Y20[] = { 3, 3 + sqrt(18), 24 };
	double Y21[] = { 3, 3 + sqrt(18), 18 };
	double Z20[] = { -3, 3 + sqrt(18), 24 };
	double Z21[] = { -3, 3 + sqrt(18), 18 };

	double V22[] = { -3 - sqrt(18), 3, 24 };
	double V23[] = { -3 - sqrt(18), 3, 18 };
	double X22[] = { -3 - sqrt(18),-3,24 };
	double X23[] = { -3 - sqrt(18),-3,18 };
	double Y22[] = { 3, -3 - sqrt(18), 24 };
	double Y23[] = { 3, -3 - sqrt(18), 18 };
	double Z22[] = { -3, -3 - sqrt(18), 24 };
	double Z23[] = { -3, -3 - sqrt(18), 18 };






//смычка
	glBegin(GL_QUADS);
	glColor3d(0.7, 0.7, 0.6);
	glVertex3dv(A);
	glVertex3dv(A21);
	glVertex3dv(B21);
	glVertex3dv(B);
	glEnd();
	glBegin(GL_QUADS);
	glColor3d(0.7, 0.7, 0.6);
	glVertex3dv(B);
	glVertex3dv(B21);
	glVertex3dv(C21);
	glVertex3dv(C);
	glEnd();
	glBegin(GL_QUADS);
	glColor3d(0.7, 0.7, 0.6);
	glVertex3dv(C);
	glVertex3dv(C21);
	glVertex3dv(D21);
	glVertex3dv(D);
	glEnd();
	glBegin(GL_QUADS);
	glColor3d(0.7, 0.7, 0.6);
	glVertex3dv(D);
	glVertex3dv(D21);
	glVertex3dv(A21);
	glVertex3dv(A);
	glEnd();
//Модуль1 Основание низ
	glBegin(GL_QUADS);
	glColor3d(0.274, 0.219, 0.231);
	glVertex3dv(A1);
	glVertex3dv(B1);
	glVertex3dv(C1);
	glVertex3dv(D1);
	glEnd();

//Модуль1 кольцо 
	glBegin(GL_QUADS);
	glColor3d(0.274, 0.219, 0.231);
	glVertex3dv(V);
	glVertex3dv(V1);
	glVertex3dv(X1);
	glVertex3dv(X);
	glEnd();
	glBegin(GL_QUADS);
	glColor3d(0.274, 0.219, 0.231);
	glVertex3dv(Y);
	glVertex3dv(Y1);
	glVertex3dv(Z1);
	glVertex3dv(Z);
	glEnd();
	glBegin(GL_QUADS);
	glColor3d(0.5, 0.5, 0.5);
	glVertex3dv(V);
	glVertex3dv(V1);
	glVertex3dv(Y1);
	glVertex3dv(Y);
	glEnd();
	glBegin(GL_QUADS);
	glColor3d(0.274, 0.219, 0.231);
	glVertex3dv(V2);
	glVertex3dv(V3);
	glVertex3dv(X3);
	glVertex3dv(X2);
	glEnd();
	glBegin(GL_QUADS);
	glColor3d(0.274, 0.219, 0.231);
	glVertex3dv(Y2);
	glVertex3dv(Y3);
	glVertex3dv(Z3);
	glVertex3dv(Z2);
	glEnd();
	glBegin(GL_QUADS);
	glColor3d(0.5, 0.5, 0.5);
	glVertex3dv(X2);
	glVertex3dv(X3);
	glVertex3dv(Z3);
	glVertex3dv(Z2);
	glEnd();
	glBegin(GL_QUADS);
	glColor3d(0.5, 0.5, 0.5);
	glVertex3dv(X);
	glVertex3dv(X1);
	glVertex3dv(Y3);
	glVertex3dv(Y2);
	glEnd();
	glBegin(GL_QUADS);
	glColor3d(0.5, 0.5, 0.5);
	glVertex3dv(V2);
	glVertex3dv(V3);
	glVertex3dv(Z1);
	glVertex3dv(Z);
	glEnd();
	//Модуль1 верхняя часть
	glBegin(GL_QUADS);
	glColor3d(0.5, 0.5, 0.5);
	glVertex3dv(A);
	glVertex3dv(B);
	glVertex3dv(Z);
	glVertex3dv(Y);
	glEnd();
	glBegin(GL_QUADS);
	glColor3d(0.2196, 0.729, 0.549);
	glVertex3dv(A);
	glVertex3dv(V);
	glVertex3dv(X);
	glVertex3dv(D);
	glEnd();
	glBegin(GL_QUADS);
	glColor3d(0.5, 0.5, 0.5);
	glVertex3dv(D);
	glVertex3dv(Y2);
	glVertex3dv(Z2);
	glVertex3dv(C);
	glEnd();
	glBegin(GL_QUADS);
	glColor3d(0.5, 0.5, 0.5);
	glVertex3dv(C);
	glVertex3dv(X2);
	glVertex3dv(V2);
	glVertex3dv(B);
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3d(0.2196, 0.729, 0.549);
	glVertex3dv(A);
	glVertex3dv(Y);
	glVertex3dv(V);
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3d(0.745, 0.814, 0.921);
	glVertex3dv(B);
	glVertex3dv(Z);
	glVertex3dv(V2);
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3d(0.745, 0.814, 0.921);
	glVertex3dv(C);
	glVertex3dv(X2);
	glVertex3dv(Z2);
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3d(0.2196, 0.729, 0.549);
	glVertex3dv(D);
	glVertex3dv(Y2);
	glVertex3dv(X);
	glEnd();
	//Модуль 1 нижняя часть
	glBegin(GL_QUADS);
	glColor3d(0.5, 0.5, 0.5);
	glVertex3dv(A1);
	glVertex3dv(B1);
	glVertex3dv(Z1);
	glVertex3dv(Y1);
	glEnd();
	glBegin(GL_QUADS);
	glColor3d(0.5, 0.5, 0.5);
	glVertex3dv(A1);
	glVertex3dv(V1);
	glVertex3dv(X1);
	glVertex3dv(D1);
	glEnd();
	glBegin(GL_QUADS);
	glColor3d(0.5, 0.5, 0.5);
	glVertex3dv(D1);
	glVertex3dv(Y3);
	glVertex3dv(Z3);
	glVertex3dv(C1);
	glEnd();
	glBegin(GL_QUADS);
	glColor3d(0.5, 0.5, 0.5);
	glVertex3dv(C1);
	glVertex3dv(X3);
	glVertex3dv(V3);
	glVertex3dv(B1);
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3d(0.745, 0.814, 0.921);
	glVertex3dv(A1);
	glVertex3dv(Y1);
	glVertex3dv(V1);
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3d(0.745, 0.814, 0.921);
	glVertex3dv(B1);
	glVertex3dv(Z1);
	glVertex3dv(V3);
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3d(0.745, 0.814, 0.921);
	glVertex3dv(C1);
	glVertex3dv(X3);
	glVertex3dv(Z3);
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3d(0.745, 0.814, 0.921);
	glVertex3dv(D1);
	glVertex3dv(Y3);
	glVertex3dv(X1);
	glEnd();

	// Модуль 2основание верх
	glBegin(GL_QUADS);
	glColor3d(0.2, 0.2, 0.2);
	glVertex3dv(A2);
	glVertex3dv(B2);
	glVertex3dv(C2);
	glVertex3dv(D2);
	glEnd();
	

	//Модуль2 кольцо 
	glBegin(GL_QUADS);
	glColor3d(0.274, 0.219, 0.231);
	glVertex3dv(V20);
	glVertex3dv(V21);
	glVertex3dv(X21);
	glVertex3dv(X20);
	glEnd();
	glBegin(GL_QUADS);
	glColor3d(0.274, 0.219, 0.231);
	glVertex3dv(Y20);
	glVertex3dv(Y21);
	glVertex3dv(Z21);
	glVertex3dv(Z20);
	glEnd();
	glBegin(GL_QUADS);
	glColor3d(0.5, 0.5, 0.5);
	glVertex3dv(V20);
	glVertex3dv(V21);
	glVertex3dv(Y21);
	glVertex3dv(Y20);
	glEnd();
	glBegin(GL_QUADS);
	glColor3d(0.274, 0.219, 0.231);
	glVertex3dv(V22);
	glVertex3dv(V23);
	glVertex3dv(X23);
	glVertex3dv(X22);
	glEnd();
	glBegin(GL_QUADS);
	glColor3d(0.274, 0.219, 0.231);
	glVertex3dv(Y22);
	glVertex3dv(Y23);
	glVertex3dv(Z23);
	glVertex3dv(Z22);
	glEnd();
	glBegin(GL_QUADS);
	glColor3d(0.5, 0.5, 0.5);
	glVertex3dv(X22);
	glVertex3dv(X23);
	glVertex3dv(Z23);
	glVertex3dv(Z22);
	glEnd();
	glBegin(GL_QUADS);
	glColor3d(0.5, 0.5, 0.5);
	glVertex3dv(X20);
	glVertex3dv(X21);
	glVertex3dv(Y23);
	glVertex3dv(Y22);
	glEnd();
	glBegin(GL_QUADS);
	glColor3d(0.5, 0.5, 0.5);
	glVertex3dv(V22);
	glVertex3dv(V23);
	glVertex3dv(Z21);
	glVertex3dv(Z20);
	glEnd();
	//Модуль2 верхняя часть
	glBegin(GL_QUADS);
	glColor3d(0.5, 0.5, 0.5);
	glVertex3dv(A2);
	glVertex3dv(B2);
	glVertex3dv(Z20);
	glVertex3dv(Y20);
	glEnd();
	glBegin(GL_QUADS);
	glColor3d(0.5, 0.5, 0.5);
	glVertex3dv(A2);
	glVertex3dv(V20);
	glVertex3dv(X20);
	glVertex3dv(D2);
	glEnd();
	glBegin(GL_QUADS);
	glColor3d(0.5, 0.5, 0.5);
	glVertex3dv(D2);
	glVertex3dv(Y22);
	glVertex3dv(Z22);
	glVertex3dv(C2);
	glEnd();
	glBegin(GL_QUADS);
	glColor3d(0.5, 0.5, 0.5);
	glVertex3dv(C2);
	glVertex3dv(X22);
	glVertex3dv(V22);
	glVertex3dv(B2);
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3d(0.745, 0.814, 0.921);
	glVertex3dv(A2);
	glVertex3dv(Y20);
	glVertex3dv(V20);
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3d(0.745, 0.814, 0.921);
	glVertex3dv(B2);
	glVertex3dv(Z20);
	glVertex3dv(V22);
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3d(0.745, 0.814, 0.921);
	glVertex3dv(C2);
	glVertex3dv(X22);
	glVertex3dv(Z22);
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3d(0.745, 0.814, 0.921);
	glVertex3dv(D2);
	glVertex3dv(Y22);
	glVertex3dv(X20);
	glEnd();
	//Модуль 2 нижняя часть
	glBegin(GL_QUADS);
	glColor3d(0.5, 0.5, 0.5);
	glVertex3dv(A21);
	glVertex3dv(B21);
	glVertex3dv(Z21);
	glVertex3dv(Y21);
	glEnd();
	glBegin(GL_QUADS);
	glColor3d(0.5, 0.5, 0.5);
	glVertex3dv(A21);
	glVertex3dv(V21);
	glVertex3dv(X21);
	glVertex3dv(D21);
	glEnd();
	glBegin(GL_QUADS);
	glColor3d(0.5, 0.5, 0.5);
	glVertex3dv(D21);
	glVertex3dv(Y23);
	glVertex3dv(Z23);
	glVertex3dv(C21);
	glEnd();
	glBegin(GL_QUADS);
	glColor3d(0.5, 0.5, 0.5);
	glVertex3dv(C21);
	glVertex3dv(X23);
	glVertex3dv(V23);
	glVertex3dv(B21);
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3d(0.745, 0.814, 0.921);
	glVertex3dv(A21);
	glVertex3dv(Y21);
	glVertex3dv(V21);
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3d(0.745, 0.814, 0.921);
	glVertex3dv(B21);
	glVertex3dv(Z21);
	glVertex3dv(V23);
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3d(0.745, 0.814, 0.921);
	glVertex3dv(C21);
	glVertex3dv(X23);
	glVertex3dv(Z23);
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3d(0.745, 0.814, 0.921);
	glVertex3dv(D21);
	glVertex3dv(Y23);
	glVertex3dv(X21);
	glEnd();

}   

