#include <iostream>
#include <fstream>
#include <cmath>
#define MAX 1000
using namespace std;
int Nx1 = 50, Ny = 50, Nx2 = 10;
double H = 5.0, L1 = 7.0, L2 = 3.0;
double alpha = 3.0; // hệ số dồn lưới 
double x[MAX][MAX], y[MAX][MAX];
double transformY(double eta, double h, double eta0, double alpha)
{
    if (eta <= eta0)
    {
        return h * eta0 * (exp(alpha) - exp(alpha * (1 - eta / eta0))) / (exp(alpha) - 1);
    }
    else
    {
        return h * eta0 + h * (1 - eta0) * ((exp(alpha * (eta - eta0) / (1 - eta0)) - 1) / (exp(alpha) - 1));
    }
}
void boundery_divide()
{
    for (int i = 1; i <= Nx1; i++)
    {
        x[i][1] = (L1 * (i - 1)) / (Nx1 - 1);
        y[i][1] = 0;
        x[i][Ny] = x[i][1];
        y[i][Ny] = H;
    }
    for (int i = Nx1; i <= Nx1 + Nx2 - 1; i++)
    {
        x[i][1] = L1 + (L2 * (i - Nx1)) / (Nx2 - 1);
        y[i][1] = ((L2 * (i - Nx1)) / (Nx2 - 1)) / 3.0;
        x[i][Ny] = x[i][1];
        y[i][Ny] = H;
    }
}
void equally_divided()
{
    for (int i = 1; i <= Nx1 + Nx2 - 1; i++)
    {
        for (int j = 2; j <= Ny; j++)
        {
            x[i][j] = x[i][1] + ((j - 1) * (x[i][Ny] - x[i][1])) / (Ny - 1);
            y[i][j] = y[i][1] + ((j - 1) * (y[i][Ny] - y[i][1])) / (Ny - 1);
        }
    }
}

void express_grid()
{
    double eta0 = 0; // Vị trí dồn lưới
    for (int i = 1; i <= Nx1 + Nx2 - 1; i++)
    {
        for (int j = 2; j <= Ny; j++)
        {
            double h = y[i][Ny] - y[i][1];
            double eta = (double)(j - 1) / (Ny - 1);
            x[i][j] = x[i][1];
            y[i][j] = transformY(eta, h, eta0, alpha) + y[i][1];
        }
    }
}

void express_two_boudary() {
    for (int i = 1; i <= Nx1; i++)
    {
    }
    for (int i = Nx1; i <= Nx1 + Nx2 - 1; i++)
    {
    }
}
void export_data()
{
    ofstream outfile("data3.dat");
    if (!outfile)
    {
        cerr << "Lỗi: Không thể mở file để ghi!" << endl;
        return;
    }
    outfile << "VARIABLES = X Y" << endl;
    outfile << "ZONE I = " << Nx1 + Nx2 - 1 << ", J = " << Ny << ", F = POINT" << endl;

    for (int j = 1; j <= Ny; j++)
    {
        for (int i = 1; i <= Nx1 + Nx2 - 1; i++)
        {
            outfile << x[i][j] << '\t' << y[i][j] << endl;
        }
    }
    outfile.close();
    cout << "Dữ liệu đã được ghi vào 'data3.dat' thành công." << endl;
}
int main()
{
    boundery_divide();
    express_grid();
    export_data();
    return 0;
}