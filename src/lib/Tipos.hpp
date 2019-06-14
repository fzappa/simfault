#ifndef TIPOS_HPP_
#define TIPOS_HPP_
//**********************************************
//  Functions for Modifications of existing Zbus. (C++11)
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

#include "Definicoes.hpp"
#include "Kron.hpp"

namespace AnaliseDeRedes {

// Tipo 1
// Referencia para barra nova
void Tipo1(Matriz &, const tipo &);

// Tipo 2
// Barra existente k para barra nova
void Tipo2(Matriz &, const int &, const tipo &);

// Tipo 3
// Barra existente k para referencia
void Tipo3(Matriz &, const int &, const tipo &);

// Tipo 4
// Barra existente j para barra existente k
void Tipo4(Matriz &, const int &, const int &, const tipo &);

}  // namespace AnaliseDeRedes

#endif
