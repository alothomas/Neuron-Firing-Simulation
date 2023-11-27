#include "Simulation.h"
#include "constants.h"
#include "Random.h"

Random* _RNG;

int main(int argc, char **argv){
    try{
        Simulation s(argc, argv);
        s.run();
    }catch(Erreur& e){
        std::cerr << e.what() << std::endl;
        return e.value();
    }catch(TCLAP::ArgException &e){
        std::cerr << "Error: " << e.argId() << std::endl;
        return 1;
    }catch(std::logic_error &e){
        std::cerr << e.what() << std::endl;
        return 1;
    }

    if (_RNG) delete _RNG;
return 0;
}

