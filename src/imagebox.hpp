#ifndef IMAGEBOX_HPP
#define IMAGEBOX_HPP

#include "box.hpp"
#include <string>

namespace cimg_library {
    template <typename T> struct CImg;
}

typedef cimg_library::CImg<unsigned char> Image;

class ImageBox {

public:
    static ImageBox load(const std::string& filepath);
    ImageBox(const ImageBox& orig);
    ~ImageBox();
    Box box() const;
    const Image& image() const;
    const std::string& name() const;

private:
    ImageBox();
    ImageBox(const Image* img, const std::string& name);
    
    std::string _name;
    const Image* _img;
    int* _ref_count;

public:
    class Comparator {
    public:
        bool operator()(const ImageBox& left, const ImageBox& right) {
            return left.box().h > right.box().h;
        }
    };
};


#endif // IMAGEBOX_HPP
