// Ví dụ (đặt các định nghĩa cần thiết vào đây):
struct EdgeInfo {
    int u_ascii, v_ascii, weight;
};

bool compareEdge(const EdgeInfo& a, const EdgeInfo& b) {
    if (a.u_ascii != b.u_ascii) {
        return a.u_ascii < b.u_ascii;
    }
    return a.v_ascii < b.v_ascii;
}

int getIndex(int ascii_val, const std::vector<int>& sorted_vertices) {
    auto it = std::lower_bound(sorted_vertices.begin(), sorted_vertices.end(), ascii_val);
    if (it != sorted_vertices.end() && *it == ascii_val) { // Quan trọng: kiểm tra có tìm thấy chính xác không
        return std::distance(sorted_vertices.begin(), it);
    }
    return -1; // Trả về -1 nếu không tìm thấy
}

// Hàm BF (phiên bản bạn đã xác nhận là "hoàn toàn đúng")
void BF(int edgeList[][3], int numEdges, char start_vertex_char, int BFValue[], int BFPrev[]) {
    std::vector<int> vertices;
    if (numEdges > 0 || start_vertex_char != '\0') {
        vertices.reserve(numEdges * 2 + (start_vertex_char != '\0' ? 1 : 0));
    }

    for (int i = 0; i < numEdges; ++i) {
        vertices.push_back(edgeList[i][0]);
        vertices.push_back(edgeList[i][1]);
    }

    bool is_start_char_actually_in_edge_list = false;
    int start_ascii_val = 0;

    if (start_vertex_char != '\0') {
        start_ascii_val = static_cast<int>(start_vertex_char);
        for (int i = 0; i < numEdges; ++i) {
            if (edgeList[i][0] == start_ascii_val || edgeList[i][1] == start_ascii_val) {
                is_start_char_actually_in_edge_list = true;
                break;
            }
        }
        vertices.push_back(start_ascii_val);
    }

    if (!vertices.empty()) {
        std::sort(vertices.begin(), vertices.end());
        vertices.erase(std::unique(vertices.begin(), vertices.end()), vertices.end());
    }

    // Xử lý quy ước đặc biệt cho BFPrev nếu start_vertex_char không có trong edgeList
    if (start_vertex_char != '\0' && !is_start_char_actually_in_edge_list && numEdges > 0) {
        const int ASSUMED_ESIZE_FOR_NON_EXISTENT_START_OUTPUT = 20;
        // Giả sử BFPrev được truyền vào có kích thước ít nhất là ASSUMED_ESIZE.
        // Và main.cpp đã khởi tạo BFValue và các BFPrev khác là -1.
        if (ASSUMED_ESIZE_FOR_NON_EXISTENT_START_OUTPUT > 0) {
           // Đảm bảo không ghi ra ngoài nếu BFPrev thực tế nhỏ hơn
           // (Điều này sẽ được xử lý bởi người gọi BF_Path nếu Esize khác nhau)
           // BFPrev[ASSUMED_ESIZE_FOR_NON_EXISTENT_START_OUTPUT - 1] = 0;
           // Trong ngữ cảnh BF_Path, chúng ta không biết Esize của main, nên không đặt BFPrev[cuối]=0 ở đây.
           // BF_Path sẽ trả về "" nếu is_start_char_actually_in_edge_list là false.
        }
        // Nếu start không có trong edgeList, BF sẽ không làm gì thêm để BF_Path có thể trả về ""
        return;
    }
    if (vertices.empty() && start_vertex_char == '\0') { return; }
    if (start_vertex_char == '\0') {
        bool zero_vertex_exists = false;
        for (size_t i = 0; i < vertices.size(); ++i) { if (vertices[i] == 0) { zero_vertex_exists = true; break; } }
        if (!zero_vertex_exists) {
            // Tương tự, nếu start là '\0' và không có đỉnh 0.
            // const int ASSUMED_ESIZE_FOR_NON_EXISTENT_START_OUTPUT = 20;
            // if (ASSUMED_ESIZE_FOR_NON_EXISTENT_START_OUTPUT > 0) {
            //    BFPrev[ASSUMED_ESIZE_FOR_NON_EXISTENT_START_OUTPUT - 1] = 0;
            // }
            return;
        }
    }


    int n_unique_vertices = vertices.size();
    int start_idx = (start_vertex_char != '\0') ? getIndex(start_ascii_val, vertices) : getIndex(0, vertices);

    if (start_idx == -1 || start_idx >= n_unique_vertices) { return; }


    if (BFValue[start_idx] == -1) { BFValue[start_idx] = 0; }

    std::vector<int> prev_BFValue(n_unique_vertices);
    for (int i = 0; i < n_unique_vertices; ++i) { prev_BFValue[i] = BFValue[i]; }

    std::vector<EdgeInfo> processed_edges;
    if (numEdges > 0) {
        processed_edges.reserve(numEdges);
        for (int i = 0; i < numEdges; ++i) {
            processed_edges.push_back({edgeList[i][0], edgeList[i][1], edgeList[i][2]});
        }
        std::sort(processed_edges.begin(), processed_edges.end(), compareEdge);
    }

    for (const auto& edge : processed_edges) {
        int u_idx = getIndex(edge.u_ascii, vertices);
        int v_idx = getIndex(edge.v_ascii, vertices);

        if (u_idx == -1 || v_idx == -1 || u_idx >= n_unique_vertices || v_idx >= n_unique_vertices) { continue; }

        if (prev_BFValue[u_idx] != -1) {
            long long candidate_dist = (long long)prev_BFValue[u_idx] + edge.weight;
            if (BFValue[v_idx] == -1 || candidate_dist < BFValue[v_idx]) {
                BFValue[v_idx] = candidate_dist;
                BFPrev[v_idx] = u_idx;
            }
        }
    }
}
// --- Kết thúc các hàm phụ trợ và BF ---


