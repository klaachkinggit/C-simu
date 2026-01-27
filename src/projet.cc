// Auteurs: Tahri Malik, Laachfoubi Kamil
// Nom du fichier: projet.cc
// version 1

#include <string>
#include <iostream>
#include <gtkmm/application.h>
#include "simulation.h"
#include "gui.h"

using namespace std;

int main(int argc, char *argv[])
{
	if (argc == 2)
	{
		string filename(argv[1]);
		auto app = Gtk::Application::create();
		Simulation donne;
		donne.lecture(filename);
		return app->make_window_and_run<MyEvent>(1, argv, donne);
	}
	else
	{
		string filename(("caca"));
		auto app = Gtk::Application::create();
		Simulation donne;
		donne.lecture(filename);
		return app->make_window_and_run<MyEvent>(1, argv, donne);
	}
}
