// Auteurs: Tahri Malik, Laachfoubi Kamil
// Nom du fichier: simulation.h
// version 1
#ifndef simulation_H
#define simulation_H

#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "lifeform.h"

typedef std::basic_ofstream<char> ofstream;

class Simulation
{
private:
    void decodage_ligne(std::string line);

    void execution_algues(bool checkbox);
    void execution_coraux();
    void execution_scavengers();

    void update_algue_age();
    void update_corail_age();
    void update_scavenger_age();
    void exec_cor_alive_rotate(Corail &cor);
    void exec_cor_alive_repro_extend(Corail &cor);
    void creation_bb_corail(Segment const &copie_effecteur);
    bool algue_near(S2d &pos, Corail const &cor, Segment const &seg);
    void removeAlgueAtPosition(const S2d &pos_near);
    void adjustAngle(double &angle, bool dir_rot);
    void adjustSegment(Segment &effecteur, const double &angle_near, bool presence,
                       bool dir_rot, double delta_rot,
                       double delta_l, std::ostringstream &oss);
    void exec_scav_mange(Scavenger &scavenger);
    void exec_scav_free(Scavenger &scavenger, std ::vector<int> &ids);
    void find_nearest_available_coral(const Scavenger &scavenger,
                                      const std::vector<int> &ids,
                                      S2d &position_corail_proche,
                                      int &id_cible_temp, int &compteur);
    void deplacement_scav(Scavenger &scavenger, double angle_scav_cible,
                          double distance);
    void creation_bb_scav(Scavenger const &scavenger, double const angle_scav_cible);
    void scav_developpement(Scavenger &scavenger , double angle_scav_cible, 
                             bool bool_defaut);
    void algues_save(ofstream &fichier);
    void coraux_save(ofstream &fichier);
    void scavengers_save(ofstream &fichier);
    
    Algues algues;

    Coraux coraux;

    Scavengers scavengers;

public:
    void lecture(std::string &nom_fichier);
    void verification_etat(bool etat_lecture);
    void sauvegarde_fichier(ofstream &fichier);
    

    void execution(bool checkbox);
    int get_nbalgues() const;
    int get_nbcoraux() const;
    int get_nbscavengers() const;
    void dessin();
};

#endif
