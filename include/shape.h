// Auteurs: Tahri Malik, Laachfoubi Kamil
// Nom du fichier: shape.h
// version 1

#ifndef SHAPE_H
#define SHAPE_H

#include <iostream>
#include "graphic.h"

constexpr double epsil_zero(0.5);

// S2d
struct S2d
{
    double x;
    double y;
};

// Carré,Cercle,Segment
class Carre
{
public:
    Carre(S2d base, double l_cote = 0.);
    S2d getbase() const;
    double getl_cote() const;
    void set_base(S2d b);
    void set_l_cote(double l);
    void dessin(double red, double green, double blue) const;

private:
    S2d base;
    double l_cote;
};

class Cercle
{
public:
    Cercle(S2d centre, double rayon = 0.);
    S2d getcentre() const;
    double getrayon() const;
    void set_centre(S2d c);
    void set_rayon(double r);
    void dessin(double red, double green, double blue) const;

private:
    S2d centre;
    double rayon;
};

class Segment
{
public:
    Segment(S2d base, double angle = 0, double longueur = 0.);
    S2d getbase() const;
    S2d getend() const;
    double getlongueur() const;
    double getangle() const;
    void set_base(S2d b);
    void set_angle(double a);
    void set_longueur(double l);
    void set_end(S2d b, double a, double l);
    void dessin(double red, double green, double blue) const;

private:
    S2d base;
    double angle;
    double longueur;
    S2d end;
};

bool segment_common_superposition(Segment const &segk, Segment const &segk_1);

bool doIntersect(S2d p1, S2d q1, S2d p2, S2d q2, bool epsil);

double ang_deviation(Segment const &segk, Segment const &segk_1);

double calcul_angle_2vect(S2d base, S2d point_1, S2d point_2);

double distance(S2d p, S2d q);

#endif