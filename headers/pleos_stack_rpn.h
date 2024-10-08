//******************
//
// pleos_data_structure.h
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
// This file contains the page of the Atmosphere plugin.
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

#ifndef PLEOS_STACK_RPN
#define PLEOS_STACK_RPN

// Include SCLS Graphic Benoit
#include "../../../scls-graphic-benoit/scls_graphic.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {

    //******************
    //
    // The "Stack_RPN_Page" class
    //
    //******************

    class Stack_RPN_Page : public scls::GUI_Page {
        // Class representating the stack RPN page for PLEOS
    public:

        // Current possible state of the animation
        enum Animation_State {AS_Off, AS_Started, AS_Terminated};

        // Stack_RPN_Page constructor
        Stack_RPN_Page(scls::_Window_Advanced_Struct* window_struct, std::string name);
        // Loads an object in a page from XML
        virtual std::shared_ptr<scls::GUI_Object> __create_loaded_object_from_type(std::string object_name, std::string object_type, scls::GUI_Object* parent);

        // Update the events in the page
        virtual void update_event();

        // Analyse the needed next character
        void analyse_next_character();
        // Place the analysed text GUI
        void place_analysed_text();
        // Setup the animation window
        void setup_animation_window();
        // Unload the animation window
        inline void unload_animation_window() {
            a_main_animation_explaination.reset();
            a_main_animation_text.reset();
            a_main_animation_title.reset();
            if(a_stack_rpn_next_button.get() != 0)a_stack_rpn_next_button.get()->set_texture("stack_rpn_page_button_start", false);
            a_text_analysed_gui.clear();
            if(a_stack_rpn_animation_window.get() != 0)a_stack_rpn_animation_window.get()->delete_children();
            if(a_stack_rpn_stack.get() != 0)a_stack_rpn_stack.get()->reset();
            text_analysed().clear();
            text_to_analyse().clear();
        };

        // Returns the current animation text
        inline std::string current_animation_text() const {
            std::string text = ""; const std::vector<std::string>& text_input = a_current_state.text_to_analyse;
            for(int i = 0;i<static_cast<int>(text_input.size());i++) {text += text_input[i];text += " ";}
            while(text[text.size() - 1] == ' ') text = text.substr(0, text.size() - 1);
            return text;
        };
        // Returns the input in the page
        inline std::string input() const {if(a_stack_rpn_input.get() == 0)return "";return a_stack_rpn_input.get()->text();};

        // Getters and setters
        inline unsigned int analyse_number() const {return a_current_state.analyse_number;};
        inline Animation_State current_animation_state() const {return a_current_state.current_animation_state;};
        inline std::vector<double>& text_analysed() {return a_current_state.text_analysed;};
        inline std::vector<std::string>& text_to_analyse() {return a_current_state.text_to_analyse;};

    private:

        // Avaible operator in the software
        const std::map<std::string, unsigned char> a_avaible_operator{{"+", 2}, {"-", 2}, {"*", 2}, {"/", 2}, {"sqrt", 1}};

        // State of the page
        struct {
            // Current state of the animation
            Animation_State current_animation_state = AS_Off;

            // Number of made analyse
            unsigned int analyse_number = 0;
            // Text analysed
            std::vector<double> text_analysed;
            // Text to analyse
            std::vector<std::string> text_to_analyse;
        } a_current_state;

        // GUI stuff

        // Animation explaination
        std::shared_ptr<scls::GUI_Text> a_main_animation_explaination;
        // Animation text
        std::shared_ptr<scls::GUI_Text> a_main_animation_text;
        // Animation title
        std::shared_ptr<scls::GUI_Text> a_main_animation_title;
        // Animation window
        std::shared_ptr<scls::GUI_Object> a_stack_rpn_animation_window;
        // Input of the text
        std::shared_ptr<scls::GUI_Text_Input> a_stack_rpn_input;
        // Next button
        std::shared_ptr<scls::GUI_Object> a_stack_rpn_next_button;
        // Stack at the left of the page
        std::shared_ptr<scls::GUI_Scroller> a_stack_rpn_stack;
        // Text analysed GUI elements
        std::vector<std::shared_ptr<scls::GUI_Text>> a_text_analysed_gui;
    };

    //******************
    //
    // Forbidden class for PLEOS prototypes
    //
    //******************

    class __Temp_Pleos_Window : public scls::Window {
        // Temporary used window by PLEOS
    public :
        // __Temp_Pleos_Window constructor
        __Temp_Pleos_Window(unsigned int w, unsigned int h, std::string path) : scls::Window(w, h, path) { set_window_title("PLEOS - Piles"); }

        // Create an object in the window
        std::shared_ptr<scls::Object> __create_loaded_object_from_type(std::string object_name, std::string object_type) {
            if(object_name == "stack_rpn_page") {
                std::shared_ptr<Stack_RPN_Page> hub = *new_page_2d<Stack_RPN_Page>(object_name);

                // Return the good object
                std::shared_ptr<scls::Object> to_return = hub;
                return to_return;
            } return scls::Window::__create_loaded_object_from_type(object_name, object_type);
        }
    };
}

#endif // PLEOS_STACK_RPN
