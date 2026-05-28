int partition(vector<int>& a, int left, int right) {

    // median-of-three
    int mid = (left + right) / 2;

    if (a[left] > a[mid])
        swap(a[left], a[mid]);

    if (a[left] > a[right])
        swap(a[left], a[right]);

    if (a[mid] > a[right])
        swap(a[mid], a[right]);

    swap(a[mid], a[right - 1]);

    int pivot = a[right - 1];

    int i = left;
    int j = right - 1;

    while (true) {

        while (a[++i] < pivot);

        while (a[--j] > pivot);

        if (i >= j)
            break;

        swap(a[i], a[j]);
    }

    swap(a[i], a[right - 1]);

    return i;
}
