#include "sprite_job.hpp"

#include <vector>
#include <iostream>
#include <algorithm>
#include <node.h>
#include <node_buffer.h>

#include "CImg.h"
#include "imagebox.hpp"
#include "layout.hpp"

using namespace cimg_library;
using namespace std;
using namespace v8;

SpriteJob::SpriteJob(const vector<string>& inputs_, const string& output_, const Persistent<Function>& callback_)
:inputs(inputs_)
,output(output_)
,callback(callback_) {
    request.data = this;
};



void write_css_class(const string& sprite_image, const string& className, const Box& position, ostream& output) {
    output << "." + className << " {" << endl;
    output << "\twidth  : " << position.h << "px;" << endl;
    output << "\theight : " << position.w << "px;" << endl;
    output << "\tmargin : 0px;" << endl;
    output << "\tpadding : 0px;" << endl;
    output << "\tbackground-image: url(" <<  sprite_image << ");" << endl;
    output << "\tbackground-position: " <<  -position.x << "px " << -position.y << "px;" <<  endl;
    output << "}" << endl;
}

void SpriteJob::run() {

    vector<ImageBox> images;
    vector<string>::const_iterator input_it;
    for (input_it = inputs.begin(); input_it != inputs.end(); ++input_it) {
        images.push_back(ImageBox::load(*input_it));
    }
    sort(images.begin(), images.end(), ImageBox::Comparator());
    vector<Box> boxes;
    vector<ImageBox>::const_iterator image_it;
    for (image_it=images.begin(); image_it!=images.end(); ++image_it) {
        Box box = image_it->box();
        boxes.push_back(box);
    }
    Layout layout = Layout::optimize(boxes);
    Image output_img(layout.W(), layout.H(), 1, 4);
    output_img.fill(0);
    output_img.get_shared_channel(3).fill(255);

    const vector<Box>& positions = layout.positions();

    // create sprite image
    for (size_t i=0; i<images.size(); ++i) {
        const ImageBox& image = images[i];
        const Box& position = positions[i];
        const Image& img = image.image();
        output_img.draw_image(position.x, position.y, img);
    }

    // create css
    image_it = images.begin();
    vector<Box>::const_iterator pos_it = positions.begin();
    for (; image_it!=images.end() && pos_it!=positions.end(); image_it++, pos_it++) {
        string image_name = image_it->filepath(); 
        imageToBox[image_name] = *pos_it;
    }

    output_img.save(output.c_str());
}

