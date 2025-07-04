#include <iostream>
#include <cmath>
#include <fstream>

#define M_PI 3.14159265358979323846
#define MAX 1000

using namespace std;

int Nx, Ny;
double eta, p, q, dy, alpha, xi, H, L, theta;
double x[MAX][MAX], y[MAX][MAX];

void bounder_divide(int start, int Nx, double L)
{
    for (int i = 0; i < Nx; i++)
    {
        xi = (L * i) / (Nx - 1);
        x[i][0] = xi;
        y[i][0] = 0;
        x[i][Ny - 1] = xi;
        y[i][Ny - 1] = H;
    }
}

void equally_divided(int start, int Nx)
{
    for (int i = 0; i < Nx; i++)
    {
        for (int j = 1; j < Ny - 1; j++)
        {
            x[i][j] = x[i][0] + (j * (x[i][Ny - 1] - x[i][0])) / (Ny - 1);
            y[i][j] = y[i][0] + (j * (y[i][Ny - 1] - y[i][0])) / (Ny - 1);
        }
    }
}

void coordinate_transformation(int start, int Nx, double theta)
{
    double tempX, tempY;

    for (int i = 0; i < Nx; i++)
    {
        for (int j = 0; j < Ny; j++)
        {
            tempX = x[i][j] * cos(theta) - y[i][j] * sin(theta);
            tempY = x[i][j] * sin(theta) + y[i][j] * cos(theta);

            x[i][j] = tempX;
            y[i][j] = tempY;
        }
    }
}

void export_data(int startIdx, int endIdx, bool append = false)
{
    ofstream outfile;

    if (append)
        outfile.open("data1.tec", ios::app); // Mở file ở chế độ append
    else
        outfile.open("data1.tec"); // Mở file ở chế độ ghi đè (cho lần đầu)

    if (!outfile)
    {
        cerr << "Không thể mở file để ghi dữ liệu!" << endl;
        return;
    }

    if (!append)
    {
        outfile << "VARIABLES = X Y" << endl;
    }

    outfile << "ZONE I = " << (endIdx - startIdx) << ", J = " << Ny << ", F = POINT" << endl;

    for (int j = 0; j < Ny; j++)
    {
        for (int i = startIdx; i < endIdx; i++)
        {
            outfile << x[i][j] << '\t' << y[i][j] << endl;
        }
    }

    outfile.close();
}

int main()
{
    L = 5.0;
    H = 2.0;
    Nx = 100;
    Ny = 50;

    // Khoảng từ 0 -> 2
    bounder_divide(0, Nx / 2, 2.0);
    equally_divided(0, Nx / 2);
    coordinate_transformation(0, Nx / 2, M_PI / 2.0);
    export_data(0, Nx / 2, false); // Ghi mới file

    // Khoảng từ 2 -> 4√2
    bounder_divide(Nx / 2, Nx, 4.0 * sqrt(2.0));
    equally_divided(Nx / 2, Nx);
    coordinate_transformation(Nx / 2, Nx, M_PI / 4.0);
    export_data(Nx / 2, Nx, true); // Ghi tiếp vào file

    cout << "Xuất dữ liệu thành công!\n";
    return 0;
}
