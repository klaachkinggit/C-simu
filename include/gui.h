// Auteurs: Tahri Malik, Laachfoubi Kamil
// Nom du fichier: gui.h
// version 1

#ifndef GUI_H
#define GUI_H

#include <gtkmm/window.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm.h>
#include "simulation.h"

struct Frame // Model Framing and window parameters
{
    double xMin; // frame parameters
    double xMax;
    double yMin;
    double yMax;
    double asp; // frame aspect ratio
    int height; // window height
    int width;  // window width
};

class MyArea : public Gtk::DrawingArea
{
public:
    MyArea(Simulation &sim);
    virtual ~MyArea();

    void setFrame(Frame x);
    void adjustFrame(int width, int height);
    void dessine() { set_draw_func(sigc::mem_fun(*this, &MyArea::on_draw)); }
    void clear();
    void draw();

protected:
    void on_draw(const Cairo::RefPtr<Cairo::Context> &cr, int width, int height);

private:
    Frame frame;
    bool empty;
    Simulation *simulation; // Pointer to the simulation
};

class MyEvent : public Gtk::Window
{
public:
    MyEvent(Simulation &simul);

protected:
    void setupLayouts();
    void setupBoxes();
    void setupLabelsAndData();
    void setupButtonsAndSignals();
    void clicked_exit();
    void clicked_open();
    void on_file_dialog_response_open(int response_id, Gtk::FileChooserDialog *dialog);
    void on_file_dialog_response_save(int response_id, Gtk::FileChooserDialog *dialog);
    void clicked_save();
    void clicked_start();
    bool on_timeout();
    void add_timer();
    void delete_timer();
    void clicked_step();
    void algues_toggled();
    bool key_pressed(guint keyval, guint keycode, Gdk::ModifierType state);

    MyArea m_Area;
    Gtk::Box m_Main_Box;
    Gtk::Box m_Side_Box;
    Gtk::Box m_Buttons_Box;
    Gtk::Label m_Label_General;
    Gtk::Button m_Button_exit;
    Gtk::Button m_Button_open;
    Gtk::Button m_Button_save;
    Gtk::Button m_Button_start_stop;
    Gtk::Button m_Button_step;
    Gtk::CheckButton m_Checkbox_algue;
    Gtk::Box m_Infos_Box;
    Gtk::Label m_Label_Info;
    Gtk::Box m_Maj_Box;
    Gtk::Label m_Label_maj, data_label_maj;
    Gtk::Box m_Algues_Box;
    Gtk::Label m_Label_algues, data_label_algues;
    Gtk::Box m_Coraux_Box;
    Gtk::Label m_Label_coraux, data_label_coraux;
    Gtk::Box m_Charognards_Box;
    Gtk::Label m_Label_charognards, data_label_charognards;

    Simulation *simu;
    // to store a single timer disconnect request
    bool disconnect;
    const int timeout_value;
    bool keyb_driven_state;
};

#endif
