// #include <iostream>
// #include <cmath>
// #include <fstream>

// #define MAX 10000
// using namespace std;

// int Nx, Ny;
// double eta, p, q, dy, xi, H, L, d_eta, d_y;
// double x[MAX][MAX], y[MAX][MAX];
// double alpha, xi_0;

// void bounder_divide()
// {
//     for (int i = 1; i <= Nx; i++)
//     {
//         xi = (L * (i - 1)) / (Nx - 1);
//         x[i][1] = xi;
//         y[i][1] = 0;
//         x[i][Ny] = xi;
//         y[i][Ny] = H +  xi;
//     }
// }

// void equally_divided()
// {
//     for (int i = 1; i <= Nx; i++)
//     {
//         for (int j = 2; j < Ny; j++)
//         {
//             x[i][j] = x[i][1] + ((j - 1) * (x[i][Ny] - x[i][1])) / (Ny - 1);
//             y[i][j] = y[i][1] + ((j - 1) * (y[i][Ny] - y[i][1])) / (Ny - 1);
//         }
//     }
// }


// void export_data()
// {
//     ofstream outfile;
//     outfile.open("data2.dat");

//     int total_rows = Ny + (Ny / 2); 
//     outfile << "VARIABLES = X Y" << endl;
//     outfile << "ZONE I = " << Nx << ", J = " << Ny << ", F = POINT" << endl;

//     for (int j = 1; j <= Ny; j++)
//     {
//         for (int i = 1; i <= Nx; i++)
//         {
//             outfile << x[i][j] << '\t' << y[i][j] << endl;
//         }
//     }

//     outfile.close();
// }

// int main()
// {
//     L = 1;
//     H = 0.5;
//     Nx = 100;
//     Ny = 100;
//     bounder_divide();
//     equally_divided();
//     export_data();
// }
#include <iostream>
#include <cmath>
#include <fstream>

#define MAX 10000
using namespace std;

int Nx, Ny;
double H, L;
double x[MAX][MAX], y[MAX][MAX];
double alpha = 5.0; // Hệ số dồn lưới



double transformY(double eta, double h, double eta0, double alpha) {
    if (eta <= eta0) {
        return h*eta0 * (exp(alpha) - exp(alpha * (1 - eta / eta0))) / (exp(alpha) - 1);
    } else {
        return h*eta0 + h * (1 - eta0) * ((exp(alpha * (eta - eta0) / (1 - eta0)) - 1) / (exp(alpha) - 1));
    }
}

void bounder_divide() {
    for (int i = 1; i <= Nx; i++) {
        double xi = (double)(i - 1) / (Nx - 1);
        x[i][1] = xi;
        y[i][1] = 0;
        x[i][Ny] = xi;
        y[i][Ny] = H + x[i][1];
    }
}

void refined_grid() {
    double h;
    double eta0 = 0.7; // Vị trí dồn lưới
    
    for (int i = 1; i <= Nx; i++) {
        h = H + x[i][1];
        for (int j = 2; j < Ny; j++) {
            double eta = (double)(j - 1) / (Ny - 1);
            x[i][j] = x[i][1];
            y[i][j] = transformY(eta, h, eta0, alpha);
        }
    }
}

void export_data() {
    ofstream outfile("data2.dat");
    if (!outfile) {
        cerr << "Lỗi: Không thể mở file để ghi!" << endl;
        return;
    }
    outfile << "VARIABLES = X Y" << endl;
    outfile << "ZONE I = " << Nx << ", J = " << Ny << ", F = POINT" << endl;

    for (int j = 1; j <= Ny; j++) {
        for (int i = 1; i <= Nx; i++) {
            outfile << x[i][j] << '\t' << y[i][j] << endl;
        }
    }

    outfile.close();
    cout << "Dữ liệu đã được ghi vào 'data2.dat' thành công." << endl;
}

int main() {
    L = 1.0;
    H = 0.5;
    Nx = 50;
    Ny = 50;
    bounder_divide();
    refined_grid();
    export_data();
}
