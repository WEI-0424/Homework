#include <iostream>
using namespace std;

int AckermannLoop(int m, int n) {
    const int STACK_SIZE = 100000;
    int stack[STACK_SIZE];
    int top = 0;

    stack[top++] = m;

    while (top > 0) {
        m = stack[--top];

        if (m == 0) {
            n += 1;
        }
        else if (n == 0) {
            stack[top++] = m - 1;
            n = 1;
        }
        else {
            stack[top++] = m - 1;
            stack[top++] = m;
            n -= 1;
        }
    }
    return n;
}

int main() {
    int m, n;
    cout << "請輸入 m 與 n ：";
    while (cin >> m >> n) {
        cout << "A(" << m << ", " << n << ") = " << AckermannLoop(m, n) << endl;
        cout << "請輸入 m 與 n ：";
    }
    return 0;
}
