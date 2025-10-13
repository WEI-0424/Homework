#include <iostream>
using namespace std;

int Ackermann(int m, int n) {
    if (m == 0)
        return n + 1;
    else if (n == 0)
        return Ackermann(m - 1, 1);
    else
        return Ackermann(m - 1, Ackermann(m, n - 1));
}

int main() {
    int m, n;
    cout << "請輸入 m 與 n ：";
    while (cin >> m >> n) {
        cout << "A(" << m << ", " << n << ") = " << Ackermann(m, n) << endl;
        cout << "請輸入 m 與 n ：";
    }
    return 0;
}
