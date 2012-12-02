cpp_spritify = require("./build/Release/spritify_cpp")
fs = require "fs"
build_sprite = cpp_spritify.build_sprite
path = require "path"

IMAGE_PTN = /^([^.].*)\.[pP][nN][gG]$/

is_png_file = (filepath)->
    IMAGE_PTN.exec(filepath)?

chain_map = (operation, elements, callback)->
    res = []
    elements = elements.slice 0
    aux = ->
        if elements.length == 0
            callback res
        else
            el = elements.shift()
            operation el, (val)->
                res.push val
                aux()
    aux()

write_css = (output_png, output_css, layout, callback)->
    ostream = fs.createWriteStream output_css, { flags: 'w', encoding: "utf-8" }
    fs.open output_css, "w", null, (err, fd)->
        write_css_class = (sprite, cb)->
            css_class = """

            .#{sprite.name} {
                width: #{sprite.box.w}px;
                height: #{sprite.box.h}px;
                top: -#{sprite.box.y}px;
                left: -#{sprite.box.x}px;
                background-image: url(#{output_png});
                padding: 0;
                margin: 0;
            }
            
            """
            ostream.write css_class
            cb sprite
        chain_map write_css_class, layout, callback

filepath_to_name = (filepath, prefix)->
    prefix = prefix ? ""
    filename = path.basename filepath
    match =  IMAGE_PTN.exec(filename)
    if match?
        prefix + match[1]
    else
        undefined

transform_sprite_info = (sprite_info, prefix)->
    layout = sprite_info.layout
    sprites = []
    for image_path, box of layout
        sprite = {}
        sprite.box = box
        sprite.path = image_path
        sprite.name = filepath_to_name image_path,prefix
        sprites.push sprite
    sprite_info.layout = sprites

spritify_directory = (options, callback)->
    input = options.input
    input_name = path.basename(input)
    output_png = options.output_png ? input_name + ".png"
    output_css = options.output_css ? input_name + ".css"
    prefix = options.prefix ? "sprite-"
    fs.readdir input, (err, filepaths)->
        png_filepaths =  (path.join(input, filepath) for filepath in filepaths when is_png_file filepath)
        build_sprite png_filepaths, output_png, (sprite_info)->
            transform_sprite_info sprite_info, prefix
            output_css_dirname = path.dirname output_css
            rel_png_path = path.relative output_css_dirname,output_png
            write_css rel_png_path, output_css, sprite_info.layout, -> callback sprite_info

module.exports = 
    __build_sprite: cpp_spritify.build_sprite
    spritify_directory: spritify_directory