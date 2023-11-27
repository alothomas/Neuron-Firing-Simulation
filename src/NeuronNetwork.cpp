#include "NeuronNetwork.h"
#include "constants.h"


NeuronNetwork::NeuronNetwork() {}

NeuronNetwork::NeuronNetwork(std::vector<Neuron*> neurons, int lambda, double L)
	: lambda_(lambda), L_(L), neurons(neurons) {}

NeuronNetwork::NeuronNetwork(double nb_neur, double prop_exciter, int conn_mean, double intensity_conn, const std::map<std::string, double>& p, bool ext, double delta)
    :lambda_(conn_mean), L_(intensity_conn)
{
    if(prop_exciter!=-1){
		int nb_RS(nb_neur*prop_exciter), nb_FS(nb_neur-nb_RS);
        for(int i(0); i<nb_RS; ++i) neurons.push_back(new Neuron("RS",0,ext,delta));
        for(int i(0); i<nb_FS; ++i) neurons.push_back(new Neuron("FS",0,ext,delta));			
    }else{
        for(const auto& I : p){
            for(int i(0); i< I.second*nb_neur; ++i) neurons.push_back(new Neuron(I.first,0,ext,delta));
        }
    }
    set_connections();
    
}

NeuronNetwork::~NeuronNetwork(){
    for(auto& n : neurons){
        if(n) delete n;
    }
}

void NeuronNetwork::generateIds(){
    for(size_t i(0); i<neurons.size(); ++i) neurons[i]->setId(i);
}


void NeuronNetwork::set_connections() {   
    generateIds();
    int d(0);
    double l(0);
    for (size_t i(0); i < neurons.size(); ++i) {
        do {
            d = _RNG->poisson(lambda_);
        } while(d==0);
        
        for (int j(0); j < d; ++j) {
            l = _RNG->uniform_double(0, 2*L_);
            int id_neuron = _RNG->uniform_int(0,neurons.size()-1);
            while (neurons[i]->checkConnection(id_neuron)) {
                id_neuron = _RNG->uniform_int(0,neurons.size()-1);  
            }
            
            neurons[i]->addConnection(std::pair<int,double>(id_neuron,l)); 
        }
    }
    for(auto& n : neurons) I(n->getId());
}

void NeuronNetwork::I(int id) {
    Neuron* n = neurons[id];
    double intensity_inhib(0.), intensity_excit(0.);
     
    for (const auto& c : n->getConnections()) {
        Neuron* neuron_connected = neurons[c.first];

        if (neuron_connected->isFiring()) {
            if (neuron_connected->getQuality()) intensity_excit += c.second;
             else intensity_inhib += c.second;
            }
    }
    double w, J(_RNG->normal(0,1));
    if(n->getQuality()) w=5;
    else w=2;
    n->setI(w*J + 0.5*intensity_excit - intensity_inhib);
    return;
}
                

Neuron* NeuronNetwork::getNeuron(int id) const {
    return neurons[id];
}

double NeuronNetwork::getPot(int id) const{
    return neurons[id]->getV();
}

double NeuronNetwork::getRelax(int id) const {
    return neurons[id]->getU();
}

std::vector<Neuron*> NeuronNetwork::getNeurons() const {
    return neurons;
}


double NeuronNetwork::eqDiffSolverV(double u, double v, double I, double delta){
    return v + (0.04*pow(v,2)  + 5*v +140 -u + I)*delta;
}

double NeuronNetwork::eqDiffSolverU(double u, double v, double a, double b, double delta){
  return u + a*(b*v-u)*delta;
}

void NeuronNetwork::update(double delta_v, double delta_u){
    double old_v;
    for(auto& n : neurons){
        if (n->getV() > 30){
            n->setFiring(true);
            n->setPotential(n->getC());
            n->setRelaxation(n->getU() + n->getD());
        }else{
            n->setFiring(false);
            old_v=n->getV();
            n->setPotential(eqDiffSolverV(n->getU(), n->getV(), n->getI(), delta_v));      // 0.5ms step for numerical stability
            n->setPotential(eqDiffSolverV(n->getU(), n->getV(), n->getI(), delta_v));      // 0.5ms step for numerical stability
            n->setRelaxation(eqDiffSolverU(n->getU(), old_v, n->getA(), n->getB(), delta_u));

        }
    }
    for(auto& n : neurons) I(n->getId());
}
 

double NeuronNetwork::getValence(int id) const{
    double tot_inhib(0.), tot_excit(0.);
    for(const auto& n : getNeuron(id)->getConnections()){
        Neuron* neuron_connected(getNeuron(n.first));
            if(neuron_connected->getQuality()) tot_excit+=n.second;
            else tot_inhib+=n.second;
    }

    return tot_excit-tot_inhib;
}

