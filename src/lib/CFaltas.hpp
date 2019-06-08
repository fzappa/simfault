#ifndef CFALTAS_HPP_
#define CFALTAS_HPP_

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


#include "Definicoes.hpp"

namespace AnaliseDeFaltas{

class CFault{
    private:
      Matriz pMZ;
      Vetor  pVE;

      uint   pBus;
      tipo   pSim_A;
      tipo   pIFault;
      Matriz pCompSim;


    public:
      CFault(const Matriz&);
      CFault(const Matriz&, const Vetor&);
      virtual ~CFault(void) noexcept(true) {}; //Destructor

      // Methods 
      void setZbus(const Matriz&);
      void setVbus(const Vetor&);
      void setBusFault(const int&);

      // Matrix Out
      // Bus |  Vbus |  Ibus  |  Ibus if fault is at this bus
      // Symmetrical faults
      Matriz getLLL(void);  // 3-phase fault
      Matriz getLLLG(void) const; // 3-phase to ground fault

      // Asymmetrical faults
      Matriz getLG(void) const; // phase-ground
      Matriz getLL(void) const; // phase-phase
      Matriz getLLG(void) const; // phase-phase to ground


  };

}// namespace



#endif
