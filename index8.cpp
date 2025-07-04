#include <iostream>
#include <fstream>
#define MAX 10000
using namespace std;
int Nx = 10, Ny = 10;
double L = 3.0, H = 3.0;
double X[MAX][MAX], Y[MAX][MAX];

void bounder_divide()
{
    for (int i = 1; i <= Nx; i++)
    {
        X[i][1] = L * (i - 1) / (Nx - 1);
        Y[i][1] = 0;
        Y[i][Ny] = H;
        X[i][Ny] = (H + L / 3.0) * (i - 1) / (Nx - 1);
    }
}
void equal_divide()
{
    for (int i = 1; i <= Nx; i++)
    { // cột
        for (int j = 2; j <= Ny; j++)
        { // hàng
            if (i == 1)
            {
                X[i][j] = 0;
                Y[i][j] = (j - 1) * L / (Ny - 1);
            }
            else
            {
                Y[i][j] = Y[1][j];
                double tan_anpha = (Y[i][Ny] - Y[i][1]) / (X[i][Ny] - X[i][1]);
                X[i][j] = Y[i][j] / tan_anpha + X[i][1];
            }
        }
    }
}


void export_data()
{
    ofstream outfile("data8.dat");
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
    cout << "Dữ liệu đã được ghi vào 'data8.dat' thành công." << endl;
}
int main()
{
    bounder_divide();
    equal_divide();
    export_data();
    return 0;
}
