
#include<iostream>
#include<iomanip>
#include<vector>
#include<fstream>
#include<cstdio>
#include<cstdlib>
#include<algorithm>
#include<string>
#include<cmath>
using namespace std;

int ni,nj,ni1,ni2,ni3;
double L,H,L1,L2,L3;
double eta, eta0, d_eta;
double eta22;
double xi, xi0, d_xi;
double X[500][500], Y[500][500];
double X1[500][500], Y1[500][500];
double X2[500][500], Y2[500][500];
double X3[500][500], Y3[500][500];
double X_prev[500][500], Y_prev[500][500];
double alpha, beta, p ,q;
double d_xi1,d_xi2,d_xi3;
void xuatData();
void exportDAT();
void chiadeu();
void taoblock1();
void taoblock2();
void taoblock2new();
void taoblock3();
void gop();
void equation4(); // nén biên 
void equation41();
void  equation5();
void equation6();
void Elliptic_Transformation();
void boundary_layer();



int main()
{
    ni = 100;
    ni1=50;
    ni2=51;
    ni3=50;
    nj = 30;
    L = 2;
    L1=1.5;
    L2=0.5;
    L3=2;
    H = 0.6;

  taoblock1();
  taoblock2new();
  //taoblock3();
  gop();
 // equation5();
  chiadeu();
 // equation6();
  // equation4();
 Elliptic_Transformation();
  xuatData();
   exportDAT();
    return 0;
}

void equation5() // nén 2 biên Đ
{
    eta0 = 0.5;
    alpha = 4; // tăng alpha để nén càng nhiều tại biên
    d_eta = (double)1/(nj - 1);
    for(int i=1;i<=ni;i++)
    {
        for(int j=2;j<=nj-1;j++)
        {
            eta = d_eta*(j-1);
            double d_y;
            if( eta > 0 && eta <= eta0 )
            {
			    double temp1 = exp(alpha * eta / eta0) - 1;
			    double temp2 = exp(alpha) - 1;
			    d_y = 1 * eta0 * temp1 / temp2;
            }
            else
            {
				double temp1 = exp(alpha * (1 - eta) / eta0) - 1;
				double temp2 = exp(alpha) - 1;
				d_y = 1 - 1*(1-eta0) * temp1 / temp2;
            }
            X[i][j] = (X[i][nj] - X[i][1])*d_y + X[i][1];
            Y[i][j] = (Y[i][nj] - Y[i][1])*d_y + Y[i][1];
        }
    }
}
void taoblock1()
{
    d_eta = (double)1/(nj - 1);
	d_xi=(double)1/(ni1 - 1);
	for (int i = 1; i <= ni1; i++)
     {         xi=d_xi*(i-1);
                Y1[i][1] = 0;
                Y1[i][nj] = H;
                X1[i][1] = L1*xi;
                X1[i][nj] = X1[i][1];
     }
}
void taoblock2new()
{
    d_eta = (double)1/(nj - 1);
	double d_xi2=(double)1/(ni2 - 1);
    for (int i = 1; i <= ni2; i++) {
		double xi2 = d_xi2*(i - 1);
		X2[i][1] = L1 + L2*xi2;
		Y2[i][nj] = H;
		X2[i][nj] = X2[i][1];
        Y2[i][1]= L2*xi2*tan(30*3.141592654/180);
 }
}
void taoblock3()
{
    double d_xi3=(double)1/(ni3 - 1);
    for (int i = 1; i <= (ni3); i++) {
		double xi3 = d_xi3*(i - 1);

		Y3[i][1] =L2*tan(30*3.141592654/180);
		X3[i][1] = 2+L3*xi3;
		Y3[i][nj] = H;
		X3[i][nj] = X3[i][1];
}
}

