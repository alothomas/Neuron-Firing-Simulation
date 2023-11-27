# NeuronNetwork 

This program is based on a simplified model of neurons by Eugene Izhikevich. 
This model reproduces spiking and bursting behavior of different types of cortical neurons (RS,IB,CH,LTS,FS).

## Installation

To use this program, simply clone the git repository of the project: ```git clone https://gitlab.epfl.ch/sv_cpp_projects/team_22.git```.

In order to build and compile this program, the gtest library, tclap library, cmake and doxygen are required.

After cloning the project, you can then execute the following commands to continue the installation :
```
cd team_22
rm -rf build
mkdir build
cd build 
cmake..
make
```

## Usage

The user needs to specify the paramaters of the desired simulation which are listed below :

* time: t
* Number of neurons: N
* Average connectivity: C
* Average intensity of the links: L
* Proportions and types of neurons: T
* Proportions of excitatory RS neurons (to be used if you only want RS/FS neurons): P
* Activation of extended control of the thalamic noise: E
* Thalamic noise control (only works when the extended control of the thalamic noise is set): D

Example of a command line for execution: ```./Neurons -t 500 -N 10000 -C 4 -L 4 -T 'FS:0.2, IB:0.3, RS:0.5```

In this example, it will create a 500ms simulation and print the results inside 3 output files (spikes.txt, parameters.txt, sample_neurons.txt).

The output can then be visualized with the Rscript ```RasterPlots.R``` which can be found in /res folder with the following command:

 ```Rscript RasterPlots.R spikes.txt sample_neurons.txt parameters.txt```. 

This program uses gtest to test its good implementation of the code which can be done with ```make test```.
The doxygen documentation can be generated with ```make doc```.

## Authors

This program was written by Charlotte Coulon, Alois Thomas, Laura Gonzalez and Angelica Reitelli.
It is based on the [Simple Model of Spiking Neurons](https://www.izhikevich.org/publications/spikes.pdf) from Eugene M. Izhikevich