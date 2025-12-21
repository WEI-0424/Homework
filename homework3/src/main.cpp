// main.cpp
int main() {
    Polynomial A, B;
    double x;

    cin >> A >> B >> x;

    cout << "A = " << A << "\n";
    cout << "B = " << B << "\n";
    cout << "A + B = " << (A + B) << "\n";
    cout << "A - B = " << (A - B) << "\n";
    cout << "A * B = " << (A * B) << "\n";
    cout << "A(" << x << ") = " << A.eval(x) << "\n";
    return 0;
}
