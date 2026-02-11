#include "simulation.h"
#include "lifeform.h"
#include "message.h"
#include "shape.h"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <locale>
#include <random>
#include <sstream>
#include <string>

using namespace std;

enum Etat_lecture {
  NB0,
  Etat_algue,
  NB1,
  Etat_corail,
  NB2,
  Etat_scavenger,
  Etat_segment,
  FIN
};

static Etat_lecture etat(NB0); // état initial
static int i(0), total(0), k(0), nb_seg(0);

static bool test_lecture(true);
static default_random_engine e;

void Simulation::verification_etat(bool test_lecture) {
  if (!test_lecture) {
    algues.clear();
    coraux.clear();
    scavengers.clear();
  } else {
    cout << message::success();
  }
}

void Simulation::lecture(string &nom_fichier) {
  algues.clear();
  coraux.clear();
  scavengers.clear();
  etat = NB0;
  i = 0, total = 0;
  k = 0, nb_seg = 0;
  test_lecture = true;
  e.seed(1);
  string line;
  ifstream fichier(nom_fichier);
  if (!fichier.fail()) {
    // l’appel de getline filtre aussi les séparateurs
    while (getline(fichier >> ws, line)) {
      // ligne de commentaire à ignorer, on passe à la suivante
      if (line[0] == '#')
        continue;
      decodage_ligne(line);
    }
  } else {
    cout << "erreur dans la lecture" << endl;
    test_lecture = false;
  }
  verification_etat(test_lecture);
}

void Simulation::decodage_ligne(string line) {
  istringstream data(line);

  switch (etat) {
  case NB0:
    if (!(data >> total))
      exit(EXIT_FAILURE);
    else
      i = 0;
    if (total == 0)
      etat = NB1;
    else
      etat = Etat_algue;
    break;

  case Etat_algue:
    ++i;
    {
      Algue algue(data, test_lecture);
      algues.push_back(algue);
      if (i == total)
        etat = NB1;
      if (test_lecture == false)
        etat = FIN;
    }
    break;

  case NB1:
    if (!(data >> total))
      exit(EXIT_FAILURE);
    else
      i = 0;
    if (total == 0)
      etat = NB2;
    else
      etat = Etat_corail;
    break;

  case Etat_corail:
    ++i;
    {
      Corail corail(data, coraux, test_lecture);
      coraux.push_back(corail);
      nb_seg = corail.getnb_segs();
      if (nb_seg == 0) {
        if (i == total)
          etat = NB2;
        // else stay in Etat_corail for the next coral
      } else {
        etat = Etat_segment;
      }
      k = 0;
      if (test_lecture == false)
        etat = FIN;
    }
    break;

  case Etat_segment:
    ++k;
    coraux[coraux.size() - 1].add_segment(data, coraux, false, test_lecture);
    if ((k == nb_seg) and (i < total))
      etat = Etat_corail;
    else if ((k == nb_seg) and (i == total))
      etat = NB2;
    if (test_lecture == false)
      etat = FIN;
    break;

  case NB2:
    if (!(data >> total))
      exit(EXIT_FAILURE);
    else
      i = 0;
    if (total == 0)
      etat = FIN;
    else
      etat = Etat_scavenger;
    break;

  case Etat_scavenger:
    ++i;
    {
      Scavenger scavenger(data, coraux, test_lecture);
      scavengers.push_back(scavenger);
      if (i == total)
        etat = FIN;
      if (test_lecture == false)
        etat = FIN;
    }
    break;

  case FIN:
    break;
  }
}

void Simulation::sauvegarde_fichier(ofstream &fichier) {
  if (!fichier) {
    cerr << "Impossible d'ouvrir le fichier pour l'écriture." << endl;
    return;
  }

  locale oldLocale = fichier.getloc();
  fichier.imbue(locale::classic()); // Utilisation de la localisation "C"

  algues_save(fichier);
  coraux_save(fichier);
  scavengers_save(fichier);

  // Restaurer la localisation précédente
  fichier.imbue(oldLocale);
}

