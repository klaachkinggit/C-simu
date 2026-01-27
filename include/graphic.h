// Auteurs: Tahri Malik, Laachfoubi Kamil
// Nom du fichier: graphic.h
// version 1

#ifndef GRAPHIC_H
#define GRAPHIC_H

void graphic_draw_segment(double xc_1, double yc_1, double xc_2, double yc_2,
                          double red, double green, double blue);

void graphic_draw_carre(double xc, double yc, double size, double red,
                        double green, double blue);

void graphic_draw_cercle(double xc, double yc, double radius, double red,
                         double green, double blue);

#endif