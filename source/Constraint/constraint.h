#ifndef LIN_CONSTRAINT_H
#define LIN_CONSTRAINT_H
#include <ilcplex/ilocplex.h>
#include <vector>

class Constraint {
    private:
        IloExpr constraint;
    public:
        virtual IloExpr load();
};

class ConnectivityConstraint: Constraint { 
    
    public:
        ConnectivityConstraint(const std::vector<size_t>);
};

class SurvivableConstraint: Constraint { 
    public:
        SurvivableConstraint(const std::vector<size_t>);
};


#endif