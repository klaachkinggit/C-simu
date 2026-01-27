// Auteurs: Tahri Malik 10%, Laachfoubi Kamil 90%
// Nom du fichier: graphic.cc
// version 1

#include <iostream>

#include "graphic_gui.h"
#include "graphic.h"

using namespace std;

static const Cairo::RefPtr<Cairo::Context> *ptcr;

void graphic_set_context(const Cairo::RefPtr<Cairo::Context> &cr)
{
    ptcr = &cr;
}

void graphic_draw_segment(double xc_1, double yc_1, double xc_2, double yc_2,
                          double red, double green, double blue)
{
    (*ptcr)->save();
    (*ptcr)->set_line_width(1.0);
    // Utiliser les paramètres de couleur fournis pour dessiner
    (*ptcr)->set_source_rgb(red, green, blue);
    (*ptcr)->move_to(xc_1, yc_1);
    (*ptcr)->line_to(xc_2, yc_2);
    (*ptcr)->stroke();
    (*ptcr)->restore();
}

void graphic_draw_carre(double xc, double yc, double size, double red,
                        double green, double blue)
{
    (*ptcr)->save();
    (*ptcr)->set_line_width(0.5);
    // Utiliser les paramètres de couleur fournis pour dessiner
    (*ptcr)->set_source_rgb(red, green, blue);
    (*ptcr)->rectangle(xc - size / 2, yc - size / 2, size, size);
    (*ptcr)->stroke();
    (*ptcr)->restore();
}

void graphic_draw_cercle(double xc, double yc, double radius, double red,
                         double green, double blue)
{
    (*ptcr)->save();
    (*ptcr)->set_line_width(0.5);
    // Utiliser les paramètres de couleur fournis pour dessiner
    (*ptcr)->set_source_rgb(red, green, blue);
    (*ptcr)->arc(xc, yc, radius, 0.0, 2.0 * M_PI); // full circle
    (*ptcr)->stroke();
    (*ptcr)->restore();
}
