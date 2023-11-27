#include <gtest/gtest.h>
#include "constants.h"
#include "Random.h"
#include "Simulation.h"
#include "NeuronNetwork.h"
#include "Neuron.h"

Random* _RNG = new Random(1234567890);

TEST(Random, uniform){
    EXPECT_GE(_RNG->uniform_int(50,150), 50);
    EXPECT_LE(_RNG->uniform_int(50,150), 150);
    EXPECT_GE(_RNG->uniform_double(13.2,50.45), 13.2);
    EXPECT_LE(_RNG->uniform_double(13.2,50.45), 50.45);
}

TEST(Simulation, cmdLine){
    std::vector <std::string> input { "./Neurons", "-t 100", "-N 50", "-C 4", "-L 2", "-T FS:0.2,CH:0.5,LTS:0.3"};
    std::vector <char*> arg;
    for(size_t i(0); i < input.size(); ++i) {
        arg.push_back(const_cast<char*>(input[i].c_str()));
    }

    Simulation s(arg.size(), &arg[0]);
    double totLTS(0.), totCH(0.), totFS(0.);

    EXPECT_EQ(s.getNetwork().size(), 50);
    for(const auto& n : s.getNetwork()){
        EXPECT_TRUE( (n->getType()=="LTS") or (n->getType()=="CH") or (n->getType()=="FS"));
        if(n->getType()=="LTS") totLTS+=1;
        if(n->getType()=="CH") totCH+=1;
        if(n->getType()=="FS") totFS+=1;
    }
    EXPECT_EQ(totLTS/s.getNetwork().size(), 0.3);
    EXPECT_EQ(totCH/s.getNetwork().size(), 0.5);
    EXPECT_EQ(totFS/s.getNetwork().size(), 0.2);
}

TEST(NeuronNetwork, set_connections) {
	
    std::vector<Neuron*> neurons;
    
    for (size_t i(0); i < 40; ++i){
        neurons.push_back(new Neuron("FS", i));
	}
	
	
    double lambda(2);
	double intensity(3.0); 
    NeuronNetwork N(neurons, lambda, intensity);
    double S(0);
    int nb_connections(0);
    int nb_neurons(0);
         
	N.set_connections();
		
	for (size_t i(0); i<neurons.size(); ++i){	
		nb_neurons=nb_neurons+1;
	    EXPECT_TRUE(!neurons[i]->getConnections().empty()); 
			
			for (auto j : neurons[i]->getConnections()){
				nb_connections=nb_connections+1;
				EXPECT_TRUE(j.first || neurons[i]->getId());  
				EXPECT_LE(j.first, neurons.size()-1); 
			}
            S+=nb_connections;
            nb_connections = 0;
	}
	
	double nb_connections_mean = S/nb_neurons;
    EXPECT_NEAR(nb_connections_mean, lambda, sqrt(lambda));
		
}


TEST(NeuronNetwork, I){
	Neuron* neuron_0 = new Neuron("FS", 0);
	Neuron* neuron_1 = new Neuron("RS", 1);
	Neuron* neuron_2 = new Neuron("FS", 2);
	std::vector<Neuron*> Neurons({neuron_0, neuron_1, neuron_2});
    for (auto& neuron : Neurons){
		neuron->setFiring(true);
	} 
	NeuronNetwork N(Neurons, 2, 3.5);

	
	neuron_0->addConnection(std::pair<int, double> (1, 4.0));
	neuron_0->addConnection(std::pair<int, double > (2, 3.0));

    N.I(neuron_0->getId());
    double computed_I = neuron_0->getI();
    double intensity_sum (0.5*4.0 - 3.0 );
    EXPECT_LE(computed_I, intensity_sum + 2*2.58);
    EXPECT_GE(computed_I, intensity_sum - 2*2.58);
}
	

