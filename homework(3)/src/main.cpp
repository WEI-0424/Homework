int main() {

    srand(time(0));

    vector<int> testSize = {
        500,
        1000,
        2000,
        3000,
        4000,
        5000
    };



    for (int n : testSize) {

        cout << "n = " << n << endl;

        vector<int> data = generateWorstCase(n);

        // Heap / Quick 使用 random permutation
        shuffle(data.begin(),
                data.end(),
                default_random_engine(time(0)));

        // Insertion Sort
        double tInsertion =
            measureTime(data, insertionSort);

        // Quick Sort
        double tQuick =
            measureTime(data, [](vector<int>& a) {
                quickSort(a, 0, a.size() - 1);
            });

        // Merge Sort
        double tMerge =
            measureTime(data, mergeSortWrapper);

        // Heap Sort
        double tHeap =
            measureTime(data, heapSort);

        cout << "Insertion Sort : "
             << tInsertion << " ms\n";

        cout << "Quick Sort     : "
             << tQuick << " ms\n";

        cout << "Merge Sort     : "
             << tMerge << " ms\n";

        cout << "Heap Sort      : "
             << tHeap << " ms\n";

        cout << "-----------------------------\n";
    }

    return 0;
}
