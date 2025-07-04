#include <iostream>
#include <cmath>
#include <fstream>
#define MAX 1000
#define PI 3.141592653589793
using namespace std;
int Ny = 100, Nx1 = 100, Nx2 = 50, Nx3 = 50, Nx4 = 100;
double L1 = 1.0, L4 = 2.0, H_total = 2.0, H = 0.5, L3 = H, L2 = H / (double)tan(PI / 3);
double X[MAX][MAX], Y[MAX][MAX];
double transform_Middle_Horizontal(double eta, double h, double eta0, double alpha)
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
double transform_Two_Boundary_Horizontal(double eta, double h, double eta0, double alpha)
{
    if (eta <= eta0)
    {
        return h * eta0 * ((exp(alpha * (eta / eta0)) - 1) / (exp(alpha) - 1));
    }
    else
    {
        return h - h * (1 - eta0) * ((exp(alpha * ((1 - eta) / (1 - eta0))) - 1) / (exp(alpha) - 1));
    }
}

double transform_Middle_Vertical(double xi, double w, double xi0, double alpha)
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

double transform_Two_Boundary_Vertical(double xi, double w, double xi0, double alpha)
{
    if (xi <= xi0)
    {
        return w * xi0 * ((exp(alpha * (xi / xi0)) - 1) / (exp(alpha) - 1));
    }
    else
    {
        return w - w * (1 - xi0) * ((exp(alpha * ((1 - xi) / (1 - xi0))) - 1) / (exp(alpha) - 1));
    }
}
void boundary_divide()
{
    // Khối 1
    for (int i = 1; i <= Nx1; i++)
    {
        X[i][1] = L1 * (i - 1) / (Nx1 - 1);
        Y[i][1] = 0;
        X[i][Ny] = X[i][1];
        Y[i][Ny] = H_total;
    }

    // Khối 2
    for (int i = Nx1 + 1; i <= Nx1 + Nx2 - 1; i++)
    {
        X[i][1] = L1 + L2 * (i - Nx1) / (Nx2 - 1);
        Y[i][1] = (double)tan(PI / 3.0) * (X[i][1] - L1);
        X[i][Ny] = X[i][1];
        Y[i][Ny] = H_total;
    }

    // Khối 3
    for (int i = Nx1 + Nx2; i <= Nx1 + Nx2 + Nx3 - 2; i++)
    {
        X[i][1] = L1 + L2 + L3 * (i - (Nx1 + Nx2 - 1)) / (Nx3 - 1);
        Y[i][1] = H - (L3 * (i - (Nx1 + Nx2 - 1))) / (Nx3 - 1);
        X[i][Ny] = X[i][1];
        Y[i][Ny] = H_total;
    }

    // Khối 4
    for (int i = Nx1 + Nx2 + Nx3 - 1; i <= Nx1 + Nx2 + Nx3 + Nx4 - 3; i++)
    {
        X[i][1] = L1 + L2 + L3 + L4 * (i - (Nx1 + Nx2 + Nx3 - 2)) / (Nx4 - 1);
        Y[i][1] = 0;
        X[i][Ny] = X[i][1];
        Y[i][Ny] = H_total;
    }
}

void equal_divide()
{
    for (int i = 1; i <= Nx1 + Nx2 + Nx3 + Nx4 - 3; i++)
    {
        for (int j = 2; j <= Ny; j++)
        {
            X[i][j] = X[i][1] + ((j - 1) * (X[i][Ny] - X[i][1])) / (Ny - 1);
            Y[i][j] = Y[i][1] + ((j - 1) * (Y[i][Ny] - Y[i][1])) / (Ny - 1);
        }
    }
}

void express_middle_Horizontal()
{
    double alpha = 3.0;                                  // hệ số dồn lưới
    double eta0 = 1.0;                                   // Vị trí dồn lưới
    for (int i = 1; i <= Nx1 + Nx2 + Nx3 + Nx4 - 3; i++) // cột
    {
        for (int j = 2; j <= Ny; j++) // hàng
        {
            double h = Y[i][Ny] - Y[i][1];
            double eta = (double)(j - 1) / (Ny - 1);
            X[i][j] = X[i][1] + ((j - 1) * (X[i][Ny] - X[i][1])) / (Ny - 1);
            Y[i][j] = Y[i][1] + transform_Middle_Horizontal(eta, h, eta0, alpha);
        }
    }
}

// alpha xác định mức độ biến đổi của tọa độ eta theo một quy luật mũ (exp).
// Khi alpha lớn, các điểm lưới sẽ tập trung nhiều hơn gần eta0 và 1 - eta0, giúp tăng độ phân giải tại hai biên.
// Khi alpha = 0, hàm trở thành một phép nội suy tuyến tính từ 0 đến h.
void express_grid_Two_Boundary_Horizontal()
{
    double eta0 = 0.5;  // Vị trí phân chia lưới trên hai biên
    double alpha = 3.0; // hệ số dồn lưới
    for (int i = 1; i <= Nx1 + Nx2 + Nx3 + Nx4 - 3; i++)
    {
        for (int j = 2; j <= Ny; j++)
        {
            double h = Y[i][Ny] - Y[i][1];
            double eta = (double)(j - 1) / (Ny - 1);
            X[i][j] = X[i][1];
            Y[i][j] = transform_Two_Boundary_Horizontal(eta, h, eta0, alpha) + Y[i][1];
        }
    }
}

