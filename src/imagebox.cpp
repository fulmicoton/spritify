#include "imagebox.hpp"

#include "CImg.h"

using namespace std;
using namespace cimg_library;

ImageBox::ImageBox(const Image* img, const string& filepath)
:_filepath(filepath)
,_img(img)
,_ref_count(new int(1)) {}

ImageBox::ImageBox(const ImageBox& orig)
:_filepath(orig._filepath)
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

ImageBox ImageBox::load(const string& filepath) {
    return ImageBox(new Image(filepath.c_str()), filepath);
}

Box ImageBox::box() const {
    return Box(_img->width(), _img->height());
}

const Image& ImageBox::image() const {
    return *_img;
}

const string& ImageBox::filepath() const {
    return _filepath;
}