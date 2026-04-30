#ifndef LIN_CONSTRAINT_H
#define LIN_CONSTRAINT_H
#include <ilcplex/ilocplex.h>
#include <unordered_set>
#include <vector>

class Constraint {
    private:
        IloExpr constraint;
    public:
        virtual IloExpr load();
        Constraint();
        ~Constraint();
        Constraint(const Constraint&) = delete;
        Constraint& operator=(const Constraint& op) = delete;
};

class ConnectivityConstraint: Constraint { 
    
    public:
        ConnectivityConstraint(const std::unordered_set<size_t>, double);

};

class SurvivableConstraint: Constraint { 
    public:
        SurvivableConstraint(const std::unordered_set<size_t>, double);
};


#endif