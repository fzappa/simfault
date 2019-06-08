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
#include <vector>

#include "Kron.hpp"

namespace AnaliseDeRedes {

  // Faz a reducao de Kron
  const Matriz Kron(const Matriz& A){
    const uint n {uint(A.rows())-1};
    Matriz temp {Matriz::Zero(n, n)};
    //Eliminacao
    for(uint i=0;i<n;++i){
      for(uint j=0;j<n;++j){
        temp(i,j) = A(i,j) - ( A(i,n) * A(n, j) )/( A(n,n));
      }
    }

    return temp;
  } // Kron


  //Reducao de Kron ate ordem N
  const Matriz KronRN(const Matriz& A, int N){

    if (N <= 0)
      throw std::invalid_argument("Ordem de redução menor ou igual a zero." );
    else if (N >= int(A.rows()))
      throw std::invalid_argument("Ordem de redução maior ou igual a ordem da Matriz." );
    else {

      Matriz temp {A};
      for (uint i= uint(temp.rows()) - 1; i>=uint(N);--i)
        temp = Kron(temp);

      return temp;
    }
  } // KronRN


  // Redução de Kron - Remove uma barra especifica
  const Matriz KronB(const Matriz& A, int L){

    if ( (L-1) < 0) // Remover a barra 1 = eliminar a linha e coluna 0
      throw std::invalid_argument("Linha a ser removida invalida." );
    else if ( L > int(A.rows())) // Se barra eliminada > numero de barras
      throw std::invalid_argument("Linha a ser removida maior que a linha de maior ordem." );
    else if (L == int(A.rows()) ) // Se barra eliminada = ultima barra
      return Kron(A);
    else { // Elimina a linha especificada

      Matriz temp ;
      std::vector<tipo> Dados;
      temp.setZero(A.rows()-1,A.rows()-1);

      int k {-1};
      for (uint i=0; i < uint(A.rows()); ++i){
        for (uint j=0; j < uint(A.rows()); ++j){
          k++;
          if ( (i!= uint(L-1)) & (j!= uint(L-1)) ){ // Se linha ou coluna forem diferentes da removida
            // Mapeia os dados para um vetor sequencial
            Dados.push_back(A(i,j) - (A(i,(L-1))*A((L-1),j))/A((L-1),(L-1)));
          }
        }
      }

      k = 0;
      for(uint i=0; i < uint(temp.rows()); ++i){
        for(uint j=0; j < uint(temp.rows()); ++j){
          if (Dados[k] != tipo(0)){
            temp(i,j) = Dados[k]; // Varre o vetor e atribui os elementos diferentes de zero
            ++k;
          }
        }
      }

      return temp;
    }

  }//KronB



} //namespace