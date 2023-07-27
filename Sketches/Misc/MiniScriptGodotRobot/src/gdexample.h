#ifndef GDEXAMPLE_H
#define GDEXAMPLE_H

#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include "MiniScript/SimpleString.h"
#include "MiniScript/UnicodeUtil.h"
#include "MiniScript/UnitTest.h"
#include "MiniScript/SimpleVector.h"
#include "MiniScript/List.h"
#include "MiniScript/Dictionary.h"
#include "MiniScript/MiniscriptParser.h"
#include "MiniScript/MiniscriptInterpreter.h"

using namespace MiniScript;

namespace godot {

class GDExample : public Sprite2D {
    GDCLASS(GDExample, Sprite2D)

private:
    godot::String code;
    double time_passed;
    double time_emit;
    Interpreter interp;
    void ConfigInterpreter(Interpreter &interp);
    static IntrinsicResult intrinsic_forward(Context *context, IntrinsicResult partialResult);
    static IntrinsicResult intrinsic_backward(Context *context, IntrinsicResult partialResult);
    static IntrinsicResult intrinsic_turnLeft(Context *context, IntrinsicResult partialResult);
    static IntrinsicResult intrinsic_turnRight(Context *context, IntrinsicResult partialResult);

protected:
    static void _bind_methods();
    static GDExample *instance;
    static Vector2 directions[];
    static double rotations[];

public:
    GDExample();
    ~GDExample();
    Vector2 position;
    int heading;
    void _process(double delta);
    void set_code(const godot::String p_code);
    godot::String get_code() const;
};

}

#endif