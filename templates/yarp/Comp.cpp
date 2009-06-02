#include "<!comp.name()!>Module.hpp"

using namespace std;

int main(int argc, char *argv[]) 
{
    yarp::os::Network yarp;
    <!comp.name()!>Module m;
    return m.runModule(argc, argv);
}


