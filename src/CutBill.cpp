// Cut-Billiard simulator.
// Author: José Ramón Palacios Barreda.
// Email: palacios_barreda@hotmail.com

#include <fstream>
#include <iostream>
#include <limits>
#include <cmath>
#include "CutBill.h"
using namespace std;
typedef std::numeric_limits< double > dbl;

// Simres
Simres::Simres()
{
	colisiones = 0;
	salida = 0;
	total_distance = 0.0;
}

// Otros
double radians(double x){return x*pi/180.0;}
double degrees(double x){return x*180.0/pi;}


// Parámetros
int max_iter = 50000;// Máximo número de iteraciones
int prec = dbl::digits10-2; // Precisión decimal
double R = 1.0;      // Radio de la frontera
double W,w;          // Corte de la frontera
double v0 = R;       // Módulo de la velocidad
double phi = 0.0;      // Ángulo de inyección
double vx, vy, vxp, vyp;// Dirección de la velocidad
double omega = 3.0*pi/4.0;  // Posición de la entrada respecto al corte
double gammax = 3.0*pi/2.0; // Posición de la salida
double delta = radians(10);// Tamaño angular de las aperturas
double x,y;   // Coordenadas de la partícula
double b;     // Ordenada al origen del corte
double alpha; // Ángulo entre la normal de la frontera y el eje x
int a;        // Apertura de salida