void Simulation::algues_save(ofstream &fichier) {
  fichier << algues.size() << " " << endl;
  for (const auto &algue : algues) {
    fichier << algue.getPosition().x << " " << algue.getPosition().y << " "
            << algue.getAge() << " " << endl;
  }
}

void Simulation::coraux_save(ofstream &fichier) {
  fichier << coraux.size() << endl;
  for (const auto &corail : coraux) {
    fichier << corail.getPosition().x << " " << corail.getPosition().y << " "
            << corail.getAge() << " " << corail.getId() << " "
            << corail.getStatut_cor() << " " << corail.getDir_rot() << " "
            << corail.getStatut_dev() << " " << corail.getnb_segs() << " "
            << endl;

    for (const auto &seg : corail.getSegments()) {
      fichier << seg.getangle() << " " << seg.getlongueur() << endl;
    }
  }
}

void Simulation::scavengers_save(ofstream &fichier) {
  fichier << scavengers.size() << endl;
  for (const auto &scavenger : scavengers) {
    fichier << scavenger.getPosition().x << " " << scavenger.getPosition().y
            << " " << scavenger.getAge() << " " << scavenger.getRadius() << " "
            << scavenger.getStatut_sca() << " ";
    if (scavenger.getStatut_sca()) {
      fichier << scavenger.getCorail_id_cible() << endl;
    } else {
      fichier << " " << endl;
    }
  }
}

void Simulation::execution(bool checkbox) {
  execution_algues(checkbox);
  execution_coraux();
  execution_scavengers();
}
int Simulation::get_nbalgues() const { return algues.size(); }
int Simulation::get_nbcoraux() const { return coraux.size(); }
int Simulation::get_nbscavengers() const { return scavengers.size(); }

void Simulation::dessin() {
  for (const auto &algue : algues) {
    // cout << "Drawing algue at " << algue.getPosition().x << ", "
    //      << algue.getPosition().y << endl;
    algue.dessin();
  }

  for (const auto &corail : coraux) {
    corail.dessin();
  }
  for (const auto &scavenger : scavengers) {
    scavenger.dessin();
  }
}
void Simulation::update_algue_age() {
  bool bool_defaut(1);
  for (size_t i(0); i < algues.size(); i++) {
    bool_defaut = 1;
    algues[i].set_age(algues[i].getAge() + 1, max_life_alg, bool_defaut);
    if (!bool_defaut) {
      algues.erase(algues.begin() + i);
      i--;
    }
  }
  algues.shrink_to_fit();
}
void Simulation::update_corail_age() {
  for (size_t i(0); i < coraux.size(); i++) {
    bool bool_defaut = 1;
    coraux[i].set_age(coraux[i].getAge() + 1, max_life_cor, bool_defaut);
    if (!bool_defaut) {
      coraux[i].setStatut_cor(false);
    }
  }
  coraux.shrink_to_fit();
}
void Simulation::update_scavenger_age() {
  for (size_t i(0); i < scavengers.size(); i++) {
    bool bool_defaut(1);
    scavengers[i].set_age(scavengers[i].getAge() + 1, max_life_sca,
                          bool_defaut);
    if (!bool_defaut) {
      scavengers.erase(scavengers.begin() + i);
      i--;
    }
  }
  scavengers.shrink_to_fit();
}

void Simulation::execution_algues(bool checkbox) {
  update_algue_age();
  double p(alg_birth_rate);    // probabilité de la section 2.1.1, Annexe A
  bernoulli_distribution b(p); // booléen true avec probabilité p
  bool bool_defaut(1);

  if (b(e) and checkbox) {
    uniform_int_distribution<unsigned> u(1, dmax - 1);
    S2d pos;
    pos.x = u(e);
    pos.y = u(e);
    string temp = to_string(pos.x) + " " + to_string(pos.y) + " " + "1";
    istringstream val_defaut(temp);
    Algue new_algue(val_defaut, bool_defaut);
    algues.push_back(new_algue);
  }
}

