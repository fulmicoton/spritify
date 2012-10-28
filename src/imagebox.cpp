#include "imagebox.hpp"

#include "CImg.h"

using namespace std;
using namespace cimg_library;

ImageBox::ImageBox(const Image* img, const string& name)
:_name(name)
,_img(img)
,_ref_count(new int(1)) {}

ImageBox::ImageBox(const ImageBox& orig)
:_name(orig._name)
,_img(orig._img)
,_ref_count(orig._ref_count) {
    (*_ref_count)++;
}

ImageBox::~ImageBox() {
    *_ref_count -= 1;
    if (*_ref_count == 0) {
        delete _ref_count;
        delete _img;
    }
}

string extract_filename(const string& name) {
    int offset = 0;
    size_t p = name.find_last_of('/');
    if (p != string::npos) {
        offset = p;
    }
    p = name.find_last_of('\\');
    if (p != string::npos) {
        offset = p;
    }
    return name.substr(offset+1, name.size()-offset-1);
}

string extract_name(const string& filepath) {
    string filename = extract_filename(filepath);
    size_t p = filename.find_last_of('.');
    if ((p == string::npos) || (p == 0))  {
        return filename;
    }
    else {
        return filename.substr(0, p);
    }
}

ImageBox ImageBox::load(const string& filepath) {
    string name = extract_name(filepath);
    return ImageBox(new Image(filepath.c_str()), name);
}

Box ImageBox::box() const {
    return Box(_img->width(), _img->height());
}

const Image& ImageBox::image() const {
    return *_img;
}

const string& ImageBox::name() const {
    return _name;
}