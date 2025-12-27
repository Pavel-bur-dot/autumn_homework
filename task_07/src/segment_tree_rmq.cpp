#include "segment_tree_rmq.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

SegmentTreeRMQ::SegmentTreeRMQ(const std::vector<int>& array) {
    n = array.size();
    arr = array;
    tree.resize(4 * n);
    build(1, 0, n - 1);
}

void SegmentTreeRMQ::build(int node, int left, int right) {
    if (left == right) {
        tree[node] = arr[left];
        return;
    }
    
    int mid = (left + right) / 2;
    build(node * 2, left, mid);
    build(node * 2 + 1, mid + 1, right);
    tree[node] = std::min(tree[node * 2], tree[node * 2 + 1]);
}

void SegmentTreeRMQ::update(int node, int left, int right, int idx, int value) {
    if (left == right) {
        tree[node] = value;
        arr[idx] = value;
        return;
    }
    
    int mid = (left + right) / 2;
    if (idx <= mid) {
        update(node * 2, left, mid, idx, value);
    } else {
        update(node * 2 + 1, mid + 1, right, idx, value);
    }
    tree[node] = std::min(tree[node * 2], tree[node * 2 + 1]);
}

int SegmentTreeRMQ::query(int node, int left, int right, int ql, int qr) {
    if (ql > right || qr < left) {
        return std::numeric_limits<int>::max();
    }
    
    if (ql <= left && right <= qr) {
        return tree[node];
    }
    
    int mid = (left + right) / 2;
    int left_min = query(node * 2, left, mid, ql, qr);
    int right_min = query(node * 2 + 1, mid + 1, right, ql, qr);
    return std::min(left_min, right_min);
}

void SegmentTreeRMQ::pointUpdate(int idx, int value) {
    update(1, 0, n - 1, idx - 1, value);
}

int SegmentTreeRMQ::rangeMinQuery(int l, int r) {
    return query(1, 0, n - 1, l - 1, r - 1);
}

void SegmentTreeRMQ::solveRMQ() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int N, Q;
    std::cin >> N >> Q;
    
    std::vector<int> arr(N);
    for (int i = 0; i < N; ++i) {
        std::cin >> arr[i];
    }
    
    SegmentTreeRMQ segtree(arr);
    
    for (int q = 0; q < Q; ++q) {
        int type;
        std::cin >> type;
        
        if (type == 1) {
            int l, r;
            std::cin >> l >> r;
            std::cout << segtree.rangeMinQuery(l, r) << "\n";
        } else if (type == 2) {
            int i, x;
            std::cin >> i >> x;
            segtree.pointUpdate(i, x);
        }
    }
}