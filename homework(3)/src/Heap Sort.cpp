// Heapify
void heapify(vector<int>& a, int n, int i) {

    int largest = i;

    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && a[left] > a[largest])
        largest = left;

    if (right < n && a[right] > a[largest])
        largest = right;

    if (largest != i) {

        swap(a[i], a[largest]);

        heapify(a, n, largest);
    }
}

// Heap Sort
void heapSort(vector<int>& a) {

    int n = a.size();

    // Build Heap
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(a, n, i);

    // Extract Elements
    for (int i = n - 1; i > 0; i--) {

        swap(a[0], a[i]);

        heapify(a, i, 0);
    }
}
