#pragma once
#include "constants.h"
#include "Random.h"
#include "Neuron.h"
#include "NeuronNetwork.h"

/*!
  @class Simulation
  @brief The Simulation class is the main class in this program. It constructs the neuron network \ref NeuronNetwork according to user-specified parameters, and @ref run "runs" the simulation.

  Simulation results are then printed to 3 \ref outfiles_ files by \ref print.

  A Simulation is made of a neuron network \ref NeuronNetwork. The \ref NeuronNetwork initializes itself with the given parameters.

  Time (\ref time_) advances from 0 until it reaches \ref time_max_. The simulation is updated every \ref delta_t_V  and the results are printed every \ref delta_t_U with @ref print.
*/

class Simulation {

public:
    /*! @name Initialization
     * With the parameters, the constructor initializes the @ref network_ the different @ref types_ and the @ref outfiles_ files.
    */
///@{
/*!
 * \brief Constructor which takes the user inputs from the command line
 * \param argc
 * \param argv
 */
Simulation(int argc, char** argv);
~Simulation();
///@}

/*! @name Running the simulation
  The simulation is performed by calling \ref headers once, then \ref run, which calls \ref print at \ref delta_t_U time intervals.
*/
///@{
void headers();
void run();
void print();
///@}


/*! @name Output management
 * These methods create the output files and print in them.
 */
///@{

/*!
 * \brief Initializes the \ref outfiles_ by naming and opening them.
 */
void createFiles();
/*!
 * \brief Prints the firing state of the neurons of the \ref network_
 */
void print_spikes();
/*!
 * \brief Prints the inititial parameters of the neurons of \ref network_
 */
void print_parameters();
/*!
 * \brief Prints the average potential, current and relaxation potential of each \ref types_ of neuron of the network.
 */
void print_samples();
///@}

/*!
 * \brief Extracts the types of neurons and the proportions given in the command line.
 * It is only called if the user specifies the types of neurons to simulate.
 * \return a map containing the types of neurons and their proportions given by the user
 */
std::map<std::string, double> test_read_proportions(std::string);

/*!
 * \brief Returns the neurons of the simulated network.
 * \return std::vector<Neuron*> the neurons of the network
 */
std::vector<Neuron*> getNetwork() const;

private:

    /*!
     Displays a progress bar
     */
void progressBar(double progress, double width) const;

    /*!
     Simulated NeuronNetwork
     */
NeuronNetwork* network_;
    
    /*!
     Stores the types of neurons to simulate
     */
std::vector<std::string> types_;
    /*!
     Attributes relative to time
     */
double time_, delta_t_V, delta_t_U, time_max_;
    
    /*!
     Output files
     */
std::vector<std::ofstream> outfiles_;
    
    /*!
     Only one Neuron of each types_ taken randomly
     */
std::vector<Neuron*> sample_; 

};



