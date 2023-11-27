#ifndef NEURONE_H
#define NEURONE_H

#include "Random.h"
#include "constants.h"

/*! @class Neuron
 @brief Creates a Neuron
 
 A Neuron is characterized by its 'inhibitory' or 'excitatory' quality.
 We assigned an identifier id for each Neuron.
 There are 5 types of Neuron : RS, FS, IB, CH and LTS.
 A Neuron is also characterized by a few parameters :
 @param a
 @param b
 @param c
 @param d
 @param u Evolutionary parameter : the relaxation
 @param v Evolutionary parameter : the membrane's potential

 */


class Neuron{
public :
/*! @name Initialization
 * A Neuron is initialized by its type and its identifier.
 * The constructor then initializes every other parameters.
 * @param _id
 * @param _type
 */
///@{ 
    Neuron(std::string _type, int id=0, bool ext=false, double delta=0.2);
    Neuron(Neuron* n);
    ///@}
    
	~Neuron();


/*! @name Manage the Neuron attributes
 */
///@{
	void setFiring(bool _f);
	void setPotential(double _v);
	void setRelaxation(double _u);
    bool isFiring();
    void setQuality(bool value);
    void setId(int i);
    void setI(double I);

///@}
    
    /*! @name Getters
     */
    ///@{
    bool getQuality() const;
    double getA() const;
    double getB() const;
    double getV() const;
    double getU() const;
    double getC() const;
    double getD() const;
    int getId() const;
    double getI()const;
    std::string getType() const;
    std::unordered_map<int,double> getConnections() const;  
    ///@}
    
    /*!
     Adds a connection to the map of connections of the Neuron.
     Used in NeuronNetwork.
     */
    void addConnection(std::pair<int,double> co);
    
    /*!
     Verifies if a Neuron is already connected to this Neuron or has the same Id as *this.
     */
    bool checkConnection(int id) const;
    
    /*!
     Sets the initial attributs of the neuron (a,b,c,d and quality) depending on its type.
     */
    void setinitialattributs(std::string type);
    

private :
    
    /*!
     Modifies the attributs a,b,c,d of the neuron depending on its type and on whether we are running the extension or not.
     */
    void setvariations(std::string type,bool ext=false,double delta=0.2);
	
    bool quality;
	std::string type;
	bool firing;
    int  id;
    double v;
    double a;
	double b;
	double c;
	double d;
	double u;
    std::unordered_map<int,double> connections;
    double current;

};
	
#endif	

