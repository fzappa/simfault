#ifndef DEFINICOES_H_
#define DEFINICOES_H_
//**********************************************
//  Common Definitions. (C++14)
//  
//  Implemented for study purposes. 
//  Uses the Eigen (http://eigen.tuxfamily.org/) 
//  library to facilitate the matrix treatment.
//  
//  
//  License: GPLv3
//  Author: Alan Franco - https://github.com/fzappa
//  Rev.: 13/05/2017 (PT_BR)
//*********************************************

#include <Eigen/Dense>

//using namespace boost::multiprecision;

// Todas as funcoes definidas para complexos
typedef unsigned int uint;
typedef long double ldouble;
typedef std::complex<double> tipo;
typedef Eigen::Matrix<tipo, Eigen::Dynamic, Eigen::Dynamic> Matriz;
typedef Eigen::Matrix<tipo, Eigen::Dynamic, 1> Vetor;
typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> Matrizd;
typedef Eigen::Matrix<double, Eigen::Dynamic, 1> Vetord;

const double radiano2grau {57.295779513};

using namespace std::literals; //para notacao complexa 1+2i



#endif
