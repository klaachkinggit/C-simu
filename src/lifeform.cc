// Auteurs: Tahri Malik 50%, Laachfoubi Kamil 50%
// Nom du fichier: lifeform.cc
// version 1

#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "message.h"
#include "shape.h"
#include "constantes.h"
#include "lifeform.h"

using namespace std;

S2d Lifeform::getPosition() const { return position; }

unsigned int Lifeform::getAge() const { return age; }

void Lifeform::setposition(const S2d pos, bool &etat_lecture)
{
    if (((1 <= pos.x) and (pos.x <= dmax - 1)) and
        ((1 <= pos.y) and (pos.y <= dmax - 1)))
    {
        position = pos;
    }
    else
    {
        cout << message::lifeform_center_outside(pos.x, pos.y);
        etat_lecture = false;
    }
}

void Lifeform::set_age(const int a, const int age_max, bool &etat_lecture)
{
    if ((a <= 0) or (a >= age_max))
    {
        cout << message::lifeform_age(a);
        etat_lecture = false;
    }
    else
    {
        age = a;
    }
}

Algue::Algue(istringstream &donnee, bool &etat_lecture) : forme(S2d(), r_alg)
{
    S2d pos;
    unsigned int a;
    if (!(donnee >> pos.x >> pos.y >> a))
    {
        etat_lecture = false;
    }
    setposition(pos, etat_lecture);
    forme.set_centre(pos);
    set_age(a, max_life_alg, etat_lecture); // Initialiser l'âge de la vie
}
Algue::~Algue()
{
}

void Algue::dessin() const
{
    forme.dessin(0, 1, 0);
}

Corail::Corail(istringstream &donnee, Coraux const &coraux, bool &etat_lecture)
    : base(S2d(), d_cor)
{
    S2d pos;
    int a, id_cor;
    bool statut_c, dir_r, statut_d;

    if (!(donnee >> pos.x >> pos.y >> a >> id_cor >>
          statut_c >> dir_r >> statut_d >> nb_segs))
    {
        etat_lecture = false;
    }
    setposition(pos, etat_lecture);
    base.set_base(pos);
    set_age(a, max_life_cor, etat_lecture);
    setId(id_cor, coraux, etat_lecture);
    setStatut_cor(statut_c);
    setDir_rot(dir_r);
    setStatut_dev(statut_d);
}
Corail::~Corail()
{
}

int Corail::getId() const { return id; }

bool Corail::getStatut_cor() const
{
    return statut_cor;
}

bool Corail::getDir_rot() const
{
    return dir_rot;
}

bool Corail::getStatut_dev() const { return statut_dev; }

int Corail::getnb_segs() const { return nb_segs; }

vector<Segment> Corail::getSegments() const { return segments; }

void Corail::setId(const int i, Coraux const &coraux, bool &etat_lecture)
{
    for (auto &corail_intra : coraux)
    {
        if (corail_intra.getId() == i)
        {
            cout << message::lifeform_duplicated_id(i);
            etat_lecture = false;
        }
    }
    id = i;
}

void Corail::setStatut_cor(const bool statut)
{
    if (statut)
    {
        statut_cor = ALIVE;
    }
    else
    {
        statut_cor = DEAD;
    }
}

void Corail::setDir_rot(const bool dir)
{
    if (dir)
    {
        dir_rot = INVTRIGO;
    }
    else
    {
        dir_rot = TRIGO;
    }
}

void Corail::setStatut_dev(const bool statut)
{
    if (statut)
    {
        statut_dev = REPRO;
    }
    else
    {
        statut_dev = EXTEND;
    }
}

bool Corail::superposition_collision(Segment const &seg,
                                     Coraux const &coraux, bool epsil,
                                     bool &etat_lecture, int id)
{
    if ((segments.size() != 0) and
        (segment_common_superposition(segments[segments.size() - 1], seg)))
    {
        cout << message::segment_superposition(id, segments.size() - 1,
                                               segments.size());
        etat_lecture = false;
        return true;
    }
    else
    {
        verification_collision(seg, coraux, epsil, etat_lecture, id);
        return false;
    }
}

void Corail::check_segment_collision(const Segment &seg,
                                     const vector<Segment> &autres_segments,
                                     int coral_id, bool epsil,
                                     bool &etat_lecture)
{
    unsigned int index = 0;
    for (auto &segment_in : autres_segments)
    {
        if (doIntersect(seg.getbase(), seg.getend(), segment_in.getbase(),
                        segment_in.getend(), epsil))
        {
            cout << message::segment_collision(coral_id, index, id,
                                               segments.size());
            etat_lecture = false;
        }
    }
    ++index;
}

void Corail::verification_collision(Segment const &seg,
                                    Coraux const &coraux, bool epsil,
                                    bool &etat_lecture, int id)
{
    if (!((seg.getend().x > epsil_zero) and
          (seg.getend().x < dmax - epsil_zero)) or
        !((seg.getend().y > epsil_zero) and
          (seg.getend().y < dmax - epsil_zero)))
        etat_lecture = false;
    if (segments.size() != 0)
    {
        unsigned int index = 0;
        for (size_t i = 0; i < segments.size() - 1; ++i)
        {
            if (doIntersect(seg.getbase(), seg.getend(), segments[i].getbase(),
                            segments[i].getend(), true))
            {
                cout << message::segment_collision(id, index, id, segments.size());
                etat_lecture = false;
            }
            ++index;
        }
    }
    if (!id)
    {
        for (size_t i = 0; i < coraux.size() - 1; ++i)
        {
            vector<Segment> autres_segments = coraux[i].getSegments();
            check_segment_collision(seg, autres_segments, coraux[i].getId(),
                                    epsil, etat_lecture);
        }
    }
    else
    {
        for (size_t i = 0; i < coraux.size(); ++i)
        {
            if (coraux[i].getId() != id)
            {
                vector<Segment> autres_segments = coraux[i].getSegments();
                check_segment_collision(seg, autres_segments, coraux[i].getId(),
                                        epsil, etat_lecture);
            }
        }
    }
}