void Simulation::execution_coraux() {
  update_corail_age();
  int nb_cor_pre_naissance = coraux.size();
  for (int i = 0; i < nb_cor_pre_naissance; i++) {
    Segments segs_copie = coraux[i].getSegments();
    if (coraux[i].getStatut_cor()) {
      if (segs_copie[segs_copie.size() - 1].getlongueur() < l_repro) {
        exec_cor_alive_rotate(coraux[i]);
      }
      segs_copie = coraux[i].getSegments();
      if (segs_copie[segs_copie.size() - 1].getlongueur() >= l_repro) {
        exec_cor_alive_repro_extend(coraux[i]);
      }
    }
  }
}

bool Simulation::algue_near(S2d &pos, Corail const &cor, Segment const &seg) {
  S2d pos_near;
  bool distance_ok(false);
  double angle;
  int k(0);
  for (size_t i(0); i < algues.size(); i++) {
    if (distance(seg.getbase(), algues[i].getPosition()) <= seg.getlongueur()) {
      double angle_i = calcul_angle_2vect(
          seg.getbase(), algues[i].getPosition(), seg.getend());
      if ((cor.getDir_rot() and angle_i < 0) or
          (!cor.getDir_rot() and angle_i > 0)) {
        if (k == 0 || abs(angle_i) < abs(angle)) {
          angle = angle_i;
          pos_near = algues[i].getPosition();
          distance_ok = true;
          k = 1;
        }
      }
    }
  }
  pos = pos_near;
  return distance_ok;
}

void Simulation::adjustAngle(double &angle, bool dir_rot) {
  double adjustment = dir_rot ? 2 * M_PI : -2 * M_PI;
  if ((dir_rot && angle < -M_PI) || (!dir_rot && angle > M_PI)) {
    angle += adjustment;
  }
}

void Simulation::removeAlgueAtPosition(const S2d &pos_near) {
  for (size_t i(0); i < algues.size(); i++) {
    if ((algues[i].getPosition().x == pos_near.x) and
        (algues[i].getPosition().y == pos_near.y)) {

      algues.erase(algues.begin() + i);
      algues.shrink_to_fit();
      break;
    }
  }
}

void Simulation::adjustSegment(Segment &effecteur, const double &angle_near,
                               bool presence, bool dir_rot, double delta_rot,
                               double delta_l, ostringstream &oss) {
  double val = effecteur.getangle();
  if (abs(angle_near) <= delta_rot && presence) {
    val += angle_near;
    effecteur.set_longueur(effecteur.getlongueur() + delta_l);
  } else {
    val += dir_rot ? -delta_rot : delta_rot;
  }
  adjustAngle(val, dir_rot);
  oss << val << " " << effecteur.getlongueur() << " ";
}

void Simulation::exec_cor_alive_rotate(Corail &cor) {
  bool test_superposition_collision = true;
  Corail cor_copie = cor;
  Segments copie_segs = cor.getSegments();
  Segment copie_effecteur = copie_segs.back();

  S2d pos_near = {0, 0};
  bool presence = algue_near(pos_near, cor, copie_effecteur);
  double angle_near = calcul_angle_2vect(copie_effecteur.getbase(), pos_near,
                                         copie_effecteur.getend());

  ostringstream oss;
  adjustSegment(copie_effecteur, angle_near, presence, cor.getDir_rot(),
                delta_rot, delta_l, oss);

  copie_segs.pop_back();
  cor_copie.setSegments(copie_segs);
  string seg_data = oss.str();
  istringstream seg_test(seg_data);
  cor_copie.add_segment(seg_test, coraux, true, test_superposition_collision,
                        cor_copie.getId());
  cor_copie.verification_collision_rebondir(copie_effecteur,
                                            test_superposition_collision);

  if (!test_superposition_collision) {
    cor.setDir_rot(!cor.getDir_rot());
  } else {
    cor = cor_copie;
    if (abs(angle_near) <= delta_rot && presence) {
      removeAlgueAtPosition(pos_near);
    }
  }
}

