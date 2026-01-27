// Auteurs: Tahri Malik 10%, Laachfoubi Kamil 90%
// Nom du fichier: gui.cc
// version 1

#include <cairomm/context.h>
#include <gtkmm/label.h>
#include <iostream>
#include <filesystem>
#include "simulation.h"
#include "graphic_gui.h"
#include "gui.h"

using namespace std;

constexpr int area_side(500);

static void draw_frame(const Cairo::RefPtr<Cairo::Context> &cr, Frame frame);
static void orthographic_projection(const Cairo::RefPtr<Cairo::Context> &cr,
                                    Frame frame);

static bool is_checked;
static bool toggled = false;
static unsigned int val(1);

// default Model Framing and window parameters
static Frame default_frame = {0., 256, 0, 256, 1, 256 / 2, 256 / 2};

MyArea::MyArea(Simulation &sim) : empty(false), simulation(&sim)
{
    set_content_width(area_side);
    set_content_height(area_side);
    set_draw_func(sigc::mem_fun(*this, &MyArea::on_draw));
}

MyArea::~MyArea()
{
}

void MyArea::clear()
{
    empty = true;
    queue_draw();
}
void MyArea::draw()
{
    std::cout << "Drawing area set to non-empty." << std::endl;
    empty = false;
    queue_draw();
}
// defining the Model space frame to visualize in the window canvas
void MyArea::setFrame(Frame f)
{
    if ((f.xMin <= f.xMax) and (f.yMin <= f.yMax) and (f.height > 0))
    {
        f.asp = f.width / f.height;
        frame = f;
    }
    else
        std::cout << "incorrect Model framing or window parameters" << std::endl;
}

void MyArea::adjustFrame(int width, int height)
{
    frame.width = width;
    frame.height = height;

    // Preventing distorsion by adjusting the frame (cadrage)
    // to have the same proportion as the graphical area

    // use the reference framing as a guide for preventing distortion
    double new_aspect_ratio((double)width / height);
    if (new_aspect_ratio > default_frame.asp)
    { // keep yMax and yMin. Adjust xMax and xMin
        frame.yMax = default_frame.yMax;
        frame.yMin = default_frame.yMin;

        double delta(default_frame.xMax - default_frame.xMin);
        double mid((default_frame.xMax + default_frame.xMin) / 2);
        // the new frame is centered on the mid-point along X
        frame.xMax = mid + 0.5 * (new_aspect_ratio / default_frame.asp) * delta;
        frame.xMin = mid - 0.5 * (new_aspect_ratio / default_frame.asp) * delta;
    }
    else
    { // keep xMax and xMin. Adjust yMax and yMin
        frame.xMax = default_frame.xMax;
        frame.xMin = default_frame.xMin;

        double delta(default_frame.yMax - default_frame.yMin);
        double mid((default_frame.yMax + default_frame.yMin) / 2);
        // the new frame is centered on the mid-point along Y
        frame.yMax = mid + 0.5 * (default_frame.asp / new_aspect_ratio) * delta;
        frame.yMin = mid - 0.5 * (default_frame.asp / new_aspect_ratio) * delta;
    }
}

void MyArea::on_draw(const Cairo::RefPtr<Cairo::Context> &cr, int width, int height)
{
    if (not empty) // drawing in the Model space
    {
        // adjust the frame (cadrage) to prevent distortion
        adjustFrame(width, height);
        draw_frame(cr, frame);              // drawing the drawingArea space
        orthographic_projection(cr, frame); // set the transformation MODELE to GTKmm
        graphic_set_context(cr);
        graphic_draw_carre(256 / 2, 256 / 2, 256 - 1, 0.5, 0.5, 0.5);
        simulation->dessin();
    }
    else
    {
        // Effacer l'écran en remplissant avec une couleur de fond (blanc ici)
        cr->set_source_rgb(1.0, 1.0, 1.0);  // Couleur blanche
        cr->rectangle(0, 0, width, height); // Couvrir toute la surface
        cr->fill();                         // Appliquer la couleur
        std::cout << "Area is empty, nothing to draw." << std::endl;
    }
}

