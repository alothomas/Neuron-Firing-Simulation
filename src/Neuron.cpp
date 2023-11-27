#include "Neuron.h"
#include "Random.h"


Neuron::Neuron(std::string _type, int id, bool ext, double delta)
    :type(_type), firing(false), id(id), v(-65), current(0)
{
	setinitialattributs(type);
	setvariations(type, ext, delta);
	u=b*v; 
} 
   
   
void Neuron::setinitialattributs(std::string type){
		
	if(type=="RS"){
		quality=true;
		a=0.02;
		b=0.2;
		c=-65;
		d=8;
		return;
		}
		
	if(type=="IB"){
		quality=true;
		a=0.02;
	    b=0.2;
	    c=-55;
	    d=4;
	    return;
		}
		
	if(type=="CH"){
		quality=true;
		a=0.02;
	    b=0.2;
	    c=-50;
	    d=2;
	    return;
		}
		
	if(type=="FS"){
		quality=false;
		a=0.1;
	    b=0.2;
	    c=-65;
	    d=2;
	    return;
		}
		
	if(type=="LTS"){
		quality=false;
		a=0.02;
	    b=0.25;
	    c=-65;
	    d=2;
	    return;
		}
	else {throw std::logic_error("type Problem for the neuron's creation");}
}

void Neuron::setvariations(std::string type, bool ext, double delta){
	
	if (ext==false) {
        if (type=="RS") {
            double r(_RNG->uniform_double(0,1));
			c*=(1-((0.23)*r*r));
			d*=(1-((0.75)*r*r));
		}
		if (type=="FS"){
            double r(_RNG->uniform_double(0,1));
			a*=(1-0.8*r);
			b*=(1+0.25*r);
		}
        return;
	}
		
	else{
		double ra(_RNG->uniform_double(1-delta,1+delta));
		double rb(_RNG->uniform_double(1-delta,1+delta));
		double rc(_RNG->uniform_double(1-delta,1+delta));
		double rd(_RNG->uniform_double(1-delta,1+delta));	
		a*=ra;
		b*=rb;
		c*=rc;
		d*=rd;
	}
}

Neuron::~Neuron() {}

void Neuron::setFiring(bool _f){
	firing=_f;
}

void Neuron::setPotential(double _v){
	v=_v;	
}
	
void Neuron::setRelaxation(double _u){
	u=_u;	
}

void Neuron::setId(int i){
    id=i;
}

void Neuron::setI(double I){
    current = I;
}
	
bool Neuron::isFiring(){
    return firing;
}

void Neuron::setQuality(bool value){
   quality=value;
}

bool Neuron::getQuality() const{
	return quality;
}

double Neuron::getA() const{
    return a;
}

double Neuron::getC() const {
    return c;
}

double Neuron::getD() const {
    return d;
}

double Neuron::getB() const{
    return b;
}

double Neuron::getV() const{
   return v;
}

double Neuron::getU() const{
    return u;
}

int Neuron::getId() const{
    return id;
}

std::string Neuron::getType() const{
    return type;
}

double Neuron::getI() const{
    return current;
}

std::unordered_map<int,double> Neuron::getConnections() const{
	return connections;
}

void Neuron::addConnection(std::pair<int,double> co){
    connections.insert(co);
}

bool Neuron::checkConnection(int id) const{
    if(this->id == id) return true;
    if(connections.empty()) return false;
    size_t test(connections.count(id));
    if(test==0) return false;
    return true;
}



