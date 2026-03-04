#include "../header/model.h"


Model::Model(Environment env): model{env.get_environment()} { }
Model::~Model() {model.end()}