MyEvent::MyEvent(Simulation &simul)
    : m_Area(simul), m_Main_Box(Gtk::Orientation::HORIZONTAL, 0),
      m_Side_Box(Gtk::Orientation::VERTICAL, 0),
      m_Buttons_Box(Gtk::Orientation::VERTICAL, 1),
      m_Label_General("General"),
      m_Button_exit("exit"),
      m_Button_open("open"),
      m_Button_save("save"),
      m_Button_start_stop("start"),
      m_Button_step("step"),
      m_Checkbox_algue("Naissance d'algue"),
      m_Infos_Box(Gtk::Orientation::VERTICAL, 2),
      m_Label_Info("Info: nombre de..."),
      m_Maj_Box(Gtk::Orientation::HORIZONTAL, 1),
      m_Label_maj("mise à jour:        "),
      data_label_maj("0"),
      m_Algues_Box(Gtk::Orientation::HORIZONTAL, 1),
      m_Label_algues("algues:               "),
      data_label_algues("0"),
      m_Coraux_Box(Gtk::Orientation::HORIZONTAL, 1),
      m_Label_coraux("coraux:               "),
      data_label_coraux("0"),
      m_Charognards_Box(Gtk::Orientation::HORIZONTAL, 1),
      m_Label_charognards("charognards:      "),
      data_label_charognards("0"),
      simu(&simul),
      disconnect(false),
      timeout_value(25),       // Value for timer
      keyb_driven_state(false) // Keyboard-driven state initialization
{
    set_title("Microrécif");
    setupLayouts();
    setupBoxes();
    setupLabelsAndData();
    setupButtonsAndSignals();
}

void MyEvent::setupLayouts()
{
    set_child(m_Main_Box);
    m_Main_Box.append(m_Side_Box);
    m_Main_Box.append(m_Area);
    m_Area.set_expand();
    m_Side_Box.append(m_Buttons_Box);
    m_Side_Box.append(m_Infos_Box);
}

void MyEvent::setupBoxes()
{
    m_Buttons_Box.append(m_Label_General);
    m_Buttons_Box.append(m_Button_exit);
    m_Buttons_Box.append(m_Button_open);
    m_Buttons_Box.append(m_Button_save);
    m_Buttons_Box.append(m_Button_start_stop);
    m_Buttons_Box.append(m_Button_step);
    m_Buttons_Box.append(m_Checkbox_algue);
    m_Infos_Box.append(m_Label_Info);
    m_Infos_Box.append(m_Maj_Box);
    m_Infos_Box.append(m_Algues_Box);
    m_Infos_Box.append(m_Coraux_Box);
    m_Infos_Box.append(m_Charognards_Box);
}

void MyEvent::setupLabelsAndData()
{
    m_Maj_Box.append(m_Label_maj);
    m_Maj_Box.append(data_label_maj);
    m_Algues_Box.append(m_Label_algues);
    m_Algues_Box.append(data_label_algues);
    m_Coraux_Box.append(m_Label_coraux);
    m_Coraux_Box.append(data_label_coraux);
    m_Charognards_Box.append(m_Label_charognards);
    m_Charognards_Box.append(data_label_charognards);
    data_label_algues.set_text(std::to_string(simu->get_nbalgues()));
    data_label_coraux.set_text(std::to_string(simu->get_nbcoraux()));
    data_label_charognards.set_text(std::to_string(simu->get_nbscavengers()));
}

void MyEvent::setupButtonsAndSignals()
{
    m_Button_exit.signal_clicked().connect(sigc::mem_fun(*this,
                                                         &MyEvent::clicked_exit));
    m_Button_open.signal_clicked().connect(sigc::mem_fun(*this,
                                                         &MyEvent::clicked_open));
    m_Button_save.signal_clicked().connect(sigc::mem_fun(*this,
                                                         &MyEvent::clicked_save));
    m_Button_start_stop.signal_clicked().connect(sigc::
                                                     mem_fun(*this,
                                                             &MyEvent::clicked_start));
    m_Button_step.signal_clicked().connect(sigc::mem_fun(*this,
                                                         &MyEvent::clicked_step));
    m_Checkbox_algue.signal_toggled().connect(sigc::mem_fun(*this,
                                                            &MyEvent::algues_toggled));

    auto controller = Gtk::EventControllerKey::create();
    controller->signal_key_pressed().connect(sigc::mem_fun(*this,
                                                           &MyEvent::key_pressed),
                                             false);
    add_controller(controller);
}

void MyEvent::clicked_exit()
{
    std::cout << "exit" << std::endl;
    hide();
}

