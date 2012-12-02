#include "layout.hpp"

#include <algorithm>

using namespace std;

Layout::Layout(int W, int H)
: _W(W),_H(H),_frontier(W, 0) {}

bool Layout::put_all(const vector<Box>& boxes) {
    vector<Box>::const_iterator box_it;
    for (box_it = boxes.begin();
         box_it!= boxes.end();
         ++box_it) {
        if (!put(*box_it)) {
            return false;
        }
    }
    return true;
}

bool Layout::put(const Box& box) {
    const int put_offset = optimal_place(box.w);
    if (put_offset == -1) {
        return false;
    }
    const int min_depth = _frontier[put_offset];
    const int total_depth = min_depth + box.h;
    
    if (total_depth <= H()) {
        const int first_column = put_offset;
        const int last_column = put_offset+box.w;
        for (int c = first_column; c<last_column; c++) {
            _frontier[c] = total_depth;
        }
        _positions.push_back(Box(first_column, min_depth, box.w, box.h));
        return true;
    }
    else {
        return false;
    }
}

int Layout::optimal_place(int w) {
    int min_offset = -1;
    int min_depth = H();
    for (int c=0; c<W()-w+1; c++) {
        int cur_depth = 0;
        for (int offset=0; offset<w; offset++) {
            const int depth = _frontier[c+offset];
            cur_depth=max(depth, cur_depth);
        }
        if (cur_depth < min_depth) {
            min_offset = c;
            min_depth = cur_depth;
        }
    }
    return min_offset;
}

Layout Layout::optimize(const vector<Box>& boxes) {
    if (boxes.size() == 0) {
        return Layout(0,0);
    }
    int H = boxes.begin()->h;
    int W = 0;
    int max_w = 0;
    vector<Box>::const_iterator box_it;
    for (box_it=boxes.begin(); box_it!=boxes.end(); ++box_it) {
        W += box_it->w;
        max_w = max(box_it->w, max_w);
    }

    int MIN_COST = W*H;
    Layout best_layout(W,H);
    best_layout.put_all(boxes);

    while (W>=max_w) {
        H+=1;
        W = (MIN_COST-1) / H;
        for (; W>=max_w ; W--)  {
            Layout cur_layout(W,H);
            if (cur_layout.put_all(boxes)) {
                best_layout.swap(cur_layout);
                MIN_COST = W*H;
            }
            else {
                break;
            }
        }       
    }
    return best_layout;
}

void Layout::swap(Layout& other) {
    std::swap(_W, other._W);
    std::swap(_H, other._H);
    _frontier.swap(other._frontier);
    _positions.swap(other._positions);
}
