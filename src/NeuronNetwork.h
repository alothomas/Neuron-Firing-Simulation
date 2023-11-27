#pragma once
#include "Neuron.h"
#include "Random.h"
#include "constants.h"


/*! @class  NeuronNetwork
 
@brief The NeuronNetwork class allows us to modelize a network of Neurons that interact with each other.
 
 The network of Neuron is initialized with a vector of pointers of Neuron, a mean connectivity and a mean intensity of connection.
Once initialized, the NeuronNetwork creates the connections between each Neuron. Every Neuron is linked to a random number d(n) of Neuron with a random intensity.
Also, the NeuronNetwork is updated as time goes on.
 */

class NeuronNetwork {

public:
    /** @name Constructors
     */

    ///@{

    /**
     Default constructor
     */
    NeuronNetwork();
    
    /**
     Constructor that initializes the network of Neuron
     @param neurons Vector of Neuron created in Simulation
     @param lambda Mean connectivity of the network
     @param L Mean intensity of connection
     */
    NeuronNetwork(std::vector<Neuron*> neurons, int lambda, double L);

    /**
     * Constructor that initializes the network of Neuron
     * @param nb_neur Number of Neurons in the network
     * @param prop_exciter Proportion of excitatory Neuron in the network
     * @param conn_mean Mean connectivity of the network
     * @param intensity_conn Mean intensity of connection
     * @param p Map that distributes the types of Neuron in the network
     * @param ext Boolean that tells if the extension was activated or not
     * @param delta Tunable parameter for the noise extension
     */
    NeuronNetwork(double nb_neur, double prop_exciter, int conn_mean, double intensity_conn, const std::map<std::string, double>& p, bool ext=false, double delta=0.2);
    ///@}

    ~NeuronNetwork();

    /** @name Getters
     Those getters allow us to access the private elements of NeuronNetwork in classes such as Neuron or Simulation. Some of them compute mean values of Neuron parameters.
     */
    
    ///@{
    
    /**
     @param id Identifier of the Neuron
     @return Pointer on a Neuron knowing its id
     */
    Neuron* getNeuron(int id) const;
    
    /**
     Method mostly used in MainTest to access the vector of Neuron
     */
    std::vector<Neuron*> getNeurons() const;
    
    /**
     @param id
     @return value v for a specific Neuron
     */
    double getPot(int id) const;

    /**
     @param id
     @return Valence for a specific Neuron
     */
    double getValence(int id) const;

    
    /**
     @param id
     @return value u for a specific Neuron
     */
    double getRelax(int id) const;
    
    ///@}
    
    /** @name Creation and manipulation of the Neuron network
     */
    
    ///@{
    
    /**
     Methode that creates connections for each Neuron in the network.
     Randon number d of connection for each Neuron with a random intensity l.
     d and l are created with methods of the class Random.
     */
    void set_connections();
    
    /**
     Computes the value of the synaptic current of a specific Neuron knowing its Id.
     Takes into account the number of inhibitory and excitatory Neurons linked to this Neuron and if they are 'firing'.
     @param id
     */
    void I(int id);
    
    /**
     Update the network of Neuron
     @param delta_v the interval of time for the potential : 0.5
     @param delta_u the interval of time for the relaxation : 1.0
     */
    void update(double delta_v, double delta_u);
    
    /**
     Computes temporal evolution of v(t)
     @param u
     @param v
     @param I
     @param delta the interval of time for the potential
     */
    double eqDiffSolverV(double u, double v, double I, double delta);
    
    /**
     Computes temporal evolution of u(t)
     @param u
     @param v
     @param a
     @param b
     @param delta the interval of time for the relaxation
     */
    double eqDiffSolverU(double u, double v, double a, double b, double delta) ;
    
    ///@}


private:

    /**
     Method that creates Ids for each Neuron of the NeuronNetwork
     */
    void generateIds();

    double lambda_;
    double L_;
    std::vector<Neuron*> neurons;

};

