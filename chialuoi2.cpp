#include <iostream>
#include <cmath>
#include <fstream>
#define MAX 10000
using namespace std;
int Nx, Ny;
double eta, p, q, dy, alpha, xi, H, L;
double x[MAX][MAX], y[MAX][MAX];
double alpha_x = 2.0, alpha_y = 10.0;
double transformY(double eta, double h, double eta0, double alpha) {
    if (eta <= eta0) {
        return h*eta0 * (exp(alpha) - exp(alpha * (1 - eta / eta0))) / (exp(alpha) - 1);
    } else {
        return h*eta0 + h * (1 - eta0) * ((exp(alpha * (eta - eta0) / (1 - eta0)) - 1) / (exp(alpha) - 1));
    }
}
void bounder_divide()
{

    for (int i = 1; i <= Nx; i++)
    {
        xi = (L * (i - 1)) / (Nx - 1);
        x[i][1] = xi;
        y[i][1] = 0;
        x[i][Ny] = xi;
        y[i][Ny] = H;
    }
}
void Slanted_line() {
    double h = 5.0;
    double eta0 = 0; // Vị trí dồn lưới
    
    for (int i = 1; i <= Nx; i++) {
        for (int j = 2; j < Ny; j++) {
            double eta = (double)(j - 1) / (Ny - 1);
            x[i][j] = x[i][1];
            y[i][j] = transformY(eta, h, eta0, alpha_y);
        }
    }
}
void equally_divided()
{
    for (int i = 1; i <= Nx; i++)
    {
        for (int j = 2; j < Ny; j++)
        {
            x[i][j] = x[i][1] + ((j - 1) * (x[i][Ny] - x[i][1])) / (Ny - 1);
            y[i][j] = y[i][1] + ((j - 1) *(y[i][Ny] - y[i][1])) / (Ny - 1);
        }
    }
}
void print_data() {
	for (int j = 1; j <= Ny; j++) {
		for (int i = 1; i <= Nx ; i++) {
            cout << x[i][j] << endl;
        }
	}
}
void export_data() {
    ofstream outfile;
    outfile.open("data.tec");
    outfile << "VARIABLES = X Y" << endl;
	outfile << "ZONE I = " << Nx << ", J = " << Ny << ", F = POINT" << endl;
    for (int j = 1; j <= Ny; j++) {
		for (int i = 1; i <= Nx ; i++) {
            outfile << x[i][j] << '\t' << y[i][j] << endl;;
        }
	}
    outfile.close();
}
int main()
{
    L = 5.0;
    H = 5.0;
    Nx = 100;
    Ny = 100;
    bounder_divide();
    Slanted_line();
    print_data();
    export_data();
}