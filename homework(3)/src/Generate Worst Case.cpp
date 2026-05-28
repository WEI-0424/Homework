vector<int> generateWorstCase(int n) {

    vector<int> a(n);

    for (int i = 0; i < n; i++) {
        a[i] = n - i;
    }

    return a;
