#include "simple_text_control.h"

#define TS TextServerManager::get_singleton()->get_primary_interface()

using namespace godot;

void SimpleTextControl::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("value_changed", "value"), &SimpleTextControl::value_changed);
}

void SimpleTextControl::add_paragraph() {
    auto font = get_theme_default_font();
    auto size = get_size();
    auto p = Paragraph();
    p.text = "This is very long line.\nAnd this is another very long line." + itos(paragraphs.size()) + "\n";
    p.tp.instantiate();
    p.tp->set_width(size.width - v_scroll->get_size().width);
    p.tp->set_break_flags(godot::TextServer::LineBreakFlag::BREAK_MANDATORY | godot::TextServer::LineBreakFlag::BREAK_GRAPHEME_BOUND);
    p.tp->add_string(p.text, font, 16);

    paragraphs.append(p);
    update_scrolls();
    queue_redraw();
}

void SimpleTextControl::update_scrolls() {
    auto size = get_size();
    auto vmin = v_scroll->get_combined_minimum_size();
    auto font = get_theme_default_font();
    int visible_rows = (size.height / font->get_height());
    int total_rows = 0;
    for (int p = 0; p < paragraphs.size(); ++p) {
        paragraphs[p].tp->set_width(size.width - v_scroll->get_size().width);
        total_rows += paragraphs[p].tp->get_line_count();
    }

	v_scroll->set_begin(Vector2(size.width - vmin.width, 0));
	v_scroll->set_end(Vector2(size.width, size.height));
    v_scroll->set_step(0.25);
    v_scroll->set_value(0);
    v_scroll->set_max(total_rows);
    v_scroll->set_page(visible_rows);
    v_scroll->show();
}

void SimpleTextControl::value_changed(float x) {
    queue_redraw();
}

void SimpleTextControl::copy() {
    Vector2i start = selection_start;
    Vector2i end = selection_end;

    if (start.x > selection_end.x) {
        end = selection_start;
        start = selection_end;
    }

    if (start.x == end.x && start.y > end.y) {
        int t = start.y;
        start.y = end.y;
        end.y = t;
    }

    String result = "";

    int total_line_counter = 0;
    for (int p = 0, cpy = 0; p <paragraphs.size(); ++p)
    {
        int plc = paragraphs[p].tp->get_line_count();
        for (int l = 0; l < plc; ++l, ++total_line_counter) {
            if (start.x <= total_line_counter && end.x >= total_line_counter) {
                auto paragraph = paragraphs[p];
                auto range = paragraph.tp->get_line_range(l);

                auto s = (start.x < total_line_counter ? range.x : start.y);
                auto e = (end.x > total_line_counter ? range.y : end.y);

                result += paragraph.text.substr(s, e - s);
            }
        }
    }

    DisplayServer::get_singleton()->clipboard_set(result);
}

Vector2i SimpleTextControl::get_row_column(Vector2i mpos) {
    auto font = get_theme_default_font();
    auto offset = (int)(((v_scroll->get_value() * font->get_height()) + mpos.y) / font->get_height());
    double margin = 5;    

    int selected_char = 0;
    for (int p = 0, lc = 0; p < paragraphs.size(); ++p) {
        int c = paragraphs[p].tp->get_line_count();
        lc += c;
        if (lc > offset) {
            int x = c - (lc - offset);
            selected_char = TS->shaped_text_hit_test_position(paragraphs[p].tp->get_line_rid(x), mpos.x - margin);
            break;
        }
    }

    
    int total_line_count = 0;
    for (int p = 0, lc = 0; p < paragraphs.size(); ++p) {
        total_line_count += paragraphs[p].tp->get_line_count();
    }

    return Vector2i(CLAMP(offset, 0, total_line_count - 1), selected_char);
}

Size2 SimpleTextControl::_get_minimum_size() const {
	return Size2(200, 60);
}

void SimpleTextControl::_unhandled_key_input(const Ref<InputEvent> &p_event) {
}

