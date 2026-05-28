// Merge Function
void merge(vector<int>& a, vector<int>& temp,
           int left, int mid, int right) {

    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {

        if (a[i] <= a[j])
            temp[k++] = a[i++];
        else
            temp[k++] = a[j++];
    }

    while (i <= mid)
        temp[k++] = a[i++];

    while (j <= right)
        temp[k++] = a[j++];

    for (int x = left; x <= right; x++)
        a[x] = temp[x];
}

// Merge Sort Recursive
void mergeSort(vector<int>& a, vector<int>& temp,
               int left, int right) {

    if (left < right) {

        int mid = (left + right) / 2;

        mergeSort(a, temp, left, mid);
        mergeSort(a, temp, mid + 1, right);

        merge(a, temp, left, mid, right);
    }
}

// Wrapper
void mergeSortWrapper(vector<int>& a) {

    vector<int> temp(a.size());

    mergeSort(a, temp, 0, a.size() - 1);
}
