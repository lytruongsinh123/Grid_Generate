#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

const int ni = 100, nj = 30;
const double L = 1.0, H = 0.2;
const double alpha = 2.0, p = 0.1, q = 3.0;
const double dxi = 1.0 / (ni - 1), det = 1.0 / (nj - 1);

int main() {
    std::vector<std::vector<double>> x(ni, std::vector<double>(nj, 0.0));
    std::vector<std::vector<double>> y(ni, std::vector<double>(nj, 0.0));

    // Set wall boundary
    for (int i = 0; i < ni; ++i) {
        double xi = dxi * i;
        y[i][0] = 0.0;
        x[i][0] = L * xi;
        y[i][nj - 1] = H;
        x[i][nj - 1] = x[i][0];
    }

    // Set initial grid linearly interpolated + stretching
    for (int i = 0; i < ni; ++i) {
        for (int j = 1; j < nj - 1; ++j) {
            double eta = det * j;
            double dy = eta;
            y[i][j] = (y[i][nj - 1] - y[i][0]) * dy + y[i][0];
            x[i][j] = (x[i][nj - 1] - x[i][0]) * dy + x[i][0];
        }
    }

    // Write to data file
    std::ofstream dataFile("Algebraic_Grid_Generation_Test.dat");
    dataFile << ni << " " << nj << "\n";
    for (int j = 0; j < nj; ++j) {
        for (int i = 0; i < ni; ++i) {
            dataFile << x[i][j] << " ";
        }
        dataFile << "\n";
    }
    for (int j = 0; j < nj; ++j) {
        for (int i = 0; i < ni; ++i) {
            dataFile << y[i][j] << " ";
        }
        dataFile << "\n";
    }
    dataFile.close();

    // Write to Tecplot file
    std::ofstream tecFile("Algebraic_Grid_Generation_Test.tec");
    tecFile << "VARIABLES = X Y\n";
    tecFile << "ZONE I=" << ni << ", J=" << nj << ", F=POINT\n";
    tecFile << std::scientific << std::setprecision(10);
    for (int j = 0; j < nj; ++j) {
        for (int i = 0; i < ni; ++i) {
            tecFile << x[i][j] << " " << y[i][j] << "\n";
        }
    }
    tecFile.close();

    return 0;
}
