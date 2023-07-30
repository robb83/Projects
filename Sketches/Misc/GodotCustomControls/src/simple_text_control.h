#ifndef SIMPLE_TEXT_CONTROL_H
#define SIMPLE_TEXT_CONTROL_H

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/vector2i.hpp>
#include <godot_cpp/variant/color.hpp>
#include <godot_cpp/templates/vector.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/display_server.hpp>
#include <godot_cpp/classes/text_server.hpp>
#include <godot_cpp/classes/text_server_manager.hpp>
#include <godot_cpp/classes/text_paragraph.hpp>
#include <godot_cpp/classes/v_scroll_bar.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/font.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/input_event_mouse.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>

namespace godot {

class SimpleTextControl : public Control {
    GDCLASS(SimpleTextControl, Control)

private:

    struct Paragraph {
        String text;
        Ref<TextParagraph> tp;
    };

	VScrollBar *v_scroll = nullptr;
    Vector<Paragraph> paragraphs;
    Vector2i selection_start;
    Vector2i selection_end;
    void copy();
    void update_scrolls();
    void add_paragraph();
    Vector4i get_row_column(Vector2i pos);

protected:
    static void _bind_methods();
    void _notification(int p_what);

public:
    virtual void _unhandled_key_input(const Ref<InputEvent> &p_event) override;
	virtual void _gui_input(const Ref<InputEvent> &p_gui_input) override;
    virtual Size2 _get_minimum_size() const override;
    
    void value_changed(float x);

    SimpleTextControl();
    ~SimpleTextControl();
};

}

#endif