TEST(NeuronNetwork, update){
	
	Neuron* neuron_0 = new Neuron("FS", 0);
	Neuron* neuron_1 = new Neuron("RS", 1);
	std::vector<Neuron*> Neurons({neuron_0, neuron_1});
	
	NeuronNetwork N (Neurons, 2, 2.0);

	neuron_0->setPotential(50);
	neuron_1->setPotential(20);
	
	double input_V_0 (neuron_0->getC());
	double old_v(neuron_1->getV());
	double input_V_1 = N.eqDiffSolverV(neuron_1->getU(), neuron_1->getV(), neuron_1->getI(), 0.5 );	
	input_V_1 = N.eqDiffSolverV(neuron_1->getU(), input_V_1, neuron_1->getI(), 0.5 );
	

	             
	double input_U_0 (neuron_0->getU()+ neuron_0->getD());
    double input_U_1 (N.eqDiffSolverU(neuron_1->getU(), old_v, neuron_1->getA(), neuron_1->getB(), 1.0));
	
    N.update(0.5, 1);

	
	EXPECT_TRUE(neuron_0->isFiring());	
	EXPECT_EQ ( input_V_0 , neuron_0->getC() );	
	EXPECT_EQ ( input_U_0 , neuron_0->getU());  
	
	EXPECT_FALSE(neuron_1->isFiring());
	EXPECT_EQ(input_V_1, neuron_1->getV());
	EXPECT_EQ(input_U_1, neuron_1->getU());   			
}

TEST(Simulation, test_read_proportions){
	
    std::vector <std::string> input { "./Neurons", "-t 100", "-N 50", "-C 4", "-L 2", "-T RS:0.1,IB:0.3,CH:0.1,FS:0.25,LTS:0.25"};
    std::vector <char*> arg;
    for(size_t i(0); i < input.size(); ++i) {
        arg.push_back(const_cast<char*>(input[i].c_str()));
    }

    Simulation S(arg.size(), &arg[0]);
    
    std::map<std::string, double> Prop = S.test_read_proportions("RS:0.1, IB:0.3, CH:0.1, FS:0.25, LTS:0.25");
	
	EXPECT_EQ(Prop.find("RS")->second, 0.1);
	EXPECT_EQ(Prop.find("IB")->second, 0.3);
	EXPECT_EQ(Prop.find("CH")->second, 0.1);
	EXPECT_EQ(Prop.find("FS")->second, 0.25);
    EXPECT_EQ(Prop.find("LTS")->second, 0.25);
}

TEST(Neuron, checkConnection){
	Neuron neuron_0("RS", 0);
	Neuron neuron_1("FS", 1);
	
	bool B1= neuron_0.checkConnection(0);
	bool B2 =  neuron_0.checkConnection(1);
	
	EXPECT_TRUE(B1);
	EXPECT_FALSE(B2);
}

TEST(Neuron, setinitialattributs){
    Neuron* neuron_0 = new Neuron("RS", 0);
    Neuron* neuron_1 = new Neuron("LTS", 1);
    Neuron* neuron_2 = new Neuron("CH", 2);
    Neuron* neuron_3 = new Neuron("FS", 3);
    
    double neuron_0_A = 0.02;
    double neuron_1_B = 0.25;
    double neuron_2_C = -50;
    double neuron_3_D = 2;
    
    neuron_0->setinitialattributs("RS");
    neuron_1->setinitialattributs("LTS");
    neuron_2->setinitialattributs("CH");
    neuron_3->setinitialattributs("FS");
    
    EXPECT_EQ(neuron_0_A,neuron_0->getA());
    EXPECT_EQ(neuron_1_B,neuron_1->getB());
    EXPECT_EQ(neuron_2_C,neuron_2->getC());
    EXPECT_EQ(neuron_3_D,neuron_3->getD());
    
}


TEST(NeuronNetwork, getValence){
	Neuron* neuron_0 = new Neuron("RS", 0);
	Neuron* neuron_1 = new Neuron("LTS", 1);
	Neuron* neuron_2 = new Neuron("CH", 2);
	Neuron* neuron_3 = new Neuron("FS", 3);
	
	std::vector<Neuron*> neurons({neuron_0, neuron_1, neuron_2, neuron_3});
	
	neuron_1->addConnection(std::pair<int, double> (0, 4.0));
	neuron_1->addConnection(std::pair<int, double> (2, 8.5));
	neuron_1->addConnection(std::pair<int, double> (3, 11.3));
	
	NeuronNetwork N(neurons, 5, 7.93);
	
	double tot_exit = 4.0+8.5;
	double tot_inhib = 11.3;
	double Valence = tot_exit-tot_inhib;
	
	double test_Valence = N.getValence(1);
	
	EXPECT_EQ(test_Valence, Valence);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    
    return RUN_ALL_TESTS();
}

