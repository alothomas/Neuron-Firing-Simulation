#pragma once
#include <stdexcept>
#include <tclap/CmdLine.h>
#include <fstream>
#include <iostream>
#include <map>
#include <unordered_map>
#include <string>
#include <vector>

/*!
 @brief A class to manage errors in the program.
 
Heritates from a subclasse std::exception.
 Every class of errors has a specific output.
*/

class Erreur : public std::runtime_error {	
public:
    Erreur (const char *c, int v=0) : std::runtime_error(c), code(v) {}
    Erreur (const std::string &s, int v=0) : std::runtime_error(s), code(v) {}  
    int value() const {return code;}
    
protected:
    const int code;
};


/*!
 Default parameters
 */
#define _TIME_DEFAULT_ 500
#define _DELTAT_T_V_DEFAULT_ 0.5
#define _DELTAT_T_U_DEFAULT_ 1
#define _N_DEFAULT_ 1000
#define _PROPORTIONS_DEFAULT_ "RS:0.8, FS:0.2"
#define _P_EXCITER_DEFAULT_ 0.8
#define _CONN_MEAN_DEFAULT_ 4
#define _INTENSITY_CONN_DEFAULT_ 4.0
#define _DELTA_DEFAULT_ 0.2

/*!
 Description for TCLAP parameters
 */
#define _PRGRM_TEXT_ "Simulation of a neuron network"
#define _TIME_TEXT_ "Time of the simulation (in ms)"
#define _N_NEUR_TEXT_ "Number of neurons to simulate"
#define _PROPORTIONS_TEXT_ "Proportions of the different neurons (write RS:x , FS:y with x and y being the corresponding proportions )"
#define _P_EXCITER_TEXT_ "Proportion of excitatory neurons"
#define _CONN_MEAN_TEXT_ "Average connectivity"
#define _INTENSITY_CONN_TEXT_ "Average intensity of the connections"
#define _EXT_TEXT_ "Use of the extension"
#define _DELTA_TEXT_ "Noise of the parameters a,b,c,d"




