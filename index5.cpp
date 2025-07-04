#include <iostream>
#include <fstream>
#define MAX 1000
using namespace std;
double H = 1.0, L = 1.0;
int Nx, Ny;
double x[MAX][MAX], y[MAX][MAX];

void bounder_divide() {
    for (int i = 1; i <= Nx; i++)
    {
        x[i][1] = (L * (i - 1)) / (Nx - 1);
        y[i][1] = 0;
        x[i][Ny] = x[i][1];
        y[i][Ny] = 1 - x[i][1];
    }
}
void equally_divided()
{
    for (int i = 1; i <= Nx; i++)
    {
        for (int j = 2; j <= Ny - 1; j++)
        {
            x[i][j] = x[i][1];
            y[i][j] = y[i][1] + ((j - 1) * (y[i][Ny] - y[i][1])) / (Ny - 1);
        }
    }
}
void export_data()
{
    ofstream outfile("data5.dat");
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
    cout << "Dữ liệu đã được ghi vào 'data5.dat' thành công." << endl;
}
int main() {
    Nx = 50;
    Ny = 50;
    bounder_divide();
    equally_divided();
    export_data();
    return 0;
}