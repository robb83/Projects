#include <string>
#include <iostream>
#include <fstream>
#include "gdexample.h"

using namespace MiniScript;
using namespace godot;

GDExample* GDExample::instance;

Vector2 GDExample::directions[] = {
    Vector2(0, -1),
    Vector2(1, 0),
    Vector2(0, 1),
    Vector2(-1, 0)
};

double GDExample::rotations[] = {
    0,
    1.5708,
    3.14159,
    4.71239
};

static void PrintErr(MiniScript::String s, bool lineBreak=true) {
  std::ofstream output("output.txt");
  output << s.c_str();
  output.close();
}

void GDExample::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_code"), &GDExample::get_code);
    ClassDB::bind_method(D_METHOD("set_code", "p_code"), &GDExample::set_code);
    ClassDB::add_property("GDExample", PropertyInfo(Variant::STRING, "code"), "set_code", "get_code");
}

void GDExample::set_code(const godot::String p_code) {
    code = p_code;
    interp.Reset(MiniScript::String(code.ascii().get_data()));
    interp.Compile();
}

godot::String GDExample::get_code() const {
    return code;
}

GDExample::GDExample() : interp(), code("") {
    // Initialize any variables here.
    GDExample::instance = this;
    time_passed = 0.0;
    heading = 0;
    position = Vector2(0, 0);
    ConfigInterpreter(interp);
}

GDExample::~GDExample() {
    // Add your cleanup here.
}

IntrinsicResult GDExample::intrinsic_forward(Context *context, IntrinsicResult partialResult) {
    GDExample::instance->position += GDExample::directions[GDExample::instance->heading];
	return IntrinsicResult::Null;
}

IntrinsicResult GDExample::intrinsic_backward(Context *context, IntrinsicResult partialResult) {	
    GDExample::instance->position -= GDExample::directions[GDExample::instance->heading];
	return IntrinsicResult::Null;
}

IntrinsicResult GDExample::intrinsic_turnLeft(Context *context, IntrinsicResult partialResult) {
	GDExample::instance->heading = ((GDExample::instance->heading - 1) % 4 + 4) %4;
	return IntrinsicResult::Null;
}

IntrinsicResult GDExample::intrinsic_turnRight(Context *context, IntrinsicResult partialResult) {
	GDExample::instance->heading = ((GDExample::instance->heading + 1) % 4 + 4) %4;
	return IntrinsicResult::Null;
}

void GDExample::ConfigInterpreter(Interpreter &interp) {
	interp.standardOutput = &PrintErr;
	interp.errorOutput = &PrintErr;
	interp.implicitOutput = &PrintErr;

  	Intrinsic *f;
	
	f = Intrinsic::Create("forward");
	f->code = &intrinsic_forward;

	f = Intrinsic::Create("backward");
	f->code = &intrinsic_backward;

	f = Intrinsic::Create("turnLeft");
	f->code = &intrinsic_turnLeft;

	f = Intrinsic::Create("turnRight");
	f->code = &intrinsic_turnRight;
}

void GDExample::_process(double delta) {
    time_passed += delta;

    time_emit += delta;
    if (time_emit > 0.001) {
        if (!interp.Done()) {
            try {
                interp.Step();
            } catch (MiniscriptException& mse) {
                interp.vm->Stop();
            }
	    }

        time_emit = 0.0;
    }

    set_rotation(GDExample::rotations[heading]);
    set_position(position * 10);
}