void Simulation::exec_cor_alive_repro_extend(Corail &cor) {
  Segments segs_copie = cor.getSegments();
  Segment copie_effecteur = segs_copie.back();
  if (!cor.getStatut_dev()) {
    segs_copie[segs_copie.size() - 1].set_longueur(l_seg_interne);
    Segment seg(segs_copie[segs_copie.size() - 1].getend(),
                segs_copie[segs_copie.size() - 1].getangle(),
                l_repro - l_seg_interne);
    segs_copie.push_back(seg);
    cor.setSegments(segs_copie);
    cor.setStatut_dev(true);
  } else {
    segs_copie[segs_copie.size() - 1].set_longueur(l_repro / 2);
    cor.setSegments(segs_copie);
    cor.setStatut_dev(false);

    creation_bb_corail(copie_effecteur);
  }
}

void Simulation::creation_bb_corail(Segment const &copie_effecteur) {
  string init_corail(" ");
  init_corail +=
      to_string(copie_effecteur.getend().x -
                cos(copie_effecteur.getangle()) * (l_repro - l_seg_interne)) +
      " ";
  init_corail +=
      to_string(copie_effecteur.getend().y -
                sin(copie_effecteur.getangle()) * (l_repro - l_seg_interne)) +
      " ";
  init_corail += "1 ";
  init_corail += to_string(coraux[coraux.size() - 1].getId() + 1) + " ";
  init_corail += "1 1 0 1";

  istringstream s(init_corail);
  bool init(1);
  Corail baby_cor(s, coraux, init);
  coraux.push_back(baby_cor);
  string init_segment(" ");
  init_segment += to_string(copie_effecteur.getangle()) + " ";
  init_segment += to_string(l_repro - l_seg_interne) + " ";
  istringstream s_1(init_segment);
  coraux[coraux.size() - 1].add_segment(s_1, coraux, true, init);
}

void Simulation::deplacement_scav(Scavenger &scavenger, double angle_scav_cible,
                                  double distance) {
  bool bool_defaut(1);
  double nvl_position_x(scavenger.getPosition().x +
                        cos(angle_scav_cible) * distance);
  double nvl_position_y(scavenger.getPosition().y +
                        sin(angle_scav_cible) * distance);
  S2d nvl_position = {nvl_position_x, nvl_position_y};
  scavenger.setposition(nvl_position, bool_defaut);
}
void Simulation::creation_bb_scav(Scavenger const &scavenger,
                                  double const angle_scav_cible) {
  string init_scav(" ");
  init_scav += to_string(scavenger.getPosition().x +
                         cos(angle_scav_cible + M_PI) * delta_l) +
               " ";
  init_scav += to_string(scavenger.getPosition().y +
                         sin(angle_scav_cible + M_PI) * delta_l) +
               " ";
  init_scav += "1 ";
  init_scav += to_string(r_sca) + " ";
  init_scav += "0";

  istringstream s(init_scav);
  bool init(1);
  Scavenger baby_scav(s, coraux, init);
  scavengers.push_back(baby_scav);
}

void Simulation::execution_scavengers() {
  update_scavenger_age();
  vector<int> ids;
  size_t k = 0;
  for (auto &corail : coraux) {
    if (corail.getStatut_cor()) {
      ++k;
    }
  }
  for (const auto &scavenger : scavengers) {
    if (scavenger.getStatut_sca()) {
      ids.push_back(scavenger.getCorail_id_cible());
    }
  }
  int nb_sca_pre_naissance = scavengers.size();
  if ((coraux.size() != 0) and ((k != coraux.size()))) {
    for (int i = 0; i < nb_sca_pre_naissance; i++) {
      if (!(scavengers[i].getStatut_sca())) {
        exec_scav_free(scavengers[i], ids);
      } else {
        exec_scav_mange(scavengers[i]);
      }
    }
  }
}

