## Auteurs: Tahri Malik, Laachfoubi Kamil
## Nom du fichier: Makefile
## version 2 (src/include/build)

OUT = projet
CXX = g++
CXXFLAGS = -Wall -std=c++17 -Iinclude
LINKING = `pkg-config --cflags gtkmm-4.0`
LDLIBS  = `pkg-config --libs gtkmm-4.0`

SRCDIR = src
INCDIR = include
BUILDDIR = build

CXXFILES = graphic.cc shape.cc lifeform.cc simulation.cc projet.cc gui.cc message.cc
SRC = $(addprefix $(SRCDIR)/,$(CXXFILES))
OFILES = $(patsubst $(SRCDIR)/%.cc,$(BUILDDIR)/%.o,$(SRC))

all: $(OUT)

$(OUT): $(OFILES)
	$(CXX) $(CXXFLAGS) $(LINKING) $(OFILES) -o $@ $(LDLIBS)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cc | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@

depend:
	@echo " *** MISE A JOUR DES DEPENDANCES ***"
	@(sed '/^# DO NOT DELETE THIS LINE/q' Makefile && \
	  $(CXX) -MM $(CXXFLAGS) $(LINKING) $(SRC) | \
	  egrep -v "/usr" | \
	  sed 's|^\([a-zA-Z0-9_]*\)\.o:|$(BUILDDIR)/\1.o:|' \
	 ) > Makefile.new
	@mv Makefile.new Makefile

clean:
	@echo " *** EFFACE MODULES OBJET ET EXECUTABLE ***"
	@/bin/rm -rf $(BUILDDIR) *.x $(OUT)
	@/bin/rm -f $(SRCDIR)/*.cc~ $(INCDIR)/*.h~

.PHONY: all clean depend

# DO NOT DELETE THIS LINE
