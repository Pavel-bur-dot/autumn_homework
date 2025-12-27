#ifndef SEGMENT_TREE_RMQ_HPP
#define SEGMENT_TREE_RMQ_HPP

#include <vector>

class SegmentTreeRMQ {
private:
    int n;
    std::vector<int> tree;
    std::vector<int> arr;
    
    void build(int node, int left, int right);
    void update(int node, int left, int right, int idx, int value);
    int query(int node, int left, int right, int ql, int qr);
    
public:
    SegmentTreeRMQ(const std::vector<int>& array);
    
    void pointUpdate(int idx, int value);
    
    int rangeMinQuery(int l, int r);
    
    static void solveRMQ();
};

#endif