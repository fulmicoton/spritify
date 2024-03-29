#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <node.h>
#include <node_buffer.h>
#include <vector>
#include <string>
#include <map>

#include "box.hpp"

struct SpriteJob {
    SpriteJob(
        const std::vector<std::string>& inputs_
       ,const std::string& output_
       ,const v8::Persistent<v8::Function>& callback_);
    uv_work_t request;
    std::vector<std::string> inputs;
    std::string output;
    v8::Persistent<v8::Function> callback;
    std::map<std::string, Box> imageToBox;
    void run();
};

void spritify(const SpriteJob& sprite_arguments);

#endif // SPRITE_HPP
