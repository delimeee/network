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

CCC = g++ -O0
CC  = gcc -O0
JAVAC = javac 

# ---------------------------------------------------------------------
# Compiler options 
# ---------------------------------------------------------------------

CCOPT = -m64 -O -fPIC -fno-strict-aliasing -fexceptions -DNDEBUG -DIL_STD
COPT  = -m64 -fPIC -fno-strict-aliasing
JOPT  = -classpath $(CPLEXDIR)/lib/cplex.jar -O

# ---------------------------------------------------------------------
# Link options and libraries
# ---------------------------------------------------------------------

CPLEXBINDIR   = $(CPLEXDIR)/bin/$(BINDIST)
CPLEXJARDIR   = $(CPLEXDIR)/lib/cplex.jar
CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)

CCLNDIRS  = -L$(CPLEXLIBDIR) -L$(CONCERTLIBDIR)
CLNDIRS   = -L$(CPLEXLIBDIR)
CCLNFLAGS = -lconcert -lilocplex -lcplex -lm -lpthread
CLNFLAGS  = -lcplex -lm -lpthread
JAVA      = java  -d64 -Djava.library.path=$(CPLEXDIR)/bin/x86-64_linux -classpath $(CPLEXJARDIR):

CONCERTINCDIR = $(CONCERTDIR)/include
CPLEXINCDIR   = $(CPLEXDIR)/include

EXDIR         = $(CPLEXDIR)/examples
EXINC         = $(EXDIR)/include
EXDATA        = $(EXDIR)/data
EXSRCC        = $(EXDIR)/src/c
EXSRCCX       = $(EXDIR)/src/c_x
EXSRCCPP      = $(EXDIR)/src/cpp
EXSRCJAVA     = $(EXDIR)/src/java

CFLAGS  = $(COPT)  -I$(CPLEXINCDIR)
CCFLAGS = $(CCOPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR) 
JCFLAGS = $(JOPT)

OBJ = ./object
BUILD = ./build
SRC = ./source
LIB = ./library
HEAD = ./header

all : init network

clean : 
	rm -f $(OBJ)/*
	rm -f $(BUILD)/*


init :
	mkdir -p $(OBJ) $(SRC) $(LIB) $(BUILD) $(HEAD)

network : $(OBJ)/network.o $(OBJ)/wgraph.o
	$(CCC) $(CCFLAGS) $(CCLNDIRS) $(OBJ)/wgraph.o $(OBJ)/network.o -o $(BUILD)/network

$(OBJ)/network.o : $(SRC)/network.cpp
	$(CCC) -c $(CCFLAGS) $(SRC)/network.cpp -o $(OBJ)/network.o

graph.o: $(SRC)/Graph/graph.cpp
	$(CCC) -c $(SRC)/Graph/graph.cpp -o $(OBJ)/graph.o

omodel.o: $(SRC)/OptimizationModel/omodel.cpp
	$(CCC) -c $(CCFLAGS) $(SRC)/OptimizationModel/omodel.cpp -o $(OBJ)/omodel.o






blend: blend.o
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -o blend blend.o $(CCLNFLAGS)
blend.o: $(EXSRCCPP)/blend.cpp
	$(CCC) -c $(CCFLAGS) $(EXSRCCPP)/blend.cpp -o blend.o