void SimpleTextControl::_gui_input(const Ref<InputEvent> &p_event) {

    Ref<InputEventMouseButton> mb = p_event;
    if (mb.is_valid()) {
        if (mb->is_pressed()) {
            			
            if (mb->get_button_index() == MouseButton::MOUSE_BUTTON_WHEEL_UP && !mb->is_command_or_control_pressed()) {
				if (mb->is_alt_pressed()) {
					v_scroll->set_value(v_scroll->get_value() - 15 * mb->get_factor());	
				} else if (v_scroll->is_visible()) {
					v_scroll->set_value(v_scroll->get_value() - 3 * mb->get_factor());
				}
			}

			if (mb->get_button_index() == MouseButton::MOUSE_BUTTON_WHEEL_DOWN && !mb->is_command_or_control_pressed()) {
				if (mb->is_alt_pressed()) {
					v_scroll->set_value(v_scroll->get_value() + 15 * mb->get_factor());					
				} else if (v_scroll->is_visible()) {					
					v_scroll->set_value(v_scroll->get_value() + 3 * mb->get_factor());
				}
			}

            if (mb->get_button_index() == MouseButton::MOUSE_BUTTON_LEFT) {
                Vector2i mpos = mb->get_position();
                selection_start = get_row_column(mpos);
                selection_end = selection_start;
                queue_redraw();
            }

            if (mb->get_button_index() == MouseButton::MOUSE_BUTTON_MIDDLE) {
                add_paragraph();
            }

        }
    }

    Ref<InputEventMouseMotion> mm = p_event;
    if (mm.is_valid()) {
        
        if (mm->get_button_mask().has_flag(MouseButtonMask::MOUSE_BUTTON_MASK_LEFT)) {            
            Vector2i mpos = mm->get_position();
            selection_end = get_row_column(mpos);
            queue_redraw();
        }

    }

    Ref<InputEventKey> k = p_event;
    if (k.is_valid()) {
        
        if (!k->is_pressed()) {
			return;
		}

        if (k->is_action("ui_copy", true)) {
            copy();
            accept_event();
            return;
        }

        if (k->is_action("ui_text_select_all", true)) {
            int total_line_count = 0;
            int total_last_char = 0;
            for (int p = 0; p < paragraphs.size(); ++p)
            {
                int plc = paragraphs[p].tp->get_line_count();
                total_line_count += plc;

                if (p == paragraphs.size() - 1) {
                    total_last_char = paragraphs[p].tp->get_line_range(plc - 1).y;
                }
            }

            selection_start.x = 0;
            selection_start.y = 0;
            selection_end.x = total_line_count;
            selection_end.y = total_last_char;
            queue_redraw();
            accept_event();
            return;
        }
    }
}

void SimpleTextControl::_notification(int p_what) {
	switch (p_what) {
        case NOTIFICATION_RESIZED: {
            update_scrolls();
        } break;

        case NOTIFICATION_READY: {
            v_scroll->connect("value_changed", Callable(this, "value_changed"));
            update_scrolls();
        } break;

        case NOTIFICATION_DRAW: {
            auto font = get_theme_default_font();
            auto size = get_size();

            RenderingServer::get_singleton()->canvas_item_set_clip(get_canvas_item(), true);
            RenderingServer::get_singleton()->canvas_item_add_rect(get_canvas_item(), Rect2(Point2i(), size), Color(0.1015625, 0.1015625, 0.1015625, 0.6));

            int visible_rows = (size.height / font->get_height()) + 1;
            int scroll_v = v_scroll->get_value();
            float m = v_scroll->get_value() - floor(v_scroll->get_value());
            float margin = 5;

            // Rendering selections background
            {
                Vector2i start = selection_start;
                Vector2i end = selection_end;

                if (start.x > selection_end.x) {
                    end = selection_start;
                    start = selection_end;
                }

                if (start.x == end.x && start.y > end.y) {
                    int t = start.y;
                    start.y = end.y;
                    end.y = t;
                }

                float offset_y = -(v_scroll->get_value() * font->get_height());
                for (int i = 0, x = 0; i < paragraphs.size(); ++i) {
                    int c = paragraphs[i].tp->get_line_count();
                    for (int j = 0; j < c; ++j, ++x) {

                        if (start.x <= x && end.x >= x) {
                            auto range = TS->shaped_text_get_range(paragraphs[i].tp->get_line_rid(j));

                            auto s = (start.x < x ? range.x : start.y);
                            auto e = (end.x > x ? range.y : end.y);

                            auto sel = TS->shaped_text_get_selection(paragraphs[i].tp->get_line_rid(j), s, e);
                            for (int z = 0; z < sel.size(); z++) {
                                Rect2 rect = Rect2(sel[z].x + margin, offset_y, sel[z].y - sel[z].x, font->get_height());
                                draw_rect(rect, Color(0.6, 0.6, 0.6, 0.6));
                            }
                        }

                        offset_y += TS->shaped_text_get_size(paragraphs[i].tp->get_line_rid(j)).y;
                    }
                }
            }

            // Rendering text lines
            {
                int p = 0, tlc = 0, lc = 0;
                for (p = 0; p < paragraphs.size(); ++p) {
                    lc = paragraphs[p].tp->get_line_count();
                    tlc += lc;
                    if (tlc > scroll_v) break;
                }

                int limit = MIN(p + visible_rows, paragraphs.size());
                float offset_y = -(float)(lc - (tlc - scroll_v)) * font->get_height() - (m * font->get_height());
                for (int i = p; i < limit; ++i) {
                    paragraphs[i].tp->draw(get_canvas_item(), Vector2(margin, offset_y));
                    offset_y += paragraphs[i].tp->get_size().height;
                }
            }

        } break;
    }
}

SimpleTextControl::SimpleTextControl() : paragraphs(), selection_start(0, 0), selection_end(0, 0) {
    set_focus_mode(FOCUS_ALL);
    
	v_scroll = memnew(VScrollBar);
    add_child(v_scroll, false, INTERNAL_MODE_FRONT);

    selection_start = Vector2i(0, 0);
    selection_end = Vector2i(0, 0);

    add_paragraph();
    add_paragraph();
    add_paragraph();
    add_paragraph();
    add_paragraph();
    add_paragraph();
    add_paragraph();
    add_paragraph();
    add_paragraph();
    add_paragraph();
}

SimpleTextControl::~SimpleTextControl() {
}