/* ================= main ================= */
int main() {
    cout << "=== LinkedGraph ===\n";

    LinkedGraph lg(6); // 頂點 5 是孤立點
    lg.insertEdge(0, 1);
    lg.insertEdge(0, 2);
    lg.insertEdge(1, 3);
    lg.insertEdge(3, 4);

    lg.display();
    lg.DFS(0);
    lg.BFS(0);
    lg.ConnectedComponents();

    cout << "\n=== WeightedGraph ===\n";

    WeightedGraph wg(4);
    wg.insertEdge(0, 1, 10);
    wg.insertEdge(0, 2, 6);
    wg.insertEdge(0, 3, 5);
    wg.insertEdge(1, 3, 15);
    wg.insertEdge(2, 3, 4);

    wg.display();
    cout << endl;

    wg.Prim(0);
    cout << endl;

    wg.Kruskal();
    cout << endl;

    wg.Dijkstra(0);

    return 0;
}
