// Auteurs: Tahri Malik, Laachfoubi Kamil
// Nom du fichier: graphic_gui.h
// version 1

#ifndef GRAPHIC_GUI_H
#define GRAPHIC_GUI_H

#include <gtkmm/drawingarea.h>
#include "graphic.h"

void graphic_set_context(const Cairo::RefPtr<Cairo::Context> &cr);

#endif // GTKMM_EXAMPLE_GRAPHIC_GUI_H