void Simulation::find_nearest_available_coral(const Scavenger &scavenger,
                                              const vector<int> &ids,
                                              S2d &position_corail_proche,
                                              int &id_cible_temp,
                                              int &compteur) {
  for (auto &corail : coraux) {
    if (!corail.getStatut_cor() &&
        find(ids.begin(), ids.end(), corail.getId()) == ids.end()) {
      S2d position_corail_disp = corail.getSegments().back().getend();
      if (compteur == 0 ||
          distance(position_corail_disp, scavenger.getPosition()) <
              distance(position_corail_proche, scavenger.getPosition())) {
        position_corail_proche = position_corail_disp;
        id_cible_temp = corail.getId();
        compteur++;
      }
    }
  }
}

void Simulation::exec_scav_free(Scavenger &scavenger, std::vector<int> &ids) {
  S2d position_corail_proche = {scavenger.getPosition().x,
                                scavenger.getPosition().y};
  int compteur(0);
  int id_cible_temp;
  find_nearest_available_coral(scavenger, ids, position_corail_proche,
                               id_cible_temp, compteur);

  double angle_scav_cible(
      atan2(position_corail_proche.y - scavenger.getPosition().y,
            position_corail_proche.x - scavenger.getPosition().x));
  double distance_scav_cible(
      distance(position_corail_proche, scavenger.getPosition()));
  bool bool_defaut(1);
  if (distance_scav_cible > delta_l) {
    deplacement_scav(scavenger, angle_scav_cible, delta_l);
    scavenger.setForme(scavenger.getPosition(), scavenger.getRadius(),
                       bool_defaut);
  } else if (distance_scav_cible) {
    deplacement_scav(scavenger, angle_scav_cible, distance_scav_cible);
    scavenger.setForme(scavenger.getPosition(), scavenger.getRadius(),
                       bool_defaut);
    scavenger.setStatut_sca(true);
    scavenger.setId_cible(id_cible_temp, coraux, bool_defaut);
    ids.push_back(scavenger.getCorail_id_cible());
  }
}
void Simulation::scav_developpement(Scavenger &scavenger,
                                    double angle_scav_cible, bool bool_defaut) {
  if (scavenger.getRadius() + delta_r_sca >= r_sca_repro) {
    scavenger.setForme(scavenger.getPosition(), r_sca, bool_defaut);
    creation_bb_scav(scavenger, angle_scav_cible);
  } else {
    scavenger.setForme(scavenger.getPosition(),
                       scavenger.getRadius() + delta_r_sca, bool_defaut);
  }
}

void Simulation::exec_scav_mange(Scavenger &scavenger) {
  int pos_corail(0);
  bool bool_defaut(1);
  for (auto &corail : coraux) {
    if (scavenger.getCorail_id_cible() == corail.getId())
      break;
    else
      ++pos_corail;
  }

  if (pos_corail >= (int)coraux.size()) {
    scavenger.setStatut_sca(false);
    return;
  }
  Segments segs_copie = coraux[pos_corail].getSegments();
  Segment last_seg(segs_copie[segs_copie.size() - 1]);
  double delta_longueur =
      (last_seg.getlongueur() < delta_l) ? last_seg.getlongueur() : delta_l;
  last_seg.set_longueur(last_seg.getlongueur() - delta_longueur);
  segs_copie.pop_back();
  segs_copie.push_back(last_seg);
  coraux[pos_corail].setSegments(segs_copie);
  double angle_scav_cible(last_seg.getangle() + M_PI);
  if (last_seg.getlongueur() != 0) {
    deplacement_scav(scavenger, angle_scav_cible, delta_longueur);
  } else {
    segs_copie.pop_back();
    if (!segs_copie.size()) {
      coraux.erase(coraux.begin() + pos_corail);
      scavenger.setStatut_sca(false);
    } else {
      coraux[pos_corail].setSegments(segs_copie);
    }
    deplacement_scav(scavenger, angle_scav_cible, delta_longueur);
  }
  scav_developpement(scavenger, angle_scav_cible, bool_defaut);
}
