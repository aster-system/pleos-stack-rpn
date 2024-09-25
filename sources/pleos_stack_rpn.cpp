//******************
//
// pleos_data_structure.cpp
//
//******************
// Presentation :
//
// PLEOS, by Aster System, is a project which aims education.
// By using the power of SCLS and other Aster System's tools, we created  this.
// We want to make education easier for everyone (teachers, as students and pupils).
// The software is made in french, because the main goal is France educational system.
// For more information, see : https://aster-system.github.io/aster-system/projects/pleos.html.
//
// The "Stack RPN" part aims IT lesson.
// Its goal is to explain how the data structure "Stack" works, with the Reversed Polish Notation exemple.
//
// This file contains the source code of "pleos_data_structure.h".
//
//******************
//
// License (GPL V3.0) :
//
// Copyright (C) 2024 by Aster System, Inc. <https://aster-system.github.io/aster-system/>
// This file is part of PLEOS.
// PLEOS is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// PLEOS is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with PLEOS. If not, see <https://www.gnu.org/licenses/>.
//

// Include SCLS Graphic Benoit
#include "../headers/pleos_stack_rpn.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {

    // Stack_RPN_Page constructor
    Stack_RPN_Page::Stack_RPN_Page(scls::_Window_Advanced_Struct* window_struct, std::string name) : scls::GUI_Page(window_struct, name) {
        parent_object()->set_background_color(scls::Color(39, 31, 58));
    }

    // Loads an object in a page from XML
    std::shared_ptr<scls::GUI_Object> Stack_RPN_Page::__create_loaded_object_from_type(std::string object_name, std::string object_type, scls::GUI_Object* parent) {
        if(object_name == "stack_rpn_animation_window") {
            a_stack_rpn_animation_window = *parent->new_object<scls::GUI_Object>(object_name);
            return a_stack_rpn_animation_window;
        }
        else if(object_name == "stack_rpn_input") {
            a_stack_rpn_input = *parent->new_object<scls::GUI_Text_Input>(object_name);
            return a_stack_rpn_input;
        }
        else if(object_name == "stack_rpn_next") {
            a_stack_rpn_next_button = *parent->new_object<scls::GUI_Object>(object_name);
            return a_stack_rpn_next_button;
        }
        else if(object_name == "stack_rpn_stack") {
            a_stack_rpn_stack = *parent->new_object<scls::GUI_Scroller>(object_name);
            return a_stack_rpn_stack;
        }

        // Classic object creation
        return GUI_Page::__create_loaded_object_from_type(object_name, object_type, parent);
    }

    // Analyse the needed next character
    void Stack_RPN_Page::analyse_next_character() {
        if(text_to_analyse().size() <= 0) {
            // Finish the analyse
            a_main_animation_title.get()->set_text(scls::to_utf_8_code_point("Analyse terminée."));
            if(text_analysed().size() > 1) {
                a_main_animation_explaination.get()->set_text(scls::to_utf_8_code_point("L'analyse est terminée.</br>Or, il reste plusieurs valeurs brute.</br>Le texte de départ était mal formaté."));
            }
            else {
                a_main_animation_explaination.get()->set_text(scls::to_utf_8_code_point("L'analyse est terminée.</br>La valeur est donc \"") + scls::format_number_to_text(text_analysed()[0]) + "\".");
            }
            a_current_state.current_animation_state = Animation_State::AS_Terminated;
            a_stack_rpn_next_button.get()->set_texture("stack_rpn_page_button_finish", false);
        }
        else {
            // Check if the character is a number
            try {
                double current_number = scls::string_to_double(text_to_analyse()[0]);
                a_main_animation_explaination.get()->set_text("On empile " + text_to_analyse()[0] + " dans la pile de droite.</br>" + scls::to_utf_8_code_point("On dépile ") + text_to_analyse()[0] + scls::to_utf_8_code_point(" dans la pile à analyser."));
                text_analysed().push_back(current_number);

                // Create the needed GUI
                std::shared_ptr<scls::GUI_Text> new_element = *a_stack_rpn_stack.get()->new_object<scls::GUI_Text>(a_stack_rpn_stack.get()->name() + "-element_" + std::to_string(analyse_number()));
                new_element.get()->set_font_color(scls::Color(255, 255, 255));
                new_element.get()->set_font_size(30);
                new_element.get()->set_height_in_pixel(30);
                new_element.get()->set_text(text_to_analyse()[0]);
                new_element.get()->set_width_in_scale(scls::Fraction(1));
                a_text_analysed_gui.push_back(new_element);
            } catch (std::invalid_argument) {
                if(a_avaible_operator.find(text_to_analyse()[0]) == a_avaible_operator.end()) {
                    // Unknown operator
                    a_main_animation_explaination.get()->set_text(scls::to_utf_8_code_point("Nous avons un opérateur ") + text_to_analyse()[0] + scls::to_utf_8_code_point(".</br>Or, cet opérateur n'est pas reconnu.</br>Le texte de départ était mal formaté.</br>"));

                    // Finish the analyse
                    a_current_state.current_animation_state = Animation_State::AS_Terminated;
                    a_stack_rpn_next_button.get()->set_texture("stack_rpn_page_button_finish", false);
                }
                else {
                    if(text_analysed().size() < a_avaible_operator.at(text_to_analyse()[0])) {
                        // Not enough operand
                        a_main_animation_explaination.get()->set_text(scls::to_utf_8_code_point("Nous avons un opérateur ") + text_to_analyse()[0] + scls::to_utf_8_code_point(".</br>Or, nous n'avons que ") + std::to_string(text_analysed().size()) + scls::to_utf_8_code_point(" disponible.</br>Le texte de départ était mal formaté.</br>"));

                        // Finish the analyse
                        a_current_state.current_animation_state = Animation_State::AS_Terminated;
                        a_stack_rpn_next_button.get()->set_texture("stack_rpn_page_button_finish", false);
                    }
                    else {
                        a_main_animation_explaination.get()->set_text(scls::to_utf_8_code_point("Nous avons un opérateur ") + text_to_analyse()[0] + scls::to_utf_8_code_point(".</br>On dépile les 2 opérandes précédents.</br>On effectue l'opération et on empile le résultat.</br>") + scls::to_utf_8_code_point("On dépile ") + text_to_analyse()[0] + scls::to_utf_8_code_point(" dans la pile à analyser."));

                        // Do the operation
                        std::string current_operator = text_to_analyse()[0];
                        double first_operand = text_analysed()[text_analysed().size() - 1]; if(text_analysed().size() > 1) text_analysed().erase(text_analysed().end() - 1);
                        double second_operand = text_analysed()[text_analysed().size() - 1];
                        double result = 0;
                        if(current_operator == "+") result = first_operand + second_operand;
                        else if(current_operator == "-") result = first_operand - second_operand;
                        else if(current_operator == "*") result = first_operand * second_operand;
                        else if(current_operator == "/") result = first_operand / second_operand;
                        else if(current_operator == "sqrt") result = std::sqrt(first_operand);
                        text_analysed()[text_analysed().size() - 1] = result;

                        // Create the needed GUI
                        for(int i = 0;i<static_cast<int>(a_avaible_operator.at(text_to_analyse()[0]));i++) {
                            a_stack_rpn_stack.get()->delete_child(a_text_analysed_gui[a_text_analysed_gui.size() - 1].get());
                            a_text_analysed_gui.erase(a_text_analysed_gui.end() - 1);
                        }
                        std::shared_ptr<scls::GUI_Text> new_element = *a_stack_rpn_stack.get()->new_object<scls::GUI_Text>(a_stack_rpn_stack.get()->name() + "-element_operator_" + std::to_string(analyse_number()));
                        new_element.get()->set_font_color(scls::Color(255, 255, 255));
                        new_element.get()->set_font_size(30);
                        new_element.get()->set_height_in_pixel(30);
                        new_element.get()->set_text(scls::format_number_to_text(result));
                        new_element.get()->set_width_in_scale(scls::Fraction(1));
                        a_text_analysed_gui.push_back(new_element);
                    }
                }
            }
            place_analysed_text();

            // Update the text
            a_main_animation_title.get()->set_text(scls::to_utf_8_code_point("Le prochain caractère est \"") + text_to_analyse()[0] + "\".");
            text_to_analyse().erase(text_to_analyse().begin());
            a_main_animation_text.get()->set_text(current_animation_text());
            a_current_state.analyse_number++;
        }
    }

    // Place the analysed text GUI
    void Stack_RPN_Page::place_analysed_text() {
        if(a_text_analysed_gui.size() > 0) {
            std::shared_ptr<scls::GUI_Text> last_button = a_text_analysed_gui[0];
            last_button.get()->attach_bottom_in_parent();
            for(int i = 1;i<static_cast<int>(a_text_analysed_gui.size());i++) {
                std::shared_ptr<scls::GUI_Text> current_button = a_text_analysed_gui[i];
                current_button.get()->attach_top_of_object_in_parent(last_button.get());
                last_button = current_button;
            }
            a_stack_rpn_stack.get()->check_scroller();
        }
    }

    // Setup the animation window
    void Stack_RPN_Page::setup_animation_window() {
        unload_animation_window();

        // Set the needed text
        std::string text = ""; a_current_state.text_to_analyse = scls::cut_string(input(), " ");

        // Create the main part of the window
        // Create the title of the main part
        a_main_animation_title = *a_stack_rpn_animation_window.get()->new_object<scls::GUI_Text>(a_stack_rpn_animation_window.get()->name() + "-animation_title");
        a_main_animation_title.get()->attach_top_in_parent();
        a_main_animation_title.get()->set_font_color(scls::Color(255, 255, 255));
        a_main_animation_title.get()->set_font_size(24);
        a_main_animation_title.get()->set_height_in_scale(scls::Fraction(1, 6));
        a_main_animation_title.get()->set_text("Nous allons analyser le texte :");
        a_main_animation_title.get()->set_width_in_scale(scls::Fraction(1));
        // Create the main part
        a_main_animation_text = *a_stack_rpn_animation_window.get()->new_object<scls::GUI_Text>(a_stack_rpn_animation_window.get()->name() + "-animation_text");
        a_main_animation_text.get()->attach_bottom_of_object_in_parent(a_main_animation_title.get());
        a_main_animation_text.get()->set_font_color(scls::Color(255, 255, 255));
        a_main_animation_text.get()->set_font_size(30);
        a_main_animation_text.get()->set_height_in_scale(scls::Fraction(1, 5));
        a_main_animation_text.get()->set_text(current_animation_text());
        a_main_animation_text.get()->set_width_in_scale(scls::Fraction(1));
        // Create the explaination part
        a_main_animation_explaination = *a_stack_rpn_animation_window.get()->new_object<scls::GUI_Text>(a_stack_rpn_animation_window.get()->name() + "-animation_explaination");
        a_main_animation_explaination.get()->attach_bottom_of_object_in_parent(a_main_animation_text.get());
        a_main_animation_explaination.get()->set_font_color(scls::Color(255, 255, 255));
        a_main_animation_explaination.get()->set_font_size(22);
        a_main_animation_explaination.get()->set_height_in_scale(scls::Fraction(1, 2));
        a_main_animation_explaination.get()->set_text(scls::to_utf_8_code_point("Pas de problèmes à signaler."));
        a_main_animation_explaination.get()->set_width_in_scale(scls::Fraction(1));

        // Change the next button texture
        a_stack_rpn_next_button.get()->set_texture("stack_rpn_page_button_next", false);

        // Change the needed datas
        a_current_state.current_animation_state = Animation_State::AS_Started;
    }

    // Update the events in the page
    void Stack_RPN_Page::update_event() {
        GUI_Page::update_event();

        if(a_stack_rpn_next_button.get()->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
            if(current_animation_state() == Animation_State::AS_Off && input() != "") setup_animation_window();
            else if(current_animation_state() == Animation_State::AS_Started) analyse_next_character();
            else if(current_animation_state() == Animation_State::AS_Terminated) {unload_animation_window();a_current_state.current_animation_state = Animation_State::AS_Off;}
        }
    }
}