void express_middle_Vertical_Block1()
{
    double alpha = 5.0; // Hệ số dồn lưới
    double xi0 = 1.0;   // Vị trí tập trung lưới
    int Nx = Nx1;
    int start = 2;
    for (int i = 1; i <= Ny; i++)
    { // Duyệt từng hàng
        for (int j = start; j <= Nx; j++)
        { // Duyệt từng cột
            double xi = (double)(j - (start - 1)) / (Nx - 1);
            double w = X[Nx][i] - X[start - 1][i];
            X[j][i] = X[start - 1][i] + transform_Middle_Vertical(xi, w, xi0, alpha);
            Y[j][i] = Y[j][i];
        }
    }
}

void express_Two_Boundry_Vertical_Block1()
{
    double alpha = 5.0; // Hệ số dồn lưới
    double xi0 = 0.5;   // Vị trí tập trung lưới
    int Nx = Nx1;
    int start = 2;
    for (int i = 1; i <= Ny; i++)
    { // Duyệt từng hàng
        for (int j = start; j <= Nx; j++)
        { // Duyệt từng cột
            double xi = (double)(j - (start - 1)) / (Nx - 1);
            double w = X[Nx][i] - X[start - 1][i];
            X[j][i] = X[start - 1][i] + transform_Two_Boundary_Vertical(xi, w, xi0, alpha);
            Y[j][i] = Y[j][i];
        }
    }
}

void express_middle_Vertical_Block2()
{
    double alpha = 5.0; // Hệ số dồn lưới
    double xi0 = 0.0;   // Vị trí tập trung lưới
    int Nx = Nx1 + Nx2 - 1;
    int start = Nx1 + 1;
    for (int i = 1; i <= Ny; i++)
    { // Duyệt từng hàng
        for (int j = start; j <= Nx; j++)
        { // Duyệt từng cột
            double xi = (double)(j - (start - 1)) / (Nx2 - 1);
            double w = X[Nx][i] - X[start - 1][i];
            double tan_alpha = (Y[Nx][i] - Y[start - 1][i]) / (X[Nx][i] - X[start - 1][i]);

            // Tính toán vị trí mới sau khi dồn lưới
            double newX = X[start - 1][i] + transform_Middle_Vertical(xi, w, xi0, alpha);
            double deltaX = newX - X[j][i];
            // Cập nhật X và Y
            X[j][i] = newX;
            Y[j][i] += tan_alpha * deltaX;
        }
    }
}

void express_Two_Boundry_Vertical_Block2()
{
    double alpha = 5.0; // Hệ số dồn lưới
    double xi0 = 0.5;   // Vị trí tập trung lưới
    int Nx = Nx1 + Nx2 - 1;
    int start = Nx1 + 1;
    for (int i = 1; i <= Ny; i++)
    { // Duyệt từng hàng
        for (int j = start; j <= Nx; j++)
        { // Duyệt từng cột
            double xi = (double)(j - (start - 1)) / (Nx2 - 1);
            double w = X[Nx][i] - X[start - 1][i];
            double tan_alpha = (Y[Nx][i] - Y[start - 1][i]) / (X[Nx][i] - X[start - 1][i]);

            // Tính toán vị trí mới sau khi dồn lưới
            double newX = X[start - 1][i] + transform_Two_Boundary_Vertical(xi, w, xi0, alpha);
            double deltaX = newX - X[j][i];
            // Cập nhật X và Y
            X[j][i] = newX;
            Y[j][i] += tan_alpha * deltaX;
        }
    }
}
void express_middle_Vertical_Block3()
{
    double alpha = 5.0; // Hệ số dồn lưới
    double xi0 = 0.5;   // Vị trí tập trung lưới
    int Nx = Nx1 + Nx2 + Nx3 - 2;
    int start = Nx1 + Nx2;
    for (int i = 1; i <= Ny; i++)
    { // Duyệt từng hàng
        for (int j = start; j <= Nx; j++)
        { // Duyệt từng cột
            double xi = (double)(j - (start - 1)) / (Nx3 - 1);
            double w = X[Nx][i] - X[start - 1][i];
            double tan_alpha = (Y[start - 1][i] - Y[Nx][i]) / (X[Nx][i] - X[start - 1][i]);

            // Tính toán vị trí mới sau khi dồn lưới
            double newX = X[start - 1][i] + transform_Middle_Vertical(xi, w, xi0, alpha);
            double deltaX = newX - X[j][i];
            // Cập nhật X và Y
            X[j][i] = newX;
            Y[j][i] -= tan_alpha * deltaX;
        }
    }
}

