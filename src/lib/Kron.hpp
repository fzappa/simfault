#ifndef KRON_HPP_
#define KRON_HPP_
//**********************************************
//  Function for Kron reduction. (C++14)
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

namespace AnaliseDeRedes {

// Faz a reducao de Kron
const Matriz Kron(const Matriz &A);

// Reducao de Kron ate ordem N
const Matriz KronRN(const Matriz &A, int N);

// Redução de Kron - Remove uma barra especifica
const Matriz KronB(const Matriz &A, int L);

}  // namespace AnaliseDeRedes

#endif
