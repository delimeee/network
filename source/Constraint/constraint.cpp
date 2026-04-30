#include "constraint.h"


Constraint::~Constraint() {
    constraint.end();
}

SurvivableConstraint::SurvivableConstraint(const std::unordered_set<size_t>, double) {
    
}

SurvivableConstraint::load() {
    return constraint;
}