#include "sprite_job.hpp"

#include <node.h>
#include <node_buffer.h>

using namespace v8;
using namespace node;
using namespace std;

static Handle<Value> build_sprite_async (const Arguments&);
static void build_sprite(eio_req *);
static int build_sprite_after(eio_req *);
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
    string output = toStdString(**output_arg);
    Persistent<Function> callback_persistent = Persistent<Function>::New(callback);
    SpriteJob* sprite_job = new SpriteJob(inputs, output, callback_persistent);
    eio_custom(build_sprite, EIO_PRI_DEFAULT, build_sprite_after, sprite_job);
    ev_ref(EV_DEFAULT_UC);
    return Undefined();
}

static void build_sprite(eio_req *req) {
    const SpriteJob* sprite_job = (struct SpriteJob*)req->data;
    sprite_job->run();
}

static int build_sprite_after(eio_req* req) {
    HandleScope scope;
    ev_unref(EV_DEFAULT_UC);
    struct SpriteJob* sprite_job  = (struct SpriteJob*)req->data;
    Local<Value> argv[0];

    TryCatch try_catch;
    sprite_job->callback->Call(Context::GetCurrent()->Global(), 0, argv);
    if (try_catch.HasCaught()) {
        FatalException(try_catch);
    }
    sprite_job->callback.Dispose();
    delete sprite_job;
    return 0;
}

extern "C" void init (Handle<Object> target) {
    HandleScope scope;
    NODE_SET_METHOD(target, "spritify", build_sprite_async);
}
