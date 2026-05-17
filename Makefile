SYSTEM     = x86-64_linux
LIBFORMAT  = static_pic

#------------------------------------------------------------
#
# Please set CPLEXDIR and CONCERTDIR to
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

STD_FLAGS = -std=c++23 -c
OBJ_FLAGS = $(STD_FLAGS) $(CCFLAGS)

all : init network

clean : 
	rm -f $(OBJ)/*
	rm -f $(BUILD)/*


init :
	mkdir -p $(OBJ) $(SRC) $(BUILD)

run :
	cat ./input/main.txt | ./build/network
	python draw.py

network : network.o graph.o omodel.o oengine.o oanalyzer.o fmodel.o
	$(CCC) $(CCFLAGS) $(CCLNDIRS) $(OBJ)/oanalyzer.o $(OBJ)/graph.o $(OBJ)/network.o  $(OBJ)/omodel.o $(OBJ)/oengine.o $(OBJ)/fmodel.o -o $(BUILD)/network $(CCLNFLAGS)

network.o : $(SRC)/network.cpp
	$(CCC) $(OBJ_FLAGS) $(SRC)/network.cpp -o $(OBJ)/network.o

graph.o: $(SRC)/Graph/graph.cpp
	$(CCC) $(STD_FLAGS) $(SRC)/Graph/graph.cpp -o $(OBJ)/graph.o

omodel.o: $(SRC)/OptimizationModel/omodel.cpp
	$(CCC) $(OBJ_FLAGS) $(SRC)/OptimizationModel/omodel.cpp -o $(OBJ)/omodel.o

oengine.o: $(SRC)/OptimizationEngine/oengine.cpp
	$(CCC) $(OBJ_FLAGS) $(SRC)/OptimizationEngine/oengine.cpp -o $(OBJ)/oengine.o

oanalyzer.o: $(SRC)/GraphAnalyzer/ganalyzer.cpp
	$(CCC) $(OBJ_FLAGS) $(SRC)/GraphAnalyzer/ganalyzer.cpp -o $(OBJ)/oanalyzer.o

fmodel.o: $(SRC)/FlowModel/fmodel.cpp
	$(CCC) $(OBJ_FLAGS) $(SRC)/FlowModel/fmodel.cpp -o $(OBJ)/fmodel.o
