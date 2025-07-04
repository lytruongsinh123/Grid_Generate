#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>

constexpr int MAX = 10000;
constexpr double PI = 3.141592653589793;

using namespace std;

int Ny = 100, Nx1 = 100, Nx2 = 50, Nx3 = 50, Nx4 = 100;
double L1 = 1.0, L4 = 2.0, H_total = 2.0, H = 0.5, L3 = H, L2 = H / tan(PI / 3);
int ni = Nx1 + Nx2 + Nx3 + Nx4 - 3, nj = Ny;

vector<vector<double>> X(ni + 1, vector<double>(nj + 1)), Y(ni + 1, vector<double>(nj + 1)), XOld(ni + 1, vector<double>(nj + 1)), YOld(ni + 1, vector<double>(nj + 1));

double transform_Middle_Horizontal(double eta, double h, double eta0, double alpha) {
    if (eta <= eta0) {
        return h * eta0 * (exp(alpha) - exp(alpha * (1 - eta / eta0))) / (exp(alpha) - 1);
    } else {
        return h * eta0 + h * (1 - eta0) * ((exp(alpha * (eta - eta0) / (1 - eta0)) - 1) / (exp(alpha) - 1));
    }
}

void boundary_divide() {
    for (int i = 1; i <= ni; i++) {
        if (i <= Nx1) {
            X[i][1] = L1 * (i - 1) / (Nx1 - 1);
            Y[i][1] = 0;
        } else if (i <= Nx1 + Nx2 - 1) {
            X[i][1] = L1 + L2 * (i - Nx1) / (Nx2 - 1);
            Y[i][1] = tan(PI / 3.0) * (X[i][1] - L1);
        } else if (i <= Nx1 + Nx2 + Nx3 - 2) {
            X[i][1] = L1 + L2 + L3 * (i - (Nx1 + Nx2 - 1)) / (Nx3 - 1);
            Y[i][1] = H - (L3 * (i - (Nx1 + Nx2 - 1))) / (Nx3 - 1);
        } else {
            X[i][1] = L1 + L2 + L3 + L4 * (i - (Nx1 + Nx2 + Nx3 - 2)) / (Nx4 - 1);
            Y[i][1] = 0;
        }
        X[i][Ny] = X[i][1];
        Y[i][Ny] = H_total;
    }
}

void elipToMesh() {
    XOld = X;
    YOld = Y;
    for (int count = 0; count < 20; count++) {
        for (int j = 2; j < nj; j++) {
            for (int i = 2; i < ni; i++) {
                double a1 = (XOld[i][j + 1] - X[i][j - 1]) / 2;
                double a2 = (YOld[i][j + 1] - Y[i][j - 1]) / 2;
                double c1 = (XOld[i + 1][j] - X[i - 1][j]) / 2;
                double c2 = (YOld[i + 1][j] - Y[i - 1][j]) / 2;
                double alpha = a1 * a1 + a2 * a2;
                double beta = a1 * c1 + a2 * c2;
                double gamma = c1 * c1 + c2 * c2;
                X[i][j] = (alpha * (XOld[i + 1][j] + X[i - 1][j]) + gamma * (XOld[i][j + 1] + X[i][j - 1]) - (beta / 2) * (XOld[i + 1][j + 1] + X[i - 1][j - 1] - XOld[i - 1][j + 1] - XOld[i + 1][j - 1])) / (2 * (alpha + gamma));
                Y[i][j] = (alpha * (YOld[i + 1][j] + Y[i - 1][j]) + gamma * (YOld[i][j + 1] + Y[i][j - 1]) - (beta / 2) * (YOld[i + 1][j + 1] + Y[i - 1][j - 1] - YOld[i - 1][j + 1] - YOld[i + 1][j - 1])) / (2 * (alpha + gamma));
            }
        }
        XOld = X;
        YOld = Y;
    }
}

void export_data()
{
    ofstream outfile("test.dat");
    if (!outfile)
    {
        cerr << "Lỗi: Không thể mở file để ghi!" << endl;
        return;
    }
    outfile << "VARIABLES = X Y" << endl;
    outfile << "ZONE I = " << ni << ", J = " << nj << ", F = POINT" << endl;

    for (int j = 1; j <= nj; j++)
    {
        for (int i = 1; i <= ni; i++)
        {
            outfile << X[i][j] << '\t' << Y[i][j] << endl;
        }
    }
    outfile.close();
    cout << "Dữ liệu đã được ghi vào 'test.dat' thành công." << endl;
}


int main() {
    boundary_divide();
    elipToMesh();
    export_data();
    return 0;
}
