// Partition Function
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
// Quick Sort Recursive
void quickSort(vector<int>& a, int left, int right) {

    if (left + 10 <= right) {

        int pivotIndex = partition(a, left, right);

        quickSort(a, left, pivotIndex - 1);
        quickSort(a, pivotIndex + 1, right);
    }
    else {

        // 小區間改用 Insertion Sort
        for (int i = left + 1; i <= right; i++) {

            int key = a[i];
            int j = i - 1;

            while (j >= left && a[j] > key) {
                a[j + 1] = a[j];
                j--;
            }

            a[j + 1] = key;
        }
    }
}

