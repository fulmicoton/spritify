#include "sprite_job.hpp"

#include <node.h>
#include <node_buffer.h>

using namespace v8;
using namespace node;
using namespace std;

static Handle<Value> build_sprite_async (const Arguments&);
static void build_sprite(uv_work_t *);
static void build_sprite_after(uv_work_t *);

extern "C" void init (Handle<Object>);

string toStdString(String& s) {
    int N = s.Length();
    char* c = new char[N+1];
    s.WriteAscii(c);
    c[N] = '\0';
    string res(c);
    delete[] c;
    return res;
}

static Handle<Value> build_sprite_async(const Arguments& args) {
    HandleScope scope;
    const char *usage = "usage: build_sprite( [ input_files ...] , output_file,  callback)";
    if (args.Length() != 3) {
        return ThrowException(Exception::Error(String::New(usage)));
    }

    Local<Array> inputs_arglist = Array::Cast(*args[0]);
    Local<String> output_arg = String::Cast(*args[1]);
    Local<Function> callback = Local<Function>::Cast(args[2]);

    int N = inputs_arglist->Length();
    vector<string> inputs;
    
    for(int i = 0; i < N; i++) {
        Local<String> val = inputs_arglist->Get(i)->ToString();
        string input = toStdString(**val);
        inputs.push_back(input);
    }
    
    Persistent<Function> callback_persistent = Persistent<Function>::New(callback);
    string output = toStdString(**output_arg);
    SpriteJob* sprite_job = new SpriteJob(inputs, output, callback_persistent);
    
    uv_queue_work(uv_default_loop(), &(sprite_job->request), build_sprite, build_sprite_after);
    return Undefined();
}

static void build_sprite(uv_work_t* req) {
    static_cast<SpriteJob*>(req->data)->run();
}

template <typename T>
void putKV(Local<Object>& dest, const string& key, const T& val) {
    dest->Set(String::NewSymbol(key.c_str()), val);
}

void putString(Local<Object>& dest, const string& key, const string& val) {
    v8::Local<v8::String> nodeString = String::New(val.c_str());
    putKV(dest, key, nodeString);
}

void putNumber(Local<Object>& dest, const string& key, double val) {
    v8::Local<v8::Number> nodeVal = Number::New(val);
    putKV(dest, key, nodeVal);
}

Local<Object> boxToObject(const Box& box) {
    Local<Object> boxObject = Object::New();
    putNumber(boxObject, "x", box.x);
    putNumber(boxObject, "y", box.y);
    putNumber(boxObject, "h", box.h);
    putNumber(boxObject, "w", box.w);
    return boxObject;
}

static void build_sprite_after(uv_work_t* req) {
    HandleScope scope;
    SpriteJob* sprite_job  = static_cast<SpriteJob*>(req->data);

    Local<Object> sprite_info = Object::New();
    putString(sprite_info, "sprite-image", sprite_job->output);
 
    Local<Object> boxes = Object::New();
    map<string, Box>::const_iterator box_it;
    const std::map<std::string, Box>& layout = sprite_job->imageToBox;
    for (box_it=layout.begin(); box_it!=layout.end(); ++box_it) {
        Local<Object> boxObject = boxToObject(box_it->second);
        putKV(boxes, box_it->first, boxObject);    
    }
    putKV(sprite_info, "layout", boxes);
    
    Local<Value> argv[1] = { sprite_info };
    sprite_job->callback->Call(Context::GetCurrent()->Global(), 1, argv);
    sprite_job->callback.Dispose();
    delete sprite_job;
}

extern "C" void init (Handle<Object> target) {
    HandleScope scope;
    NODE_SET_METHOD(target, "spritify", build_sprite_async);
}
