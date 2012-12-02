#include "box.hpp"

Box::Box()
:x(0)
,y(0)
,w(0)
,h(0) {}

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
