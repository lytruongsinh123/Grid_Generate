#include <iostream>
#include <cmath>
#include <fstream>
#define MAX 10000
using namespace std;
double H, L1, L2, L3;
int Nx1, Nx2, Nx3, Ny;
double x[MAX][MAX], y[MAX][MAX];
double R = 3.0;
// dồn lưới giữa
double transform_Middle(double eta, double h, double eta0, double alpha)
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
// dồn lưới hai biên
double transform_Two_Boundary(double eta, double h, double eta0, double alpha)
{
    if (eta >= 0 && eta <= eta0)
    {
        return h * eta0 * (exp(alpha * (eta / eta0) - 1)) / (exp(alpha) - 1);
    }
    else 
    {
        return h - h * (1 - eta0) * (exp(alpha * ((1 - eta) / (1 - eta0))) - 1) / (exp(alpha) - 1);
    }
}
void boundary_devider()
{
    for (int i = 1; i <= Nx1; i++)
    {
        x[i][1] = (L1 * (i - 1)) / (Nx1 - 1);
        y[i][1] = 0;
        x[i][Ny] = x[i][1];
        y[i][Ny] = H;
    }
    for (int i = Nx1 + 1; i <= Nx1 + Nx2 - 1; i++)
    {
        x[i][1] = L1 + (L2 * (i - Nx1)) / (Nx2 - 1);
        y[i][1] = sqrt(pow(R, 2) - pow(R - (x[i][1] - L1), 2));
        x[i][Ny] = x[i][1];
        y[i][Ny] = H;
    }
    for (int i = Nx1 + Nx2; i <= Nx1 + Nx2 + Nx3 - 1; i++)
    {
        x[i][1] = L1 + L2 + (L3 * (i - (Nx1 + Nx2 - 1))) / (Nx3 - 1);
        y[i][1] = 0;
        x[i][Ny] = x[i][1];
        y[i][Ny] = H;
    }
}
void express_grid()
{
    double eta0 = 1; // Vị trí dồn lưới
    for (int i = 1; i <= Nx1 + Nx2 + Nx3 - 1; i++)
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
void express_grid_Two_Boundary() {
    double eta0 = 0.5; // Vị trí dồn lưới
    for (int i = 1; i <= Nx1 + Nx2 + Nx3 - 1; i++)
    {
        for (int j = 2; j <= Ny; j++)
        {
            double h = y[i][Ny] - y[i][1];
            double eta = (double)(j - 1) / (Ny - 1);
            x[i][j] = x[i][1];
            y[i][j] = transform_Two_Boundary(eta, h, eta0, 5) + y[i][1];
        }
    }
}
// chia đều lưới
void equally_divided()
{
    for (int i = 1; i <= Nx1 + Nx2 + Nx3 - 1; i++)
    {
        for (int j = 2; j <= Ny; j++)
        {
            x[i][j] = x[i][1] + ((j - 1) * (x[i][Ny] - x[i][1])) / (Ny - 1);
            y[i][j] = y[i][1] + ((j - 1) * (y[i][Ny] - y[i][1])) / (Ny - 1);
        }
    }
}
void export_data()
{
    ofstream outfile("data4.dat");
    if (!outfile)
    {
        cerr << "Lỗi: Không thể mở file để ghi!" << endl;
        return;
    }
    outfile << "VARIABLES = X Y" << endl;
    outfile << "ZONE I = " << Nx1 + Nx2 + Nx3 - 1 << ", J = " << Ny << ", F = POINT" << endl;

    for (int j = 1; j <= Ny; j++)
    {
        for (int i = 1; i <= Nx1 + Nx2 + Nx3 - 1; i++)
        {
            outfile << x[i][j] << '\t' << y[i][j] << endl;
        }
    }
    outfile.close();
    cout << "Dữ liệu đã được ghi vào 'data4.dat' thành công." << endl;
}
int main()
{
    H = 5.0;
    L1 = 6.0;
    L2 = 2 * R;
    L3 = 6.0;
    Nx1 = 50;
    Nx2 = 60;
    Nx3 = 50;
    Ny = 50;
    boundary_devider();
    // equally_divided();
    // express_grid();
    express_grid_Two_Boundary();
    export_data();
}