void MyEvent::clicked_open()
{
    std::cout << "open" << std::endl;
    auto dialog = new Gtk::FileChooserDialog("Please choose a file",
                                             Gtk::FileChooser::Action::OPEN);
    dialog->set_transient_for(*this);
    dialog->set_modal(true);
    dialog->signal_response().connect(sigc::bind(
        sigc::mem_fun(*this, &MyEvent::on_file_dialog_response_open), dialog));

    // Add response buttons to the dialog:
    dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
    dialog->add_button("_Open", Gtk::ResponseType::OK);

    // Add filters, so that only certain file types can be selected:

    auto filter_text = Gtk::FileFilter::create();
    filter_text->set_name("Text files");
    filter_text->add_mime_type("text/plain");
    dialog->add_filter(filter_text);

    auto filter_cpp = Gtk::FileFilter::create();
    filter_cpp->set_name("C/C++ files");
    filter_cpp->add_mime_type("text/x-c");
    filter_cpp->add_mime_type("text/x-c++");
    filter_cpp->add_mime_type("text/x-c-header");
    dialog->add_filter(filter_cpp);

    auto filter_any = Gtk::FileFilter::create();
    filter_any->set_name("Any files");
    filter_any->add_pattern("*");
    dialog->add_filter(filter_any);

    // Show the dialog and wait for a user response:
    dialog->show();
}

void MyEvent::on_file_dialog_response_open(int response_id,
                                           Gtk::FileChooserDialog *dialog)
{
    // Handle the response:
    switch (response_id)
    {
    case Gtk::ResponseType::OK:
    {
        std::string filename = dialog->get_file()->get_path();
        std::cout << "Open clicked." << std::endl;
        std::cout << "File selected: " << filename << std::endl;

        simu->lecture(filename);

        // Rafraîchir l'affichage pour montrer les nouvelles données
        m_Area.clear();
        m_Area.draw();

        m_Checkbox_algue.set_active(false);
        is_checked = false;
        data_label_maj.set_text("0");
        val = 1;
        data_label_algues.set_text(std::to_string(simu->get_nbalgues()));
        data_label_coraux.set_text(std::to_string(simu->get_nbcoraux()));
        data_label_charognards.set_text(std::to_string(simu->get_nbscavengers()));
        break;
    }
    case Gtk::ResponseType::CANCEL:
    {
        std::cout << "Cancel clicked." << std::endl;
        break;
    }
    default:
    {
        std::cout << "Unexpected button clicked." << std::endl;
        break;
    }
    }
    delete dialog;
}

void MyEvent::on_file_dialog_response_save(int response_id,
                                           Gtk::FileChooserDialog *dialog)
{
    switch (response_id)
    {
    case Gtk::ResponseType::OK:
    {
        // Obtenez le chemin du fichier en tant que std::string
        auto file = dialog->get_file();
        std::string filepath = file->get_path();

        // Ouvrez un ofstream avec ce chemin
        std::ofstream out(filepath);
        if (out)
        {
            // Écrivez les données nécessaires
            simu->sauvegarde_fichier(out);
            out.close();
            std::cout << "File saved successfully to " << filepath << std::endl;
        }
        else
        {
            std::cout << "Failed to open file for writing." << std::endl;
        }
        break;
    }
    case Gtk::ResponseType::CANCEL:
    {
        std::cout << "Save canceled." << std::endl;
        break;
    }
    default:
    {
        std::cout << "Unexpected button clicked." << std::endl;
        break;
    }
    }
    delete dialog;
}

void MyEvent::clicked_save()
{
    std::cout << "save" << std::endl;

    auto dialog = new Gtk::FileChooserDialog("Please choose a file",
                                             Gtk::FileChooser::Action::SAVE);
    dialog->set_transient_for(*this);
    dialog->set_modal(true);
    dialog->signal_response().connect(sigc::bind(
        sigc::mem_fun(*this, &MyEvent::on_file_dialog_response_save), dialog));

    // Add response buttons to the dialog:
    dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
    dialog->add_button("_Save", Gtk::ResponseType::OK);

    // Add filters, so that only certain file types can be selected:

    auto filter_text = Gtk::FileFilter::create();
    filter_text->set_name("Text files");
    filter_text->add_mime_type("text/plain");
    dialog->add_filter(filter_text);

    auto filter_cpp = Gtk::FileFilter::create();
    filter_cpp->set_name("C/C++ files");
    filter_cpp->add_mime_type("text/x-c");
    filter_cpp->add_mime_type("text/x-c++");
    filter_cpp->add_mime_type("text/x-c-header");
    dialog->add_filter(filter_cpp);

    auto filter_any = Gtk::FileFilter::create();
    filter_any->set_name("Any files");
    filter_any->add_pattern("*");
    dialog->add_filter(filter_any);

    // Show the dialog and wait for a user response:
    dialog->show();
}