void gop()
{
    for (int i = 1; i <=ni1; i++) {
		for (int j = 1; j <= nj; j++) {
			X[i][j] = X1[i][j];
			Y[i][j] = Y1[i][j];
		}
	}
	for (int i = ni1; i <=ni; i++) {
		for (int j = 1; j <= nj; j++) {
			X[i][j] = X2[i-ni1+1][j];
			Y[i][j] = Y2[i-ni1+1][j];
		}
	}

}
void Elliptic_Transformation()
{
   for(int i=1;i<=ni;i++)
    {
        for(int j=1;j<=nj;j++)
        {
            X_prev[i][j] = X[i][j];
            Y_prev[i][j] = Y[i][j];
        }
    }
    int count = 0;
    while(count <= 100)
    {
        for(int j=2;j<nj;j++)
        {
            for(int i=2;i<ni;i++)
            {
                double a1 = (X_prev[i][j+1] - X[i][j-1])/2;
                double a2 = (Y_prev[i][j+1] - Y[i][j-1])/2;
                double c1 = (X_prev[i+1][j] - X[i-1][j])/2;
                double c2 = (Y_prev[i+1][j] - Y[i-1][j])/2;

                double alpha_e = a1*a1 + a2*a2;
                double gamma_e = c1*c1 + c2*c2;
                double beta_e = a1*c1+ a2*c2;

                X[i][j] = (alpha_e*(X_prev[i + 1][j] + X[i - 1][j]) + gamma_e*(X_prev[i][j + 1] + X[i][j - 1]) - (beta_e/2)*(X_prev[i + 1][j + 1] + X[i - 1][j - 1] - X_prev[i - 1][j + 1] - X_prev[i + 1][j - 1]))/(2*(alpha_e + gamma_e));
				Y[i][j] = (alpha_e*(Y_prev[i + 1][j] + Y[i - 1][j]) + gamma_e*(Y_prev[i][j + 1] + Y[i][j - 1]) - (beta_e/2)*(Y_prev[i + 1][j + 1] + Y[i - 1][j - 1] - Y_prev[i - 1][j + 1] - Y_prev[i + 1][j - 1]))/(2*(alpha_e + gamma_e));

                X_prev[i][j] = X[i][j];
                Y_prev[i][j] = Y[i][j];
            }
        }
count++;
    }

}
void chiadeu()
{
    d_eta = (double)1/(nj - 1);
	for (int i = 1; i <= ni; i++)
	{
        for (int j = 2; j <= nj-1; j++)
        {
			eta = (double)d_eta*(j - 1);
			double d_y = eta;
            X[i][j] = (X[i][nj] - X[i][1])*d_y + X[i][1];
			Y[i][j] = (Y[i][nj] - Y[i][1])*d_y + Y[i][1];
		}
    }
}
void equation4() // nén giữa Đ
{
    eta0 = 0.5;
    alpha = 4;
    d_eta = (double)1/(nj - 1);
    for(int i=1;i<=ni;i++)
    {
        for(int j=2;j<=nj-1;j++)
        {
            eta = (double)d_eta*(j-1);
            double d_y;
            if( eta >= 0 && eta <= eta0 )
            {
                d_y = 1*eta0*( exp(alpha) - exp( alpha*(1-eta/eta0) ) )/( exp(alpha) - 1 );
            }
            else if ( eta > eta0 && eta < 1)
            {
                d_y = 1*eta0 + 1*(1-eta0)*( exp(alpha*(eta-eta0)/(1-eta0)) - 1 )/(exp(alpha) - 1);
            }

            X[i][j] = (X[i][nj] - X[i][1])*d_y + X[i][1];
            Y[i][j] = (Y[i][nj] - Y[i][1])*d_y + Y[i][1];
        }
    }
}

void equation6() // đang nén bên dưới
{
    p = 0.1;
    q = 4;
    d_eta = (double)1/(nj - 1);
    for(int i=1;i<=ni;i++)
    {
        for(int j=2;j<=nj-1;j++)
        {
            eta = (double)d_eta*(j-1);
            double d_y = p*eta+(1-p)*(double)(1 -  tanh(q*(1-eta))/tanh(q)  );

            X[i][j] = (X[i][nj] - X[i][1])*d_y + X[i][1];
            Y[i][j] = (Y[i][nj] - Y[i][1])*d_y + Y[i][1];
        }
    }
}
void xuatData()
{
	ofstream outfile;
	outfile.open("chialuoiKT.tec");
	outfile << "VARIABLES = X Y" << endl;
	outfile << "ZONE I = " << ni << ", J = " << nj << ", F = POINT" << endl;

	for (int j = 1; j <= nj ; j++) {
		for (int i = 1; i <= ni; i++) {
			outfile << X[i][j] << '\t' << Y[i][j] << endl;
		}
	}

	outfile.close();
}
void exportDAT()
{
	ofstream outfiledat;
	outfiledat.open("chialuoi.dat");
	for (int j = 1; j <= nj; j++)
	{
		for (int i = 1; i <= ni; i++)
		{
			outfiledat << X[i][j] << "\t\t" << Y[i][j] <<endl;
		}
	}
	outfiledat.close();
}
