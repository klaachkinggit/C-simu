// Auteurs: Tahri Malik, Laachfoubi Kamil
// Nom du fichier: lifeform.h
// version 1

#ifndef LIFEFORM_H
#define LIFEFORM_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include "shape.h"
#include "constantes.h"

class Lifeform;
class Algue;
class Corail;
class Scavenger;

typedef std::vector<Corail> Coraux;

typedef std::vector<Scavenger> Scavengers;

typedef std::vector<Algue> Algues;

typedef std::vector<Segment> Segments;

class Lifeform
{
protected:
    S2d position;
    unsigned int age;

public:
    S2d getPosition() const;
    unsigned int getAge() const;
    void setposition(S2d pos, bool &etat_lecture);
    void set_age(const int a, const int age_max, bool &etat_lecture);
};

class Algue : public Lifeform
{
public:
    Algue(std::istringstream &donnee, bool &etat_lecture);
    ~Algue();
    void dessin() const;

private:
    Cercle forme;
};

class Corail : public Lifeform
{
public:
    Corail(std::istringstream &donnee, Coraux const &coraux, bool &etat_lecture);

    Segments getSegments() const;
    int getId() const;
    bool getStatut_cor() const;
    bool getDir_rot() const;
    bool getStatut_dev() const;
    int getnb_segs() const;
    ~Corail();

    void add_segment(std::istringstream &donne, Coraux const &coraux,
                     bool epsil, bool &etat_lecture, int id = 0);
    void setSegments(Segments const seg);
    void setId(const int id, Coraux const &coraux, bool &etat_lecture);
    void setStatut_cor(const bool statut);
    void setDir_rot(const bool dir);
    void setStatut_dev(const bool statut);
    void dessin() const;

    void verification_collision_rebondir(Segment const &copie_effecteur, bool &etat_lecture);

private:
    Carre base;
    Segments segments;
    int id;
    Status_cor statut_cor;
    Dir_rot_cor dir_rot;
    Status_dev statut_dev;
    int nb_segs;

    void verification_collision(Segment const &seg, Coraux const &coraux,
                                bool epsil, bool &etat_lecture, int id = 0);
    bool superposition_collision(Segment const &seg, Coraux const &coraux,
                                 bool epsil, bool &etat_lecture, int id);
    void check_segment_collision(const Segment &seg,
                                 const std::vector<Segment> &autres_segments,
                                 int coral_id, bool epsil,
                                 bool &etat_lecture);
};

class Scavenger : public Lifeform
{
public:
    Scavenger(std::istringstream &donne, Coraux const &coraux, bool &etat_lecture);
    double getRadius() const;
    bool getStatut_sca() const;
    int getCorail_id_cible() const;
    void setForme(const S2d pos, const double r, bool &etat_lecture);
    void setStatut_sca(const bool statut);
    void setId_cible(const int id, Coraux const &coraux, bool &etat_lecture);
    ~Scavenger();
    void dessin() const;

private:
    Cercle forme;
    Status_sca statut_sca;
    int corail_id_cible;
};

#endif
