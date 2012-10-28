#ifndef LAYOUT_HPP
#define LAYOUT_HPP

#include <vector>
#include "imagebox.hpp"
#include "box.hpp"

class Layout {

public:
    Layout(int W, int H);
    bool put_all(const std::vector<Box>&);
    static Layout optimize(const std::vector<Box>&);
    int W() const;
    int H() const;
    const std::vector<Box>& positions() const;

private:
    int optimal_place(int w);
    bool put(const Box& box);
    void put_to_offset(const Box&, int offset);
    void swap(Layout& other);

    int _W;
    int _H;
    std::vector<int> _frontier;
    std::vector<Box> _positions;
};


inline int Layout::W() const { return _W; }
inline int Layout::H() const { return _H; }
inline const std::vector<Box>& Layout::positions() const {  return _positions;  }

#endif // LAYOUT_HPP
