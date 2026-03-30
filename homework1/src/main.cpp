int main() {
    MinHeap<int> h;

    h.Push(12);
    h.Push(6);
    h.Push(8);
    h.Push(7);
    h.Push(15);
    h.Push(3);

    cout << "Min Heap: ";
    h.PrintHeap();

    cout << "Top = " << h.Top() << endl;

    h.Pop();
    cout << "After Pop: ";
    h.PrintHeap();

    return 0;
}
