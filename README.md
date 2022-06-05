
<br>
<p align="center">
  <img src="https://github.com/ramosrafaela/GEANT4-ARAPUCA-Simulations/blob/main/figures/muon_2_1.png" />
</p>


<h1 align="center"> GEANT4 Simulatios for X-ARAPUCA test device </h1>

![Badge concluded](http://img.shields.io/static/v1?label=STATUS&message=CONCLUDED&color=GREEN&style=for-the-badge)

This is a project that I developed during my undergraduating research in the Leptons Laboratory at UNICAMP. This simulation aims to simulate the apparatus used in the real experiment performed in the laboratory, the radioactive source used and quantify how many photons arrive in the acceptance window of the X-ARAPUCA light detector. With this information it is possible to combine with experimental data and obtains the total efficiency of the device.

The experiment in question consisted of a support for an alpha radiation source and for the X-ARAPUCA device. The holder and detector were then placed in a cryostat where it was filled with liquid argon (LAr) as the active volume. When alpha particles are emitted, they, in contact with argon, produce a scintillation process, which in short, is a process of excitation and ionization of argon molecules that results in the production of photons, that is, light. These photons are emitted isotropically, therefore, the interest of the work is to verify how many of these photons reach the acceptance window of the device.

Three types of simulations were performed, with different ionizing particles: alpha particles, gamma particles and atmospheric muons.

The main result of the simulation consists of a histogram of the number of photons that arrived in the X-ARAPUCA acceptance window per ionizing particle.

To acess the codes you will need to perform the following path: 

- GEANT4-ARAPUCA-Simulations/Alpha_Particle/B1/src/

<br>
<p align="center">
  <img src="https://github.com/ramosrafaela/GEANT4-ARAPUCA-Simulations/blob/main/figures/simulacao_com_exp_smearing.pdf" />
</p>

- GEANT4-ARAPUCA-Simulations/Atmospheric_Muon/B1/src/

<br>
<p align="center">
  <img src="https://github.com/ramosrafaela/GEANT4-ARAPUCA-Simulations/blob/main/figures/detPhotons_log.pdf" />
</p>

- GEANT4-ARAPUCA-Simulations/Gamma_Particle/B1/src

<br>
<p align="center">
  <img src="https://github.com/ramosrafaela/GEANT4-ARAPUCA-Simulations/blob/main/figures/gamma_5M_log.pdf" />
</p>


in src (source) you will find six different codes, where each one has a different task within the simulation:

- ``B1ActionInitialization.cc:`` It is the class where the user must instantiate and register a concrete G4VUserPrimaryGeneratorAction subclass, which generates the primary particles for each event.
- ``B1DetectorConstruction.cc:`` It is the class responsible for creating and modeling the detector's geometry, as well as determining its physical properties.
- ``B1EventAction.cc:`` This is the class thats provides interfaces to interact with Geant4 at the beginning and at the end of each event.

- ``B1PrimaryGeneratorAction.cc: `` This is the class responsible for creating the particles and where their energy, initial position and momentum are assigned.
 
- ``B1RunAction.cc:`` This class allows to retrieve information at each step (indipendentely of sensitive detectors)
- ``G4RunAction:`` This class is responsible for extracting the data from the simulation, creating the objects in which the data will be stored.

## ✔️ Techniques and technologies used

- ``C/C++``
- ``GEANT4``
- ``ROOT``
