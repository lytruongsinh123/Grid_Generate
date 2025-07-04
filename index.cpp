#include <iostream>
#include <cmath>
#include <fstream>

#define MAX 10000
using namespace std;

int Nx, Ny;
double eta, p, q, dy, xi, H, L;
double x[MAX][MAX], y[MAX][MAX];

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

    int extra_rows = Ny / 2;  
    for (int j = 1; j <= extra_rows; j++)
    {
        for (int i = 1; i <= Nx; i++)
        {
            y[i][Ny + j] = H + ((i - 1) * L / (Nx - 1)) * (double(j) / extra_rows);
            x[i][Ny + j] = x[i][1];
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
            y[i][j] = y[i][1] + ((j - 1) * (y[i][Ny] - y[i][1])) / (Ny - 1);
        }
    }
}

// Hàm làm trơn lưới bằng cách lấy trung bình có trọng số của các điểm lân cận
void smooth_grid(int iterations = 5)
{
    for (int k = 0; k < iterations; k++)
    {
        for (int i = 2; i < Nx; i++)
        {
            for (int j = 2; j < Ny; j++)
            {
                // Lấy trung bình của các điểm xung quanh
                x[i][j] = 0.25 * (x[i - 1][j] + x[i + 1][j] + x[i][j - 1] + x[i][j + 1]);
                y[i][j] = 0.25 * (y[i - 1][j] + y[i + 1][j] + y[i][j - 1] + y[i][j + 1]);
            }
        }
    }
}

void export_data()
{
    ofstream outfile;
    outfile.open("data.dat");

    int total_rows = Ny + (Ny / 2); 
    outfile << "VARIABLES = X Y" << endl;
    outfile << "ZONE I = " << Nx << ", J = " << total_rows << ", F = POINT" << endl;

    for (int j = 1; j <= total_rows; j++)
    {
        for (int i = 1; i <= Nx; i++)
        {
            outfile << x[i][j] << '\t' << y[i][j] << endl;
        }
    }

    outfile.close();
}

int main()
{
    L = 1;
    H = 1;
    Nx = 50;
    Ny = 50;
    bounder_divide();
    equally_divided();
    smooth_grid(); // Gọi hàm làm trơn
    export_data();
}