Simres Simulate(bool log=false,bool cerrado=false, bool silent=true)
{
	ofstream outf, outfs;
	outf.precision(dbl::digits10-2);
	outfs.precision(dbl::digits10-2);
	Simres res;
	if(log)
	{
		outf.open ("data/bcc_tray.dat");
		outfs.open ("data/serie_s.dat");
		outf <<"# phi,gamma,delta,R,b,W"<<endl;
		outf <<"# "<<fixed<<phi<<","<<gammax<<","<<delta<<","<<R<<","<<b<<","<<W<<endl;
		outf<<fixed<<x<<","<<y<<endl;
	}
	res.intersections.push_back(Coord(x,y));

	res.colisiones = 0;
	double const _large = 1e99;
	double clx = -b/2 - sqrt(2*R*R - b*b)/2; // Limites del corte recto
	double cly = b/2 - sqrt(2*R*R - b*b)/2;
	double cux = -b/2 + sqrt(2*R*R - b*b)/2;
	double cuy = b/2 + sqrt(2*R*R - b*b)/2;
	double eps = 1/pow(10.0,prec-2); // Precisión

	// Coordenadas de Birkhoff
	double psi1,psi2;
	double s = 0, p, theta = 0;
	if(cux > 0)
	{
		psi1 = acos(cux/R);
		psi2 = acos(abs(clx)/R) + pi;
	}
	else
	{
		psi1 = pi/2 + acos(cuy/R);
		psi2 = pi - acos(clx/R);
	}

	double l1 = R*psi1;
	double l2 = sqrt((clx-cux)*(clx-cux)+(cly-cuy)*(cly-cuy));
	double l = l1+l2+R*(2*pi-psi2);
	a = 0;
	while(1)
	{
		if(abs(vx) < eps) vx = 0;
		if(abs(vy) < eps) vy = 0;
		// Adelantar hasta la colisión más próxima
		double m;
		if(vx == 0) m = _large;
		else m = vy/vx;
		double prx = 0, pry = 0, dr = 0;
		double pc1x, pc1y,pc2x,pc2y,dc1,dc2;
		bool col_corte = false;
		// Colisión con corte recto
		if (m != _large)
		{
			prx = (y - m*x - b)/(1-m);
			pry = prx + b;
			dr = sqrt((prx-x)*(prx-x)+(pry-y)*(pry-y));
			pc1x = (1/(2*(1+m*m)))*(-2*m*(y-m*x) + sqrt(4*m*m*(y - m*x)*(y - m*x) - 4*(1+m*m)*(m*m*x*x + y*y - 2*m*x*y - R*R)));
			pc1y = m*(pc1x-x) + y;
			dc1 = sqrt((pc1x-x)*(pc1x-x)+(pc1y-y)*(pc1y-y));
			pc2x = (1/(2*(1+m*m)))*(-2*m*(y-m*x) - sqrt(4*m*m*(y - m*x)*(y - m*x) - 4*(1+m*m)*(m*m*x*x + y*y - 2*m*x*y - R*R)));
			pc2y = m*(pc2x-x) + y;
			dc2 = sqrt((pc2x-x)*(pc2x-x)+(pc2y-y)*(pc2y-y));
		}
		else
		{
			// Trayectoria con pendiente infinita
			int z = 0;
			if(vy < 0) z++;
			prx = x;
			pry = x + b;
			dr = sqrt((prx-x)*(prx-x)+(pry-y)*(pry-y));
			pc1x = x;
			pc1y = pow(-1.0,z+1)*sqrt(R*R - x*x);
			dc1 = sqrt((pc1x-x)*(pc1x-x)+(pc1y-y)*(pc1y-y));
			pc2x = x;
			pc2y = pow(-1.0,z)*sqrt(R*R - x*x);
			dc2 = sqrt((pc2x-x)*(pc2x-x)+(pc2y-y)*(pc2y-y));
		}

		// Casos de colisión
		// Puerta de salida
		//if(res.colisiones==0) dc2 = _large;
		// Puntos de corte equidistantes.
		if(dc1 == dc2)
		{
			if(vx > 0) { dc2 = _large; }
			else { dc1 = _large; }
		}
		// Puntos detrás del corte (en la circ)
		if((pc1x <= (pc1y - b))&&(pc1y >= (pc1x + b))){ dc1 = _large; }
		if((pc2x <= (pc2y - b))&&(pc2y >= (pc2x + b))){ dc2 = _large; }
		// Paralelo a corte
		if(m==1) dr = _large;
		// Intersección entre circunferencia y corte recto
		if(dr==dc1) dr = _large;
		if(dr==dc2) dr = _large;
		// Punto de colisión
		if(dr < eps) dr = _large;
		if(dc1 < eps) dc1 = _large;
		if(dc2 < eps) dc2 = _large;
		// Puntos en el corte, fuera de la circ.
		if((prx > cux)||(prx < clx)) { dr = _large; }
		// Escoger colisión "ganadora"
		if((dr < dc1)&&(dr < dc2)){x = prx; y = pry; col_corte=true;}
		else if((dc1 < dc2)&&(dc1 < dr)){x = pc1x; y = pc1y;}
		else if((dc2 < dc1)&&(dc2 < dr)){x = pc2x; y = pc2y;}
		else
		{
			if(!silent)
			{
				cout << "Error en detección de colisión. simular(): "<<endl
						<<"phi="<<degrees(phi)<<",W="<<W<<",vx="<<vx<<",vy="<<vy<<endl;
			}
		}

		// Actualizar velocidades
		if(col_corte)
		{ // Colisión con corte recto
			// Coordenadas de Birkhoff
			s = l1 + sqrt((x-cux)*(x-cux)+(y-cuy)*(y-cuy));
			s /= l; // Normalizar
			// Pasar al marco de referencia rotado
			vxp = vx*cos(omega) + vy*sin(omega);
			vyp = -vx*sin(omega) + vy*cos(omega);
			// Ángulo de colisión
			if(vyp > 0) theta = acos(vyp/sqrt(vxp*vxp+vyp*vyp));
			else theta = acos(-vyp/sqrt(vxp*vxp+vyp*vyp));
			p = cos(theta);
			// Invertir velocidad normal a la frontera
			vxp *= -1;
			// Regresar al marco original
			vx = vxp*cos(omega) - vyp*sin(omega);
			vy = vxp*sin(omega) + vyp*cos(omega);

			// Guardar punto de colisión
			if(log)
			{
				outf<<fixed<<x<<","<<y<<","<<s<<","<<p<<endl;
				outfs<<s<<endl;
			}
			res.intersections.push_back(Coord(x,y));
			res.colisiones += 1;
		}
		else
		{
            // Colisión con arco circular
			if ((x >= 0)&&(y >= 0)){// Cuad I
				alpha = acos(x/R);
			}
			else if ((x <= 0)&&(y >= 0)){// Cuad II
				alpha = pi/2 + acos(y/R);
			}
			else if ((x <= 0)&&(y <= 0)){// Cuad III
				alpha = pi + acos(abs(x)/R);
			}
			else if ((x >= 0)&&(y <= 0)){// Cuad IV
				alpha = 2*pi - acos(x/R);
			}

			//Coordenadas de Birkhoff
			if((0 <= alpha )&&(alpha <= psi1)) s = R*alpha;
			else if ((psi2 <= alpha)&&(alpha <= 2*pi)) s=l1+l2+R*(alpha-psi2);
			else
			{
				if(!silent)
				{
					cout <<"Error en cálculo de coordenadas de Birkhoff."<<fixed<<endl
							<<"phi="<<degrees(phi)<<",W="<<W<<",vx="<<vx<<",vy="<<vy<<endl
							<<"psi1="<<degrees(psi1)<<",psi2="<<degrees(psi2)<<",alpha="<<degrees(alpha)<<endl;
				}
			}
			s /= l; // Normalizar

			// Pasar al marco de referencia rotado
			vxp = vx*cos(alpha) + vy*sin(alpha);
			vyp = -vx*sin(alpha) + vy*cos(alpha);
			// Invertir velocidad normal a la frontera
			vxp *= -1;
			// Ángulo de colisión
			if(vyp > 0) theta = acos(vyp/sqrt(vxp*vxp+vyp*vyp));
			else theta = acos(-vyp/sqrt(vxp*vxp+vyp*vyp));
			p = cos(theta);
			// Regresar al marco original
			vx = vxp*cos(alpha) - vyp*sin(alpha);
			vy = vxp*sin(alpha) + vyp*cos(alpha);

			// Colisión con aperturas
			if(cerrado==false)
			{
				if ((alpha < (gammax + delta/2))&&(alpha > (gammax - delta/2))) a=2;
				else if ((alpha <= delta/2)||((2*pi - delta/2) <= alpha)) a=1;
			}

			// Guardar punto de colisión
			if(log)
			{
				outf<<fixed<<x<<","<<y<<","<<s<<","<<p<<endl;
				outfs<<s<<endl;
			}
			res.intersections.push_back(Coord(x,y));
			res.colisiones += 1;
		}

		if (a != 0) break;
		if(res.colisiones >= max_iter) break;
	}

	if(log) outf.close();
	res.salida = a;
	return res;
}

Simres Sim_Billiard(BillParams param)
{
	Simres out;
    W = param.W;
    phi = param.phi;
    //x = R*cos(radians(param.delta));
    //y = 0;/
    x = param.injection_x;
    y = param.injection_y;
    w = W/R;
    vx = v0*cos(param.phi);
    vy = v0*sin(param.phi);
    b = (W-R)/cos(omega - pi/2);
    max_iter = param.iter;
    out = Simulate(false, param.closed, false);

    // Calculate total distance.
    if(out.intersections.size() > 0)
    {
		for(unsigned int i=1; i<out.intersections.size(); i++)
		{
			out.total_distance += sqrt(pow(out.intersections[i].m_x-out.intersections[i-1].m_x, 2.0) +
					                   pow(out.intersections[i].m_y-out.intersections[i-1].m_y, 2.0));
		}
    }
    return out;
}
