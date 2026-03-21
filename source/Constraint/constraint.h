#ifndef LIN_CONSTRAINT_H
#define LIN_CONSTRAINT_H
#include <ilcplex/ilocplex.h>


class Constraint {
    public:
        virtual IloExpr load();
};

class ConnectivityConstraint: Constraint { };
class SurvivableConstraint: Constraint { };


#endif