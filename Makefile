SYSTEM     = x86-64_linux
LIBFORMAT  = static_pic

#------------------------------------------------------------
#
# When you adapt this makefile to compile your CPLEX programs
# please copy this makefile and set CPLEXDIR and CONCERTDIR to
# the directories where CPLEX and CONCERT are installed.
#
#------------------------------------------------------------

CPLEXDIR      = ../../programs/cplex/cplex/
CONCERTDIR    = ../../programs/cplex/concert/
# ---------------------------------------------------------------------
# Compiler selection 
# ---------------------------------------------------------------------

CCC = g++

# ---------------------------------------------------------------------
# Compiler options 
# ---------------------------------------------------------------------

CCOPT = -m64 -fPIC -fno-strict-aliasing -fexceptions -DNDEBUG
COPT  = -m64 -fPIC
JOPT  = -classpath $(CPLEXDIR)/lib/cplex.jar -O

# ---------------------------------------------------------------------
# Link options and libraries
# ---------------------------------------------------------------------

CPLEXBINDIR   = $(CPLEXDIR)/bin/$(BINDIST)
CPLEXJARDIR   = $(CPLEXDIR)/lib/cplex.jar
CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)

# For dynamic linking
CPLEXBINDIR   = $(CPLEXDIR)/bin/$(SYSTEM)
CPLEXLIB      = cplex$(dynamic:yes=2212)
run           = $(dynamic:yes=LD_LIBRARY_PATH=$(CPLEXBINDIR))

CCLNDIRS  = -L$(CPLEXLIBDIR) -L$(CONCERTLIBDIR) $(dynamic:yes=-L$(CPLEXBINDIR))
CCLNFLAGS = -lconcert -lilocplex -l$(CPLEXLIB) -lm -lpthread -ldl

CONCERTINCDIR = $(CONCERTDIR)/include
CPLEXINCDIR   = $(CPLEXDIR)/include

OBJ = ./objects
BUILD = ./build
SRC = ./src

CCFLAGS = $(CCOPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR) 

all : clean elnet commi fake

clean : 
	rm -f $(OBJ)/*
	rm -f $(BUILD)/*

elnet: cplex_code_202502.o
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -o $(BUILD)/elnet $(OBJ)/cplex_code_202502.o $(CCLNFLAGS)

cplex_code_202502.o: $(SRC)/cplex_code_202502.cpp
	$(CCC) -c $(CCFLAGS) $(SRC)/cplex_code_202502.cpp -o $(OBJ)/cplex_code_202502.o

commi : commi.obj
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -o $(BUILD)/commi $(OBJ)/commi.o $(CCLNFLAGS)

commi.obj : $(SRC)/commi.cpp
	$(CCC) -c $(CCFLAGS) $(SRC)/commi.cpp -o $(OBJ)/commi.o

fake : $(SRC)/fake_data_gen.cpp
	$(CCC) $(SRC)/fake_data_gen.cpp -o $(BUILD)/fake

graph : 
	dot -Tpng graph.dot -o graph.png

run :
	./build/fake > ./Data/Cities.txt
	./build/commi > runtime.log
	dot -Tpng graph.dot -o graph.png