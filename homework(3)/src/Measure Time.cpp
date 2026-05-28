template <typename SortFunc>
double measureTime(vector<int> a, SortFunc sortFunc) {

    auto start = high_resolution_clock::now();

    sortFunc(a);

    auto end = high_resolution_clock::now();

    duration<double, milli> elapsed = end - start;

    return elapsed.count();
}
