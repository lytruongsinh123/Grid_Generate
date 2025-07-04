#include <iostream>
#include <cmath>
#include <fstream>
#define MAX 100
#define PI 3.14159265358979
using namespace std;
int Ny = 50, Nx = 50;
double L = 3.0, H_total = 2.0, L1 = 1.0, H = 0.5;
double X[MAX][MAX], Y[MAX][MAX];

void boundary_divide()
{
    for (int i = 1; i <= Nx; i++)
    {
        X[i][1] = L * (i - 1) / (Nx - 1);
        Y[i][Ny] = H_total;
        X[i][Ny] = X[i][1];
    }

    for (int i = 1; i <= Nx; i++)
    {
        if (X[i][1] >= L1 && X[i][1] <= (H / tan(PI / 3.0)))
        {
            double x = X[i][1] - L1;
            Y[i][1] = x * tan(PI / 3);
        }
        else if (X[i][1] >= L1 + (H / tan(PI / 3.0)) && X[i][1] <= L1 + (H / tan(PI / 3.0)) + H)
        {
            double x = X[i][1] - L1 - (H / tan(PI / 3.0));
            Y[i][1] = H - x;
        }
        else if (X[i][1] >= L1 + (H / tan(PI / 3.0)) + H)
        {
            Y[i][1] = 0;
        }
    }
}

void equal_divide()
{
    for (int i = 1; i <= Nx; i++)
    {
        for (int j = 2; j <= Ny; j++)
        {
            X[i][j] = X[i][1] + ((j - 1) * (X[i][Ny] - X[i][1])) / (Ny - 1);
            Y[i][j] = Y[i][1] + ((j - 1) * (Y[i][Ny] - Y[i][1])) / (Ny - 1);
        }
    }
}

double transform_Middle_doc(double xi, double w, double xi0, double alpha)
{
    if (xi <= xi0)
    {
        return w * xi0 * (exp(alpha) - exp(alpha * (1 - xi / xi0))) / (exp(alpha) - 1);
    }
    else
    {
        return w * xi0 + w * (1 - xi0) * ((exp(alpha * (xi - xi0) / (1 - xi0)) - 1) / (exp(alpha) - 1));
    }
}
void express_middle_doc() {
    double alpha = 5.0;  // Hệ số dồn lưới
    double xi0 = 0.5;    // Vị trí tập trung lưới
    for(int i = 1; i <= Ny; i++) { // hàng
        for(int j = 2; j <= Nx; j++) { // cột
            double xi = (double)(j - 1) / (Ny - 1);
            double w = X[Ny][i] - X[1][i];
            X[j][i] = X[1][i] + transform_Middle_doc(xi, w, xi0, alpha);
            Y[j][i] = Y[j][i];
        }
    }
}

void export_data()
{
    ofstream outfile("data10.dat");
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
            outfile << X[i][j] << '\t' << Y[i][j] << endl;
        }
    }
    outfile.close();
    cout << "Dữ liệu đã được ghi vào 'data10.dat' thành công." << endl;
}

int main()
{
    boundary_divide();
    equal_divide(); // ✅ Cần khởi tạo lưới đều trước khi dồn lưới
    express_middle_doc();
    export_data();
    return 0;
}
