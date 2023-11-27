#include "Simulation.h"
#include "constants.h"


Simulation::Simulation(int argc, char **argv){
    TCLAP::CmdLine cmd(_PRGRM_TEXT_);
        
    TCLAP::ValueArg<double> duration_sim ("t", "duration_sim", _TIME_TEXT_, true, _TIME_DEFAULT_, "double");
    cmd.add(duration_sim);
        
    TCLAP::ValueArg<int> nb_neur ("N", "nb_neur", _N_NEUR_TEXT_, true, _N_DEFAULT_, "int");
    cmd.add(nb_neur);
        
    TCLAP::ValueArg<double> prop_exciter ("P", "prop_exciter", _P_EXCITER_TEXT_, false, _P_EXCITER_DEFAULT_, "double");
    cmd.add(prop_exciter);
        
    TCLAP::ValueArg<int> conn_mean ("C", "conn_mean", _CONN_MEAN_TEXT_, false, _CONN_MEAN_DEFAULT_, "int");
    cmd.add(conn_mean);
        
    TCLAP::ValueArg<double> intensity_conn ("L", "intensity_conn", _INTENSITY_CONN_TEXT_, false, _INTENSITY_CONN_DEFAULT_, "double");
    cmd.add(intensity_conn);
        
    TCLAP::ValueArg<std::string> proportionstot ("T", "Proportions", _PROPORTIONS_TEXT_, false, _PROPORTIONS_DEFAULT_, "string");
    cmd.add(proportionstot);
    
    TCLAP::SwitchArg ext("E","ext",_EXT_TEXT_,false);  
    cmd.add(ext);
    
    TCLAP::ValueArg<double> delta ("D", "delta", _DELTA_TEXT_, false, _DELTA_DEFAULT_, "double");
    cmd.add(delta);
        
    cmd.parse(argc, argv);
        
    if (duration_sim.getValue()<=0) throw (std::invalid_argument("Time must be positive"));
    if (nb_neur.getValue()<=0) throw (std::invalid_argument("LNumber of neurons must be positive"));
    if (prop_exciter.getValue()<0 or prop_exciter.getValue()>1) throw (std::invalid_argument("Proportion of excitatory neurons must be between 0 and 1"));
    if (conn_mean.getValue()<=0) throw (std::invalid_argument("The average connectivity must be positive"));
    if (intensity_conn.getValue()<=0) throw (std::invalid_argument("The average intensity of the links must be positive"));
	if (delta.getValue()<=0.0 or delta.getValue()>=1.0) throw (std::invalid_argument("delta must be between 0 and 1 (0 and 1 not included)"));
    time_max_=duration_sim.getValue();
    time_=0.;
    delta_t_U=1.;
    delta_t_V=0.5;
    if(prop_exciter.isSet() and proportionstot.isSet()) throw std::invalid_argument("You need to specify the proportions and/or the types of neurons");
    if(not(prop_exciter.isSet()) and not(proportionstot.isSet())) throw std::invalid_argument("You need to specify the proportions and/or the types of neurons");

    types_={"RS", "FS"};
    if(prop_exciter.getValue()==1) types_ = {"RS"};
    if(prop_exciter.getValue()==0) types_ ={"FS"};

    std::map<std::string, double> p; 
    double propexci(prop_exciter.getValue());
    
    if(proportionstot.isSet()){
        types_.clear();
        p=test_read_proportions(proportionstot.getValue());
        propexci=-1;       
    }

    _RNG = new Random;
    network_ = new NeuronNetwork(nb_neur.getValue(), propexci, conn_mean.getValue(), intensity_conn.getValue(), p, ext.getValue(), delta.getValue());
		
    headers();
}

Simulation::~Simulation(){
    for(auto& I : outfiles_){
        if(I.is_open()) I.close();
    }
    std::cout.flush();
    if(network_) delete network_;
}


std::map<std::string, double> Simulation::test_read_proportions(std::string dataprop){    
    std::map<std::string, double> p;
    p["FS"] = 0.0;
    p["LTS"] = 0.0;
    p["RS"] = 0.0;
    p["IB"] = 0.0;
    p["CH"] = 0.0;

    std::string key, numb_s;
    dataprop.erase(std::remove_if(dataprop.begin(), dataprop.end(), isspace), dataprop.end()); 
    std::stringstream ss(dataprop);
    int e(0);
                                            
    do {
		std::getline(ss, key, ':'); 

        for (auto& x : p){
			if (key==x.first){
				types_.push_back(x.first);
                std::getline(ss, numb_s, ',');
                x.second=std::stod(numb_s);
                e=1;
            }
        }

    } while (!key.empty() and !ss.eof());
        
    if (e==0) throw (std::invalid_argument("Impossible to read the input, verify that your synthax is correct  (like 'IB:0.3, FS:0.3, CH:0.2') with the desired types of neurons."));

    float sum(0);
    for (const auto& typeneuron : p){
     sum+=typeneuron.second;
    }
    if (sum>1.0 or sum<0.0) throw (std::invalid_argument("The sum of the proportions must be equal to 1"));

    if(sum!=1.) p["RS"] += 1-sum;

    for(const auto& s :types_){
     if(s=="RS"){ return p;}
    }
    for(auto& I : p){
     if((I.first == "RS") and (I.second >0)) types_.push_back("RS");
    }
    return p;
}
    

