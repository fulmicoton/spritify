#ifndef BOX_HPP
#define BOX_HPP

struct Box {
    Box(int x, int y, int w, int h);
    Box(int w, int h);
    int x;
    int y;
    int w;
    int h;
};

#endif // BOX_HPP

