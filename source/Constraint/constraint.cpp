#include "constraint.h"

// class Constraint {
//     public:
//         virtual IloExpr load();
// };

// class ConnectivityConstraint: Constraint { 
    
//     public:
//         ConnectivityConstraint(const std::vector<size_t>);
// };

// class SurvivableConstraint: Constraint { 
//     public:
//         SurvivableConstraint(const std::vector<size_t>);
// };

SurvivableConstraint::SurvivableConstraint(std::vector<size_t>& vars) {

}

SurvivableConstraint::load() {
    return constraint;
}