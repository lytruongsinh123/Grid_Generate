#include <iostream>
#include <fstream>
#include <cmath>
#define MAX 1000
using namespace std;
double R = 1.0, L = 2.0;
int Nx, Ny;
double x[MAX][MAX], y[MAX][MAX];
double transform_Middle(double eta, double h, double eta0, double alpha)
{
    if (eta <= eta0 && eta >= 0)
    {
        return h * eta0 * (exp(alpha) - exp(alpha * (1 - eta / eta0))) / (exp(alpha) - 1);
    }
    else
    {
        return h * eta0 + h * (1 - eta0) * ((exp(alpha * (eta - eta0) / (1 - eta0)) - 1) / (exp(alpha) - 1));
    }
}
void bounder_divide() {
    for (int i = 1; i <= Nx; i++)
    {
        x[i][1] = (L  * (i - 1)) / (Nx - 1);
        y[i][1] = - sqrt(pow(R,2) - pow(x[i][1] - R, 2));
        x[i][Ny] = x[i][1];
        y[i][Ny] = sqrt(pow(R,2) - pow(x[i][1] - R, 2));
    }
}
void equally_divided()
{
    for (int i = 1; i <= Nx; i++)
    {
        for (int j = 2; j <= Ny; j++)
        {
            x[i][j] = x[i][1];
            y[i][j] = y[i][1] + ((j - 1) * (y[i][Ny] - y[i][1])) / (Ny - 1);
        }
    }
}

void express_grid_middle(){
    double eta0 = 0.5; // Vị trí dồn lưới
    for (int i = 1; i <= Nx; i++)
    {
        for (int j = 2; j <= Ny; j++)
        {
            double h = y[i][Ny] - y[i][1];
            double eta = (double)(j - 1) / (Ny - 1);
            x[i][j] = x[i][1];
            y[i][j] = transform_Middle(eta, h, eta0, 5.0) + y[i][1];
        }
    }
}
void export_data()
{
    ofstream outfile("data6.dat");
    if (!outfile)
    {
        cerr << "Lỗi: Không thể mở file để ghi!" << endl;
        return;
    }
    outfile << "VARIABLES = X Y" << endl;
    outfile << "ZONE I = " << Nx << ", J = " << Ny << ", F = POINT" << endl;

    for (int j = 1; j <= Ny; j++)
    {
        for (int i = 1; i <= Nx; i++)
        {
            outfile << x[i][j] << '\t' << y[i][j] << endl;
        }
    }
    outfile.close();
    cout << "Dữ liệu đã được ghi vào 'data6.dat' thành công." << endl;
}
int main() {
    Nx = 60;
    Ny = 60;
    bounder_divide();
    // equally_divided();
    express_grid_middle();
    export_data();
    return 0;
}