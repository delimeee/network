#include"../header/environment.h"


Environment::Environment(): env{} model()

Environment::~Environment() {
    env.end();
}