void Simulation::run(){
    print_parameters();
    double progress(0.);

    while(time_<=time_max_){

        if(time_!=0) print();
        network_->update(delta_t_V, delta_t_U);
        progressBar(progress, 40);
        time_+=delta_t_U;
        progress = time_/time_max_;
    }
    std::cout << std::endl << "Your files are ready." << std::endl;
}

void::Simulation::createFiles(){
    std::vector<std::string> names = {"spikes.txt","parameters.txt", "sample_neurons.txt"};
    for(auto& s : names) outfiles_.push_back(std::ofstream(s, std::ios_base::out));
    return;
}

void Simulation::headers(){
    createFiles();

    std::ostream *outstr = nullptr;
    if(outfiles_[1].is_open()){
        outstr = &outfiles_[1];
        *outstr << "Type"
                << "\t" << "a"
                << "\t" << "b"
                << "\t" << "c"
                << "\t" << "d"
                << "\t" << "Inhibitory"
                << "\t" << "degree"
                << "\t" << "valence"
                << std::endl;
    }
    if(outstr==nullptr) throw Erreur("Couldn't write in a file", 0);

    
    outstr = nullptr;
    if(outfiles_[2].is_open()){
        outstr = &outfiles_[2];
        for(const auto& s : types_){
            *outstr << "\t" << s+".v"
                    << "\t" << s+".u"
                    << "\t" << s+".I";
        }
     *outstr << std::endl;
    }
    if(outstr==nullptr) throw Erreur("Couldn't write in a file", 0);

    Neuron* n;
    for(const auto& s : types_){
        do{
            n=network_->getNeuron(_RNG->uniform_int(0, network_->getNeurons().size()-1));
        }while(n->getType()!=s);
        sample_.push_back(n);
    }

}

void Simulation::print(){
    print_spikes();
    print_samples();
}

void Simulation::print_spikes(){
    std::ostream *outstr = nullptr;
    if (outfiles_[0].is_open()){
        outstr = &outfiles_[0];
        *outstr << time_ << "\t";
        for(const auto& n : network_->getNeurons()){
            *outstr << n->isFiring() << "\t";
        }
        *outstr << std::endl;
    }
    if(outstr==nullptr) throw Erreur("Couldn't write in a file", 0);

}

void Simulation::print_parameters(){
    std::ostream *outstr = nullptr;
    if (outfiles_[1].is_open()){
        outstr = &outfiles_[1];
        for(const auto& n : network_->getNeurons()){
            *outstr << std::setprecision(6)
                    << n->getType()
                    << "\t" << n->getA()
                    << "\t" << n->getB()
                    << "\t" << n->getC()
                    << "\t" << n->getD()
                    << "\t" << not(n->getQuality())
                    << "\t" << n->getConnections().size()
                    << "\t" << network_->getValence(n->getId())
                    << std::endl;
        }
    }
    if(outstr==nullptr) throw Erreur("Couldn't write in a file", 0);
}

void Simulation::print_samples(){
    std::ostream *outstr = nullptr;
    if (outfiles_[2].is_open()){
        outstr = &outfiles_[2];
        *outstr << time_;
        for(const auto& s : sample_){
            *outstr << std::setprecision(6)
                    << "\t" << s->getV()
                    << "\t" << s->getU()
                    << "\t" << s->getI();
        }
        *outstr << std::endl;
    }
    if(outstr==nullptr) throw Erreur("Couldn't write in a file", 0);
}


std::vector<Neuron*> Simulation::getNetwork() const{
    return network_->getNeurons();
}

void Simulation::progressBar(double progress, double width) const{
    std::cout << "Progress: [";
    int arrow_pos = width * progress;
    for (int i(0); i < width; ++i) {
        if (i < arrow_pos) std::cout << "=";
        else if (i == arrow_pos) std::cout << ">";
        else std::cout << " ";
    }
   std::cout << "] " << int(progress * 100) << " %" << '\r';
}












