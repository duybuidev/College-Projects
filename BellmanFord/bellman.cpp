struct EdgeInfo {
    int u_ascii, v_ascii, weight;
};

int getIndex(int ascii_val, const std::vector<int>& sorted_vertices) {
    return std::lower_bound(sorted_vertices.begin(), sorted_vertices.end(), ascii_val) - sorted_vertices.begin();
}

void BF(int edgeList[][3], int numEdges, char startChar, int BFValue[], int BFPrev[]) {
    std::vector<int> vertices;
    for (int i = 0; i < numEdges; ++i) {
        vertices.push_back(edgeList[i][0]);
        vertices.push_back(edgeList[i][1]);
    }

    int start_ascii = (startChar != '\0') ? (int)startChar : 0;
    bool startInGraph = false;

    for (int i = 0; i < numEdges; ++i)
        if (edgeList[i][0] == start_ascii || edgeList[i][1] == start_ascii)
            startInGraph = true;

    vertices.push_back(start_ascii);
    std::sort(vertices.begin(), vertices.end());
    vertices.erase(std::unique(vertices.begin(), vertices.end()), vertices.end());

    if (vertices.empty() || (!startInGraph && startChar != '\0' && numEdges > 0)) {
        BFPrev[19] = 0; // output marker for special case
        return;
    }

    if (startChar == '\0' && !std::binary_search(vertices.begin(), vertices.end(), 0)) {
        BFPrev[19] = 0;
        return;
    }

    int n = vertices.size();
    int startIdx = getIndex(start_ascii, vertices);

    if (BFValue[startIdx] == -1)
        BFValue[startIdx] = 0;

    std::vector<int> prevBF(n);
    for (int i = 0; i < n; ++i)
        prevBF[i] = BFValue[i];

    std::vector<EdgeInfo> edges;
    for (int i = 0; i < numEdges; ++i)
        edges.push_back({edgeList[i][0], edgeList[i][1], edgeList[i][2]});
    
    std::sort(edges.begin(), edges.end(), [](const EdgeInfo& a, const EdgeInfo& b) {
        return a.u_ascii != b.u_ascii ? a.u_ascii < b.u_ascii : a.v_ascii < b.v_ascii;
    });

    for (const auto& edge : edges) {
        int u = getIndex(edge.u_ascii, vertices);
        int v = getIndex(edge.v_ascii, vertices);
        if (prevBF[u] != -1) {
            int64_t cand = (int64_t)prevBF[u] + edge.weight;
            if (BFValue[v] == -1 || cand < BFValue[v]) {
                BFValue[v] = cand;
                BFPrev[v] = u;
            }
        }
    }
}
