#include "box.hpp"

Box::Box(int x_, int y_, int w_, int h_)
:x(x_)
,y(y_)
,w(w_)
,h(h_) {}

Box::Box(int w_, int h_) 
:x(0)
,y(0)
,w(w_)
,h(h_) {}
