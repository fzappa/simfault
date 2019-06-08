#ifndef READ_XML_HPP_
#define READ_XML_HPP_
//**********************************************
//  Class for read bus data from XML  
//  
//  Implemented for study purposes. 
//  Uses the Eigen (http://eigen.tuxfamily.org/) 
//  library to facilitate the matrix treatment.
//
//  Uses the Boost Library for XML (http://www.boost.org/)
//  
//  
//  License: GPLv3
//  Author: Alan Franco - https://github.com/fzappa
//  Rev.: 06/05/2017 (PT_BR)
//*********************************************

#include <string>
#include <boost/property_tree/ptree.hpp>

#include "Definicoes.hpp"


namespace XML {
  class CLeXML{

    private:
      std::string pArquivo {""};
      std::string pElemento {""};
      Matriz pMatriz;
      uint pLin;  // para contar o numero de linhas
      uint pElem; // para contar o numero de elementos
      uint pCol;  // para contar o numero de atributos

      // Member function - XML
      void pLeGera(void); // Le os dados do XML e gera a matriz
      const inline boost::property_tree::ptree& limpa_ptree(void) const;
      inline void pZera(void);


    public:  
      // Constructor
      CLeXML(const std::string&);
      CLeXML(const std::string&, const std::string&);
      virtual ~CLeXML(void) noexcept(true) {}; //Destructor

      // Methods 
      void   setArquivo(const std::string&);
      std::string getArquivo(void) const;

      void  setElemento(const std::string&);
      std::string getElemento(void) const;

      Matriz getMatriz(void) const;
      int getLin(void) const;
      int getCol(void) const;


  }; //class

} //namespace

#endif