void express_Two_Boundry_Vertical_Block3()
{
    double alpha = 5.0; // Hệ số dồn lưới
    double xi0 = 0.5;   // Vị trí tập trung lưới
    int Nx = Nx1 + Nx2 + Nx3 - 2;
    int start = Nx1 + Nx2;
    for (int i = 1; i <= Ny; i++)
    { // Duyệt từng hàng
        for (int j = start; j <= Nx; j++)
        { // Duyệt từng cột
            double xi = (double)(j - (start - 1)) / (Nx3 - 1);
            double w = X[Nx][i] - X[start - 1][i];
            double tan_alpha = (Y[start - 1][i] - Y[Nx][i]) / (X[Nx][i] - X[start - 1][i]);

            // Tính toán vị trí mới sau khi dồn lưới
            double newX = X[start - 1][i] + transform_Two_Boundary_Vertical(xi, w, xi0, alpha);
            double deltaX = newX - X[j][i];
            // Cập nhật X và Y
            X[j][i] = newX;
            Y[j][i] -= tan_alpha * deltaX;
        }
    }
}

void express_middle_Vertical_Block4()
{
    double alpha = 5.0; // Hệ số dồn lưới
    double xi0 = 0.5;   // Vị trí tập trung lưới
    int Nx = Nx1 + Nx2 + Nx3 + Nx4 - 3;
    int start = Nx1 + Nx2 + Nx3 - 1;
    for (int i = 1; i <= Ny; i++)
    { // Duyệt từng hàng
        for (int j = start; j <= Nx; j++)
        { // Duyệt từng cột
            double xi = (double)(j - (start - 1)) / (Nx4 - 1);
            double w = X[Nx][i] - X[start - 1][i];
            X[j][i] = X[start - 1][i] + transform_Middle_Vertical(xi, w, xi0, alpha);
            Y[j][i] = Y[j][i];
        }
    }
}

void export_data()
{
    ofstream outfile("data9.dat");
    if (!outfile)
    {
        cerr << "Lỗi: Không thể mở file để ghi!" << endl;
        return;
    }
    outfile << "VARIABLES = X Y" << endl;
    outfile << "ZONE I = " << Nx1 + Nx2 + Nx3 + Nx4 - 3 << ", J = " << Ny << ", F = POINT" << endl;

    for (int j = 1; j <= Ny; j++)
    {
        for (int i = 1; i <= Nx1 + Nx2 + Nx3 + Nx4 - 3; i++)
        {
            outfile << X[i][j] << '\t' << Y[i][j] << endl;
        }
    }
    outfile.close();
    cout << "Dữ liệu đã được ghi vào 'data9dat' thành công." << endl;
}

void express_bottom() // đang nén bên dưới
{
    double p = 1.0;
    // nếu p = 1 thì lưới ko nén d_y = eta
    // nếu p > 1 thì p * eta tăng lên đột ngột phần (1 - tanh(q * (1 - eta)) / tanh(q)) bị nhỏ lại gây ra bị d_y rất lớn làm rãn lưới 
    // nếu p càng về 0 thì nén đáy càng mạnh
    double q = 4;
    // nếu q tiền về 0 thì hàm gần như mất chức năng nén
    double d_eta = (double)1 / (Ny - 1);
    for (int i = 1; i <= Nx1 + Nx3 + Nx3 + Nx4 - 3; i++)
    {
        for (int j = 2; j <= Ny - 1; j++)
        {
            double eta = (double)d_eta * (j - 1);
            double d_y = p * eta + (1 - p) * (double)(1 - tanh(q * (1 - eta)) / tanh(q));

            X[i][j] = (X[i][Ny] - X[i][1]) * d_y + X[i][1];
            Y[i][j] = (Y[i][Ny] - Y[i][1]) * d_y + Y[i][1];
        }
    }
    //     eta là gì?

    // eta là một giá trị chuẩn hóa từ 0 đến 1 dọc theo trục Y của lưới, với d_eta = 1 / (Ny - 1).

    // eta = d_eta * (j - 1) nghĩa là eta = 0 ở j = 1 (đáy lưới), và eta = 1 ở j = Ny (đỉnh lưới).

    // Công thức d_y để làm gì?

    // d_y là một hàm ánh xạ từ eta sang một hệ tọa độ bị biến đổi.

    // Thành phần tuyến tính: p * eta giúp kiểm soát mức độ giữ nguyên của phân bố ban đầu.

    // Thành phần tanh: Giúp làm mịn sự biến đổi, nén mạnh phần phía dưới và giãn phần phía trên.

    // Công thức X[i][j] và Y[i][j] để làm gì?

    // Biến đổi giá trị tọa độ X và Y dựa trên d_y, nghĩa là nó sẽ kéo dãn hoặc nén các điểm theo trục Y.

    // Ở j = 1 (đáy), d_y = 0 nên X[i][j] = X[i][1], Y[i][j] = Y[i][1].

    // Ở j = Ny (đỉnh), d_y = 1 nên X[i][j] = X[i][Ny], Y[i][j] = Y[i][Ny].
}

int main()
{
    boundary_divide();
    express_grid_Two_Boundary_Horizontal();
    // express_bottom();
    export_data();
    return 0;
}