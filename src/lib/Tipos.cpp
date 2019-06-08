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

#include "Tipos.hpp"

namespace AnaliseDeRedes {

  // Tipo 1
  // Referencia para barra nova
  void Tipo1(Matriz& M, const tipo& Zb){

    int lin {int(M.rows())};
    int col {int(M.cols())};

    Matriz Nova {Matriz::Zero(lin+1,col+1)};

    //Como as matrizes serao quadradas...
    for(int i=0;i<=lin;++i){
      for (int j=0;j<=col;++j){

        if((i==lin) & (j==col)){
          Nova(i,j) = Zb;
        } 
        else if(i==lin){
          Nova(i,j) = 0.0;
        }
        else if(j==col){
          Nova(i,j) = 0.0;
        }
        else
          Nova(i,j) = M(i,j);
      }
    }

    M.resize(lin+1,col+1);
    M = Nova;
  } //Tipo1


  // Tipo 2
  // Barra existente para barra nova
  void Tipo2(Matriz& M, const int& K, const tipo& Zb){

    int lin {int(M.rows())};
    int col {int(M.cols())};

    Matriz Nova {Matriz::Zero(lin+1,col+1)};

    //Como as matrizes serao quadradas...
    for(int i=0;i<=lin;++i){
      for (int j=0;j<=col;++j){

        if((i==lin) & (j==col)){
          Nova(i,j) = M(K-1,K-1) + Zb;
        } 
        else if( (i==lin) & (j<col) ){
          Nova(i,j) = M(K-1,j);
        }
        else if( (j==col) & (i<lin)){
          Nova(i,j) = M(i,K-1);
        }
        else
          Nova(i,j) = M(i,j);
      }
    }

    M.resize(lin+1,col+1);
    M = Nova;
  }//Tipo 2



  // Tipo 3
  // Barra existente para referencia
  void Tipo3(Matriz& M, const int& d, const tipo& Zb){

    Matriz Nova {Matriz::Zero(M.rows()-1,M.cols()-1)};
    Tipo2(M,d,Zb);
    Nova = Kron(M);

    M.resize(Nova.rows(),Nova.cols());
    M = Nova;  
  } //Tipo 3




  // Tipo 4
  // Barra existente para barra existente
  void Tipo4(Matriz& M, const int& d, const int& p, const tipo& Zb){

    int lin {int(M.rows())};
    int col {int(M.cols())};

    Matriz Nova {Matriz::Zero(lin+1,col+1)};

    //Como as matrizes serao quadradas...
    for(int i=0;i<=lin;++i){
      for (int j=0;j<=col;++j){

        if((i==lin) & (j==col)){
          Nova(i,j) = M(d-1,d-1) + M(p-1,p-1) -2.0*M(d-1,p-1) + Zb;
        } 
        else if( (i==lin) & (j<col) ){
          Nova(i,j) = M(d-1,j) - M(p-1,j);
        }
        else if( (j==col) & (i<lin)){
          Nova(i,j) = M(i,d-1) - M(i,p-1);
        }
        else
          Nova(i,j) = M(i,j);
      }
    }

    M = Kron(Nova);

  }//Tipo 4



} //namespace