std::string BF_Path(int edgeList[][3], int numEdges,
                    char start_char_vertex, char goal_char_vertex) {

    // ---- BƯỚC 1: Xác định các đỉnh duy nhất của toàn bộ đồ thị ----
    std::vector<int> all_unique_vertices;
    if (numEdges > 0 || start_char_vertex != '\0' || goal_char_vertex != '\0') {
        all_unique_vertices.reserve(numEdges * 2 + 2); // +2 cho start và goal
    }

    for (int i = 0; i < numEdges; ++i) {
        all_unique_vertices.push_back(edgeList[i][0]);
        all_unique_vertices.push_back(edgeList[i][1]);
    }
    // Thêm start và goal vào để đảm bảo chúng có trong map, ngay cả khi là đỉnh cô lập
    // hoặc không có trong edgeList ban đầu.
    if (start_char_vertex != '\0') {
        all_unique_vertices.push_back(static_cast<int>(start_char_vertex));
    }
    if (goal_char_vertex != '\0') {
        all_unique_vertices.push_back(static_cast<int>(goal_char_vertex));
    }

    if (!all_unique_vertices.empty()) {
        std::sort(all_unique_vertices.begin(), all_unique_vertices.end());
        all_unique_vertices.erase(std::unique(all_unique_vertices.begin(), all_unique_vertices.end()),
                                 all_unique_vertices.end());
    }

    // Nếu sau khi thêm start/goal mà vẫn không có đỉnh nào (trường hợp rất đặc biệt)
    if (all_unique_vertices.empty()) {
        if (start_char_vertex != '\0' && start_char_vertex == goal_char_vertex) {
             std::string s = ""; s += start_char_vertex; return s;
        }
        return "";
    }

    int n_vertices = all_unique_vertices.size();

    int start_idx = getIndex(static_cast<int>(start_char_vertex), all_unique_vertices);
    int goal_idx = getIndex(static_cast<int>(goal_char_vertex), all_unique_vertices);

    // Nếu start hoặc goal không được map (ví dụ: là '\0' và không có đỉnh 0)
    if (start_idx == -1 || goal_idx == -1) {
        // Xử lý trường hợp start == goal và cả hai đều là '\0' (và không có đỉnh 0)
        if (start_char_vertex == '\0' && goal_char_vertex == '\0' && getIndex(0, all_unique_vertices) == -1) {
            // Không có cách biểu diễn đường đi cho đỉnh không tồn tại.
            return "";
        }
        // Nếu start == goal nhưng một trong hai không map được
        if (start_char_vertex == goal_char_vertex && start_idx != -1 && goal_idx != -1 ) {
            // đã xử lý ở dưới
        } else {
            return "";
        }
    }


    // ---- BƯỚC 1.5: Kiểm tra start_char_vertex có trong edgeList gốc không ----
    bool is_start_in_actual_edges = false;
    if (numEdges == 0) { // Nếu không có cạnh
        if (start_idx != -1) is_start_in_actual_edges = true; // Đỉnh nguồn là đỉnh cô lập hợp lệ
    } else {
        if (start_char_vertex != '\0') {
            int start_ascii = static_cast<int>(start_char_vertex);
            for (int i = 0; i < numEdges; ++i) {
                if (edgeList[i][0] == start_ascii || edgeList[i][1] == start_ascii) {
                    is_start_in_actual_edges = true;
                    break;
                }
            }
        } else { // start_char_vertex là '\0', kiểm tra đỉnh 0
            for (int i = 0; i < numEdges; ++i) {
                if (edgeList[i][0] == 0 || edgeList[i][1] == 0) {
                    is_start_in_actual_edges = true;
                    break;
                }
            }
        }
    }

    if (!is_start_in_actual_edges && start_char_vertex != '\0') {
        // Theo quy ước: nếu start không có trong edgeList, không có đường đi.
        // "Expected Output" cho ']' (khi nó không có trong edgeList) là toàn -1
        // cho BFValue và BFPrev[19]=0. BF_Path nên trả về chuỗi rỗng.
        // Phần đặt BFPrev[19]=0 sẽ được xử lý bởi hàm BF nếu ASSUMED_ESIZE được dùng.
        // BF_Path chỉ cần biết là không có đường đi.
        return "";
    }
     if (!is_start_in_actual_edges && start_char_vertex == '\0') {
        // Nếu start là '\0' và không có đỉnh 0 trong các cạnh (và có cạnh)
        if (numEdges > 0) { // Chỉ áp dụng nếu có cạnh để kiểm tra
            bool zero_in_edges = false;
            for(int i=0; i < numEdges; ++i) if(edgeList[i][0]==0 || edgeList[i][1]==0) zero_in_edges=true;
            if(!zero_in_edges) return "";
        } else if (start_idx == -1) { // Không có cạnh và start ('\0') không map được thành đỉnh 0
            return "";
        }
    }


    // ---- BƯỚC 2: Chạy đầy đủ thuật toán Bellman-Ford ----
    std::vector<int> current_BFValue(n_vertices, -1);
    std::vector<int> current_BFPrev(n_vertices, -1);

    current_BFValue[start_idx] = 0;

    for (int i = 0; i < n_vertices ; ++i) { // Chạy n_vertices lần để đảm bảo hội tụ (hoặc phát hiện chu trình âm ở lần thứ n)
                                           // Assignment không yêu cầu phát hiện chu trình âm, nên n-1 là đủ.
                                           // Tuy nhiên, hàm BF của bạn được gọi lặp lại.
                                           // Mỗi lần gọi BF, nó sẽ làm mới BFValue và BFPrev.
                                           // Để BF_Path hoạt động đúng, BFValue và BFPrev phải được tích lũy.
                                           // Do đó, BF_Path cần tự quản lý BFValue và BFPrev tích lũy.
                                           // Hàm BF gốc của bạn không phù hợp để gọi lặp lại trực tiếp kiểu này.
                                           // Nó phải được sửa để nhận và cập nhật BFValue/Prev được truyền vào.

        // *** ĐIỀU CHỈNH QUAN TRỌNG NẾU DÙNG LẠI BF GỐC CỦA BẠN ***
        // Hàm BF gốc của bạn có vẻ như được thiết kế để gọi 1 lần và nó tự khởi tạo start_idx.
        // Để chạy Bellman-Ford đầy đủ, chúng ta cần tự lặp và nới lỏng.
        // Phiên bản dưới đây sẽ mô phỏng lại logic nới lỏng của hàm BF gốc,
        // nhưng lặp lại đúng số lần.

        std::vector<int> temp_BFValue_for_this_iteration = current_BFValue; // Giống prev_BFValue trong BF của bạn

        std::vector<EdgeInfo> edges_for_iteration;
        if (numEdges > 0) {
            edges_for_iteration.reserve(numEdges);
            for (int k = 0; k < numEdges; ++k) {
                edges_for_iteration.push_back({edgeList[k][0], edgeList[k][1], edgeList[k][2]});
            }
            std::sort(edges_for_iteration.begin(), edges_for_iteration.end(), compareEdge);
        }

        bool changed_in_iteration = false;
        for (const auto& edge : edges_for_iteration) {
            int u_map_idx = getIndex(edge.u_ascii, all_unique_vertices);
            int v_map_idx = getIndex(edge.v_ascii, all_unique_vertices);

            if (u_map_idx == -1 || v_map_idx == -1) continue;

            if (temp_BFValue_for_this_iteration[u_map_idx] != -1) {
                long long candidate_dist = (long long)temp_BFValue_for_this_iteration[u_map_idx] + edge.weight;
                if (current_BFValue[v_map_idx] == -1 || candidate_dist < current_BFValue[v_map_idx]) {
                    current_BFValue[v_map_idx] = candidate_dist;
                    current_BFPrev[v_map_idx] = u_map_idx;
                    changed_in_iteration = true;
                }
                // Không có else if cho == theo logic BF gốc của bạn
            }
        }
        if (i < n_vertices -1 && !changed_in_iteration && numEdges > 0) { // Tối ưu: nếu không thay đổi ở iteration < n-1 thì dừng sớm
            break;
        }
    }


    // ---- BƯỚC 3: Truy vết đường đi ----
    if (current_BFValue[goal_idx] == -1) {
        return ""; // Không có đường đi
    }

    // Xử lý trường hợp start == goal
    if (start_idx == goal_idx) {
        std::string s = "";
        s += start_char_vertex;
        return s;
    }

    std::vector<int> path_indices_reversed;
    int current_path_node_idx = goal_idx;

    while (current_path_node_idx != -1 && current_path_node_idx != start_idx) {
        path_indices_reversed.push_back(current_path_node_idx);
        int prev_node = current_BFPrev[current_path_node_idx];
        
        // Kiểm tra để tránh vòng lặp vô hạn nếu BFPrev bị lỗi
        bool already_visited = false;
        for(size_t k=0; k < path_indices_reversed.size(); ++k) {
            if (path_indices_reversed[k] == prev_node && prev_node != -1) { // prev_node != -1 để tránh dừng ở start
                already_visited = true;
                break;
            }
        }
        if (already_visited) return ""; // Lặp lại đỉnh, lỗi truy vết

        current_path_node_idx = prev_node;
        if (path_indices_reversed.size() > (size_t)n_vertices + 1) {
            return ""; // Đường đi quá dài
        }
    }

    if (current_path_node_idx == start_idx) {
        path_indices_reversed.push_back(start_idx);
    } else { // Không truy vết về được start_idx
        return "";
    }

    // ---- BƯỚC 4: Chuyển đổi chỉ số thành ký tự và tạo output string ----
    std::string result_path_str = ""; // Khởi tạo chuỗi rỗng

    if (path_indices_reversed.empty() && !(start_idx == goal_idx && current_BFValue[goal_idx] == 0) ) {
        // Trường hợp này không nên xảy ra nếu logic truy vết đúng và có đường đi
        // (trừ khi start == goal và không có đường đi nào được "truy vết" nhưng giá trị là 0)
        // Nếu start == goal và current_BFValue[goal_idx] == 0, thì path_indices_reversed sẽ có 1 phần tử.
        return ""; // An toàn: trả về rỗng nếu không có chỉ số nào
    }
    
    // Đặc biệt xử lý trường hợp start == goal, vì vòng lặp dưới có thể không chạy nếu path_indices_reversed chỉ có 1 phần tử
    if (start_idx == goal_idx && current_BFValue[goal_idx] == 0) {
        if (!path_indices_reversed.empty() && path_indices_reversed.size() == 1 && path_indices_reversed[0] == start_idx) {
            result_path_str += static_cast<char>(all_unique_vertices[path_indices_reversed[0]]);
            return result_path_str;
        } else {
             return ""; // Lỗi logic nếu start==goal mà path không đúng
        }
    }


    for (int i = path_indices_reversed.size() - 1; i >= 0; --i) {
        result_path_str += static_cast<char>(all_unique_vertices[path_indices_reversed[i]]);
        if (i > 0) { // Thêm dấu cách nếu không phải là đỉnh cuối cùng
            result_path_str += " ";
        }
    }

    return result_path_str;
} // Kết thúc hàm BF_Path
