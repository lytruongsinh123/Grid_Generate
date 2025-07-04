#include <iostream>
#include <fstream>
#define MAX 10000
using namespace std;
int Nx = 10 , Ny = 10;
double L = 3.0, H = 3.0;
double X[MAX][MAX], Y[MAX][MAX];

void bounder_divide()
{
    for (int i = 1; i <= Ny; i++)
    {
        Y[1][i] = L * (i - 1) / (Ny - 1);
        X[1][i] = 0;
        Y[Nx][i] = Y[1][i];
        X[Nx][i] = H +  Y[Nx][i] / 3.0;
    }
}
void equal_divide() {
    for(int i = 1; i <= Ny; i++) { // hàng
        for(int j = 2; j <= Nx;j++) { // cột
            Y[j][i] = Y[1][i];
            X[j][i] = (j - 1) * (X[Nx][i] - X[1][i]) / (Nx - 1) + X[1][i];
        }
    }
}
void print() {
    for (int i = 1; i <= Ny; i++) { // hàng
        for(int j = 1; j <= Ny; j++) { // cột 
            cout << X[j][i] << "\t" << Y[j][i] << endl;
        }
        cout << endl;
    }

    cout << endl;
    cout << endl;
    cout << X[2][2] << " " << Y[2][2] << endl;
}

void export_data()
{
    ofstream outfile("data7.dat");
    if (!outfile)
    {
        cerr << "Lỗi: Không thể mở file để ghi!" << endl;
        return;
    }
    outfile << "VARIABLES = X Y" << endl;
    outfile << "ZONE I = " << Nx << ", J = " << Ny << ", F = POINT" << endl;

    for (int i = 1; i <= Ny; i++) { // hàng
        for(int j = 1; j <= Nx; j++) { // cột 
            outfile << X[j][i] << "\t" << Y[j][i] << endl;
        }
        cout << endl;
    }
    outfile.close();
    cout << "Dữ liệu đã được ghi vào 'data7.dat' thành công." << endl;
}

int main() {
    bounder_divide();
    equal_divide();
    print();
    export_data();
    return 0;
}
