//**********************************************
//  Class for Fault analysis. (C++14)
//  
//  Implemented for study purposes. 
//  Uses the Eigen (http://eigen.tuxfamily.org/) 
//  library to facilitate the matrix treatment.
//  
//  
//  License: GPLv3
//  Author: Alan Franco - https://github.com/fzappa
//  Rev.: 20/06/2017 (PT_BR)
//*********************************************

#include <iostream>
#include <stdexcept>
#include "CFaltas.hpp"

namespace AnaliseDeFaltas{

  CFault::CFault(const Matriz& MA){
    pMZ = MA;
    pVE = Vetor::Ones(MA.rows());

    pBus = 0;
    pIFault = 0.0;

    pSim_A = tipo(-0.5,0.866025403784); // a em coordenadas retangulares
    pCompSim = Matriz::Ones(3,3);
    pCompSim(1,1) = pSim_A*pSim_A;  //  | 1  1  1  |
    pCompSim(1,2) = pSim_A;         //  | 1  a2 a  |
    pCompSim(2,1) = pSim_A;         //  | 1  a  a2 |
    pCompSim(2,2) = pSim_A*pSim_A;
  }

  CFault::CFault(const Matriz& MA, const Vetor& VB){
    pMZ = MA;
    pVE = VB;

    pBus = 0;
    pIFault = 0.0;

    pSim_A = tipo(-0.5,0.866025403784); // a em coordenadas retangulares
    pCompSim = Matriz::Ones(3,3);
    pCompSim(1,1) = pSim_A*pSim_A;  //  | 1  1  1  |
    pCompSim(1,2) = pSim_A;         //  | 1  a2 a  |
    pCompSim(2,1) = pSim_A;         //  | 1  a  a2 |
    pCompSim(2,2) = pSim_A*pSim_A;
  }

  void CFault::setZbus(const Matriz& MA){
    pMZ = MA;
  }
  
  void CFault::setVbus(const Vetor& VB){
    pVE = VB;
  }
  
  void CFault::setBusFault(const int& bus){
    pBus = bus;
  }

  // Symmetrical faults
  // 3-phase fault
  Matriz CFault::getLLL(void){

    Vetor Tensoes {pVE};
    Matriz Saida {Matriz::Zero(pVE.size(),4)};

    // Passando para complexo
    Tensoes.imag() = Tensoes.real();
    Tensoes.real().setZero();

    // Tensoes = Tensoes pre-falta nas barras
    // Faltas = onde for zero e a barra de falta

    // Corrente de falta
    if(pBus == 0.0)
      throw std::invalid_argument("\n[CFaltas] Barra de curto não definida!\n\n");
    else
      // Corrente de falta na barra escolhida
      pIFault = -Tensoes(pBus-1)/pMZ(pBus-1,pBus-1).imag(); // ignorando a resistencia

    // Coluna com o numero das barras
    for(uint i = 0; i < Tensoes.size(); ++i){
      Saida(i,0) = i+1; // numero da barra
      Saida(i,3) = -Tensoes(i)/pMZ(i,i).imag(); //Corrente de curto na barra
    }


    Vetor I012 {Vetor::Zero(3)};
    Vetor Iabc {Vetor::Zero(3)};
    // Z = sqrt(real^2 + imag^2)
    // theta = arctan(imag/real)

    I012(1) = pIFault; // Sequencia positiva

    Iabc.real().array() = (pCompSim*I012).array().abs();
    Iabc.imag().array() = ( (pCompSim*I012).array().imag()/(pCompSim*I012).array().real()).atan()*radiano2grau;

    // Tensoes nas barras
    Vetor Vbarra {Vetor::Zero(Tensoes.size())};
    Vetor V012 {Vetor::Zero(3)};
  
    for(uint i = 0; i < Vbarra.size(); ++i){
      //Tensao na barra
      Saida(i,1) = Tensoes(i).imag() - (pMZ(i,pBus-1).imag() * (-pIFault.imag()) ); // Ignorando as resistencias
      
      // Corrente na barra
      Saida(i,2) = -Saida(i,1) / pMZ(i,i).imag();
    }

    // Arrumando a casa...
    Saida.col(2).imag().array() = Saida.col(2).real().array();
    Saida.col(2).real().array().setZero();
    Saida(pBus-1,1) = 0.0;
    pIFault.real(0.0);
    Saida(pBus-1,2) = pIFault;


    return Saida;

  } 



  // 3-phase to ground fault
  Matriz CFault::getLLLG(void) const {
    Matriz Saida {Matriz::Ones(3,3)};

    return Saida;
  } 

  // Asymmetrical faults
  // phase-ground
  Matriz CFault::getLG(void) const {
    Matriz Saida {Matriz::Ones(3,3)};

    return Saida;
  }

  // phase-phase 
  Matriz CFault::getLL(void) const {
    Matriz Saida {Matriz::Ones(3,3)};

    return Saida;
  }  

  // phase-phase to ground
  Matriz CFault::getLLG(void) const {
    Matriz Saida {Matriz::Ones(3,3)};

    return Saida;
  }  


}// namespace