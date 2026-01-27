// Auteurs: Tahri Malik 50%, Laachfoubi Kamil50%
// Nom du fichier: shape.cc
// version 1

#include <iostream>
#include <cmath>
#include "graphic.h"
#include "shape.h"

using namespace std;

// Given three collinear points p, q, r, the function checks if
// point q lies on line segment 'pr'

static bool onSegment(S2d p, S2d q, S2d r, bool epsil);
static int orientation(S2d p, S2d q, S2d r, bool epsil);

Carre::Carre(S2d base, double l_cote)
    : base(base), l_cote(l_cote) {}

S2d Carre::getbase() const { return base; }
double Carre::getl_cote() const { return l_cote; }

void Carre::set_base(S2d b) { base = b; }
void Carre::set_l_cote(double l) { l_cote = l; }
void Carre::dessin(double red, double green, double blue)
    const { graphic_draw_carre(base.x, base.y, l_cote, red, green, blue); }

Cercle::Cercle(S2d centre, double rayon)
    : centre(centre), rayon(rayon) {}

S2d Cercle::getcentre() const { return centre; }
double Cercle::getrayon() const { return rayon; }

void Cercle::set_centre(S2d c) { centre = c; }
void Cercle::set_rayon(double r) { rayon = r; }
void Cercle::dessin(double red, double green, double blue)
    const { graphic_draw_cercle(centre.x, centre.y, rayon, red, green, blue); }

Segment::Segment(S2d base, double angle, double longueur)
    : base(base), angle(angle), longueur(longueur)
{
    set_end(base, angle, longueur);
}

S2d Segment::getbase() const { return base; }
S2d Segment::getend() const { return end; }
double Segment::getlongueur() const { return longueur; }
double Segment::getangle() const { return angle; }

void Segment::set_base(S2d b)
{
    base = b;
    set_end(base, angle, longueur);
}
void Segment::set_angle(double a)
{
    angle = a;
    set_end(base, angle, longueur);
}
void Segment::set_longueur(double l)
{
    longueur = l;
    set_end(base, angle, longueur);
}
void Segment::set_end(S2d b, double a, double l)
{
    end.x = b.x + l * cos(a);
    end.y = b.y + l * sin(a);
}
void Segment::dessin(double red, double green, double blue)
    const { graphic_draw_segment(base.x, base.y, end.x, end.y, red, green, blue); }

bool doIntersect(S2d p1, S2d q1, S2d p2, S2d q2, bool epsil)
{
    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(p1, q1, p2, epsil);
    int o2 = orientation(p1, q1, q2, epsil);
    int o3 = orientation(p2, q2, p1, epsil);
    int o4 = orientation(p2, q2, q1, epsil);

    if (o1 != o2 && o3 != o4)
        return true;

    if (o1 == 0 && onSegment(p1, p2, q1, epsil))
        return true;

    if (o2 == 0 && onSegment(p1, q2, q1, epsil))
        return true;

    if (o3 == 0 && onSegment(p2, p1, q2, epsil))
        return true;

    if (o4 == 0 && onSegment(p2, q1, q2, epsil))
        return true;

    return false;
}

bool segment_common_superposition(Segment const &segk, Segment const &segk_1)
{

    if (ang_deviation(segk, segk_1) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

double ang_deviation(Segment const &segk, Segment const &segk_1)
{
    double ecart = atan2(segk_1.getend().y - segk_1.getbase().y,
                         segk_1.getend().x - segk_1.getbase().x) -
                   atan2(segk.getend().y - segk.getbase().y,
                         segk.getend().x - segk.getbase().x);
    return (ecart < 0) ? -M_PI - ecart : M_PI - ecart;
}

double calcul_angle_2vect(S2d base, S2d point_1, S2d point_2)
{
    S2d vector1 = {point_1.x - base.x, point_1.y - base.y};
    S2d vector2 = {point_2.x - base.x, point_2.y - base.y};
    double angle = (atan2(vector1.y, vector1.x) - atan2(vector2.y, vector2.x));
    while (angle > M_PI)
        angle -= 2 * M_PI;
    while (angle < -M_PI)
        angle += 2 * M_PI;
    return angle;
}

double distance(S2d p, S2d q)
{
    return sqrt((q.y - p.y) * (q.y - p.y) + (q.x - p.x) * (q.x - p.x));
}

static bool onSegment(S2d p, S2d q, S2d r, bool epsil)
{
    double s = (q.x - p.x) * (r.x - p.x) + (q.y - p.y) * (r.y - p.y);
    double x = s / sqrt((r.y - p.y) * (r.y - p.y) + (r.x - p.x) * (r.x - p.x));
    if (epsil)
    {
        return (((-epsil_zero) <= x) and (x <= sqrt((r.y - p.y) * (r.y - p.y) +
                                                    (r.x - p.x) * (r.x - p.x)) +
                                                   epsil_zero));
    }
    else
    {
        return ((0 <= x) and (x <= sqrt((r.y - p.y) * (r.y - p.y) +
                                        (r.x - p.x) * (r.x - p.x))));
    }
}

static int orientation(S2d p, S2d q, S2d r, bool epsil)
{
    // See https://www.geeksforgeeks.org/orientation-3-ordered-points/
    // for details of below formula.
    double val = (q.y - p.y) * (r.x - q.x) -
                 (q.x - p.x) * (r.y - q.y);
    val /= sqrt((q.y - p.y) * (q.y - p.y) + (q.x - p.x) * (q.x - p.x));

    if (epsil)
    {
        if (abs(val) <= 0)
            return 0; // collinear
        return (val > 0) ? 1 : 2;
    }
    else
    {
        if (abs(val) == 0)
            return 0;             // collinear
        return (val > 0) ? 1 : 2; // clock or counterclock wise
    }
}