void MyEvent::clicked_start()
{
    std::cout << "start" << std::endl;
    if (!toggled)
    {
        m_Button_start_stop.set_label("stop");
        toggled = true;
        add_timer();
    }
    else
    {
        m_Button_start_stop.set_label("start");
        toggled = false;
        delete_timer();
    }
}

void MyEvent::add_timer()
{
    // Creation of a new object prevents long lines and shows us a little
    // how slots work.  We have 0 parameters and bool as a return value
    // after calling sigc::bind.
    sigc::slot<bool()>
        my_slot = sigc::bind(sigc::mem_fun(*this,
                                           &MyEvent::on_timeout));

    // This is where we connect the slot to the Glib::signal_timeout()
    auto conn = Glib::signal_timeout().connect(my_slot, timeout_value);

    std::cout << "Timer added" << std::endl;
}

void MyEvent::delete_timer()
{
    std::cout << "manually disconnecting the timer " << std::endl;
    disconnect = true;
}

bool MyEvent::on_timeout()
{
    if (disconnect)
    {
        disconnect = false; // reset for next time a Timer is created

        return false; // End of Timer
    }

    data_label_maj.set_text(std::to_string(val)); // display he simulation clock
    simu->execution(is_checked);
    data_label_algues.set_text(std::to_string(simu->get_nbalgues()));
    data_label_coraux.set_text(std::to_string(simu->get_nbcoraux()));
    data_label_charognards.set_text(std::to_string(simu->get_nbscavengers()));

    m_Area.queue_draw();
    std::cout << "This is simulation update number : " << val << std::endl;

    ++val;
    return true;
}

void MyEvent::clicked_step()
{

    if (!toggled)
    {
        std::cout << "step" << std::endl;
        data_label_maj.set_text(std::to_string(val)); // display he simulation clock
        simu->execution(is_checked);
        data_label_algues.set_text(std::to_string(simu->get_nbalgues()));
        data_label_coraux.set_text(std::to_string(simu->get_nbcoraux()));
        data_label_charognards.set_text(std::to_string(simu->get_nbscavengers()));

        m_Area.clear();
        m_Area.draw();
        std::cout << "This is simulation update number : " << val << std::endl;
        ++val;
    }
}

void MyEvent::algues_toggled()
{
    is_checked = m_Checkbox_algue.get_active();
    std::cout << "Checkbox for algues is ";
    std::cout << (is_checked ? "checked" : "unchecked") << std::endl;
}

bool MyEvent::key_pressed(guint keyval, guint, Gdk::ModifierType state)
{
    switch (gdk_keyval_to_unicode(keyval))
    {
    case 's':
        std::cout << " key s" << std::endl;
        clicked_start();
        return true;
    case '1':
        std::cout << " key 1"
                  << std::endl;
        clicked_step();
        return true;
    }
    // the event has not been handled
    return false;
}

static void draw_frame(const Cairo::RefPtr<Cairo::Context> &cr, Frame frame)
{
    cr->set_line_width(1.0);
    cr->set_source_rgb(0, 0, 0);
    cr->rectangle(0, 0, frame.width, frame.height);
    cr->stroke();
}

static void orthographic_projection(const Cairo::RefPtr<Cairo::Context> &cr,
                                    Frame frame)
{
    // déplace l'origine au centre de la fenêtre
    cr->translate(frame.width / 2, frame.height / 2);

    // normalise la largeur et hauteur aux valeurs fournies par le cadrage
    // ET inverse la direction de l'axe Y
    cr->scale(frame.width / (frame.xMax - frame.xMin),
              -frame.height / (frame.yMax - frame.yMin));

    // décalage au centre du cadrage
    cr->translate(-(frame.xMin + frame.xMax) / 2, -(frame.yMin + frame.yMax) / 2);
}