void Corail::verification_collision_rebondir(Segment const &copie_effecteur,
                                             bool &etat_lecture)
{
    if (segments.size() != 1)
    {
        if (abs(ang_deviation(segments[segments.size() - 2],
                              segments[segments.size() - 1])) < M_PI / 2)
        {
            if ((dir_rot) and
                (ang_deviation(segments[segments.size() - 2],
                               segments[segments.size() - 1]) *
                     ang_deviation(segments[segments.size() - 2],
                                   copie_effecteur) <
                 0))
            {

                etat_lecture = false;
            }
            if ((!dir_rot) and
                (ang_deviation(segments[segments.size() - 2],
                               segments[segments.size() - 1]) *
                     ang_deviation(segments[segments.size() - 2],
                                   copie_effecteur) <
                 0))
            {

                etat_lecture = false;
            }
        }
    }
}

void Corail::add_segment(istringstream &donnee, Coraux const &coraux, bool epsil,
                         bool &etat_lecture, int id)
{
    double a, s;
    if (!(donnee >> a >> s))
        etat_lecture = false;
    if ((l_repro - l_seg_interne <= s) and (((s < l_repro)) or epsil))
    {
        if ((-M_PI <= a) and (a <= M_PI))
        {
            Segment seg(position, a, s);
            if (segments.size() != 0)
            {
                seg.set_base(segments[segments.size() - 1].getend());
            }
            if (!((seg.getend().x > epsil_zero) and
                  (seg.getend().x < dmax - epsil_zero)) or
                !((seg.getend().y > epsil_zero) and
                  (seg.getend().y < dmax - epsil_zero)))
            {
                cout << message::
                        lifeform_computed_outside(coraux[coraux.size() - 1].getId(),
                                                  seg.getend().x, seg.getend().y);
                etat_lecture = false;
            }
            if (!(superposition_collision(seg, coraux, epsil, etat_lecture, id)))
            {
                segments.push_back(seg);
                nb_segs = segments.size();
            }
        }
        else
        {
            cout << message::segment_angle_outside(id, a);
            etat_lecture = false;
        }
    }
    else
    {
        cout << message::segment_length_outside(id, s);
        etat_lecture = false;
    }
}

void Corail::setSegments(Segments const segs)
{
    segments = segs;
}
void Corail::dessin() const
{
    if (statut_cor)
    {
        base.dessin(0, 0, 1);
    }
    else
    {
        base.dessin(0, 0, 0);
    }
    for (auto &segment_in : segments)
    {
        if (statut_cor)
        {
            segment_in.dessin(0, 0, 1);
        }
        else
        {
            segment_in.dessin(0, 0, 0);
        }
    }
}

Scavenger::Scavenger(istringstream &donnee, Coraux const &coraux, bool &etat_lecture)
    : forme(S2d(), 0.0)
{
    S2d pos;
    unsigned int a, corail_id(0);
    double r;
    bool statut_s;
    if (!(donnee >> pos.x >> pos.y >> a >> r >>
          statut_s))
    {
        etat_lecture = false;
    }
    if (statut_s)
    {
        if (!(donnee >> corail_id))
        {
            etat_lecture = false;
        }
        else
        {
            setId_cible(corail_id, coraux, etat_lecture);
        }
    }
    set_age(a, max_life_sca, etat_lecture);
    setForme(pos, r, etat_lecture);
    setStatut_sca(statut_s);
}
Scavenger::~Scavenger()
{
}

double Scavenger::getRadius() const { return forme.getrayon(); }

bool Scavenger::getStatut_sca() const { return statut_sca; }

int Scavenger::getCorail_id_cible() const { return corail_id_cible; }

void Scavenger::setForme(const S2d pos, const double r, bool &etat_lecture)
{
    setposition(pos, etat_lecture);
    forme.set_centre(pos);
    if ((r_sca <= r) and (r < r_sca_repro))
    {
        forme.set_rayon(r);
    }
    else
    {
        cout << message::scavenger_radius_outside(r);
        etat_lecture = false;
    }
}

void Scavenger::setStatut_sca(const bool statut)
{
    if (statut)
    {
        statut_sca = EATING;
    }
    else
    {
        statut_sca = FREE;
    }
}

void Scavenger::setId_cible(const int id, Coraux const &coraux, bool &etat_lecture)
{
    bool similar_id = false;
    for (auto &corail_intra : coraux)
    {
        if (corail_intra.getId() == id)
        {
            similar_id = true;
        }
    }
    if (similar_id)
    {
        corail_id_cible = id;
    }
    else
    {
        cout << message::lifeform_invalid_id(id);
        etat_lecture = false;
    }
}

void Scavenger::dessin() const
{
    forme.dessin(1, 0, 0);
}
