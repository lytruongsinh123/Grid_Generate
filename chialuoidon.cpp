#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

int Nx, Ny;
double eta, d_eta, p, q, d_y, alpha, xi, d_xi, H, L;
double x[100][100], y[100][100];
double eta_0;


//Chia luoi bien tren va duoi
void boudary_layer() {
	//delta_xi va delta_eta
	d_xi = (double)1/(Nx - 1);
	d_eta = (double)1/(Ny - 1);
	for (int i = 1; i <= Nx; i++)  
    {
		xi = d_xi*(i - 1);
		y[i][1] = 0;
		x[i][1] = L*xi;
		cout << x[i][1] << endl;
		y[i][Ny] = H;
		x[i][Ny] = x[i][1];
	}
}

//Noi suy nen deu
void equally_divided() 
{
	for (int i = 1; i <= Nx; i++)
		for (int j = 2; j < Ny; j++) 
        {
			eta = d_eta*(j - 1);
			d_y = eta;
			cout << d_y << endl;
			x[i][j] = (x[i][Ny] - x[i][1])*d_y + x[i][1];
			y[i][j] = H*d_y + y[i][1];
		}
}

//Luoi equation 4
void equation_4() 
{
	eta_0 = 0.5;
	alpha = 5;
	for (int i = 1; i <= Nx; i++) 
    {
		for (int j = 2; j < Ny; j++) 
        {
			eta = d_eta*(j - 1);
			if (eta >= 0 && eta <= eta_0) {
				d_y = H*eta_0*(exp(alpha) - exp(alpha - (1 - eta/eta_0)))/(exp(alpha - 1));
			} else if (eta > eta_0 && eta < 1) {
				d_y = H*eta_0 + H*(1 -  eta_0)*((exp(alpha*(eta - eta_0)/(1 - eta_0)) - 1)/(exp(alpha - 1)));
			}
			cout << d_y << endl;
			x[i][j] = (x[i][Ny] - x[i][1])*d_y + x[i][1];
			y[i][j] = (y[i][Ny] - y[i][1])*d_y + y[i][1];
		}
	}
}

// In data ra man hinh
void print_data() {
	for (int j = 1; j <= Ny; j++) {
		for (int i = 1; i <= Nx; i++) {
			cout << x[i][j] << '\t' << y[i][j] << endl;
		}
	}
}

// Xuat data ra file
void export_data() {
	ofstream outfile;
	outfile.open("chiadeu.tec");
	outfile << "VARIABLES = X Y" << endl;
	outfile << "ZONE I = " << Nx << ", J = " << Ny << ", F = POINT" << endl;
	
	for (int j = 1; j <= Ny ; j++) {
		for (int i = 1; i <= Nx; i++) {
			outfile << x[i][j] << '\t' << y[i][j] << endl;
		}
	}
	outfile.close();
}

int main() {
	Nx = 50;
	Ny = 50;
	L = 5;
	H = 5;
	boudary_layer();
	
    equation_4();
	print_data();
	export_data();
	return 0;
}