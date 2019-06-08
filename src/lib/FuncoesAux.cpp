//**********************************************
//  Functions for Modifications of existing Zbus. (C++14)
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

#include <boost/lexical_cast.hpp>

#include "FuncoesAux.hpp"
#include "Tipos.hpp"

using namespace std;
using namespace boost;


void AdequaValores(Matriz& M, const double& tol){

  // Verifica se o valor esta dentro da tolerancia
  // Para eliminar valores pequenos que podem ser 
  // considerados como zero de acordo com a tolerancia
  // ex: Se(1e-18 < tol), entao valor = 0 
  for(uint i = 0; i < uint(M.size()); ++i){
    if( (M(i).real() < tol) &  (M(i).real() > 0.0) )
      M(i).real(0.0);
    if( (M(i).imag() < tol) &  (M(i).imag() > 0.0) )
      M(i).imag(0.0);
    if( (M(i).real() > -tol) &  (M(i).real() < 0.0) )
      M(i).real(0.0);
    if( (M(i).imag() > -tol) &  (M(i).imag() < 0.0) )
      M(i).imag(0.0);
  }

} //AdequaValores


void OrganizaLinhas(Matriz& MLinhas){

  // --- INICIO - Adequando os Dados de Linhas ---//

  // Como o parser XML que criei le todos os dados como real
  // e necessario converter os valores lidos nas 
  // colunas de reatancia para complexo

  //Convertendo para X
  // imag(X) <- real(X)
  MLinhas.col(4).imag() = MLinhas.col(4).real(); // Valores de X imagnario
  MLinhas.col(4).real().setZero();  // Zera a componente real de X

  // Convertendo para o shunt B
  // imag(B/2) <- real(B/2)
  MLinhas.col(5).imag() = MLinhas.col(5).real(); // Valores de B imagnario
  MLinhas.col(5).real().setZero();  // Zera a componente real de B

  // --- FIM - Adequando os Dados de Linhas ---//

} // OrganizaLinhas


void OrganizaBarras(Matriz& MBarras){

  // --- INICIO - Adequando os Dados de Barras ---//

  //Fazendo a mesma conversao para as barras
  // imag(Xd) <- real(Xd)
  MBarras.col(2).imag() = MBarras.col(2).real(); // Valores de Xd imaginario
  MBarras.col(2).real().setZero();

  // --- FIM - Adequando os Dados de Barras ---//

} // OrganizaBarras


Matriz MontaYbarra(XML::CLeXML& Dados){
  int De {0};
  int Para {0};

  Matriz MBarras;
  Matriz MLinhas;


  // Dados de Linhas
  Dados.setElemento("Caso.DadosDeLinhas");
  MLinhas = Dados.getMatriz();
  if(MLinhas.size() == 0){
    throw std::invalid_argument("\n[FuncoesAux] Não existem dados de linhas!\n\n");
  }

  OrganizaLinhas(MLinhas); // --- Adequa os Dados de Linhas ---//

  // Verifica a barra de maior numero em "De - Para"
  int maxde {lexical_cast<int>(MLinhas.real().col(1).maxCoeff())};
  int maxpara {lexical_cast<int>(MLinhas.real().col(2).maxCoeff())};
  int bmax;

  //Essa sera a dimensao da matriz Ybarra
  bmax = max(maxde,maxpara);

  // Inicializa a Matriz Ybarra
  Matriz MYb {Matriz::Zero(bmax,bmax)};

  
  // Adiciona os dados de barras aos elementos Ykk
  // Dados de Barras
  Dados.setElemento("Caso.DadosDeBarras");
  MBarras = Dados.getMatriz();

  if(MBarras.size() > 0){ // Se existir Dados de Barras

    OrganizaBarras(MBarras); // --- Adequando os Dados de Barras ---//

    // Le a tabela DadosDeBarras 
    // Atribui Xd ao valor inicial de Ykk
    // Daqui sai a primeira parte: Ykk = Xd 
    for(uint i = 0; i < uint(MBarras.rows()) ;++i){  // Variando ate o numero de barras
      if(MBarras(i,2).imag() != 0){
        De = (lexical_cast<int>(MBarras(i,1).real())-1); // Redundante, mas facilita a leitura
        MYb(De, De) = -1.0/MBarras(i,2);  // M(i,i) = 1/Xd
      }
    }
  } // if(MBarras.size() > 0)


  //Elementos fora da diagonal
  // Considerando o Tap do transformador
  // Elementos em serie
  // Yik = tap * 1/(R + jX)
  // Elementos em paralelo 
  // Ykk += (tap^2 - tap)*B/2
  // Yii += (1 - tap)*B/2
  for(uint i=0; i < uint(MLinhas.rows()) ; ++i){  // Variando ate o numero de linhas
    De = (lexical_cast<int>(MLinhas(i,1).real())-1); // Redundante, mas facilita a leitura
    Para = (lexical_cast<int>(MLinhas(i,2).real())-1);
    MYb(De, Para) = (MLinhas(i,6))*(-1.0/(MLinhas(i,3) + MLinhas(i,4) ) )  ;  // Yik = tap * 1/(R + jX)
    MYb(Para, De) = MYb(De, Para); // Yki = Yik

    // Adiciona ao valor Ykk (De e Para) o valor de B/2
    // Ao final Ykk = Xd + B/2(De) + B/2(Para) (se for o caso)
    MYb(De,De) += (MLinhas(i,6)*MLinhas(i,6) - MLinhas(i,6)) * (-MLinhas(i,5)); // (tap^2 - tap)*y
    MYb(Para,Para) += (1.0 - MLinhas(i,6)) * (-MLinhas(i,5)/2.0); // (1 - tap)*B/2
  }

  // Agora, os elementos da diagonal principal sao 
  // Ykk = 1/Xd + B/2(De) + B/2(Para)
  // E os elementos fora da diagonal sao
  // Yik = 1/(R + jX)
  // Ykk(final) sera a soma do Ykk(antigo) com todos os Yik da mesma linha
  // resultando na soma de todos os elementos conectados a barra
  // Ykk = 1/Xd + B/2(De) + B/2(Para) + Soma(Yik) conectados a Ykk
  for(uint i=0; i < uint(MYb.rows()); ++i){
    MYb(i,i) = -MYb.row(i).sum() ; // X'd + B(barra de) + B(barra para) + Yik
  }

  // --- FIM - Construcao da Ybarra ---//

  return MYb; 

} //MontaYbarra



Matriz MontaZbarra(XML::CLeXML& Dados){
  uint De {0};
  uint Para {0};

  Matriz MBarras;
  Matriz MLinhas;

  // Dados de Linhas
  Dados.setElemento("Caso.DadosDeLinhas");
  MLinhas = Dados.getMatriz();
  if(MLinhas.size() == 0){
    throw std::invalid_argument("\n[FuncoesAux] Não existem dados de linhas!\n\n");
  }
  OrganizaLinhas(MLinhas); // --- Adequa os Dados de Linhas ---//

  // Verifica a barra de maior numero em "De - Para"
  uint maxde {lexical_cast<uint>(MLinhas.real().col(1).maxCoeff())};
  uint maxpara {lexical_cast<uint>(MLinhas.real().col(2).maxCoeff())};
  uint bmax;

  //Essa sera a dimensao da matriz Zbarra
  bmax = max(maxde,maxpara);

  Matriz Zbarra {Matriz::Zero(bmax,bmax)};
  
  // Dados de Barras
  Dados.setElemento("Caso.DadosDeBarras");
  MBarras = Dados.getMatriz();

  // --- INICIO - Construcao da Zbarra ---//

  // Considerando que uma barra isolada é equivalente a uma barra conectada
  // a referencia atraves de uma impedancia infinita...
  // Adiciona todas as barras como Tipo 1
  // Referencia para barra nova
  // Inicializa todas as barras, assim so existirao conexoes tipo 4
  // Barras sem conexao a referencia (0) são inicializadas com 
  // alta impedancia (1e10)
  if(MBarras.size() > 0){ // Se existir Dados de Barras

    OrganizaBarras(MBarras); // --- Adequa os Dados de Barras ---//

    // Adiciona Xd ao elemento kk
    for(uint i = 0; i < uint(MBarras.rows()); ++i){
        De = lexical_cast<uint>(MBarras(i,1).real()) -1; 
        Zbarra(De,De) = MBarras(i,2);
    }
  } //(MBarras.size() > 0)


  //Adiciona impedancia alta as outras barras
   for(uint i = 0; i < bmax; ++i){
     if( Zbarra(i,i).imag() == 0.0 ){
      Zbarra(i,i) = tipo(1e10,1e10);
    }
  }


  // Agora, todos os elementos serao do tipo 4
  for(uint i = 0; i < uint(MLinhas.rows()); ++i){
    De = lexical_cast<uint>( MLinhas(i,1).real() ) ;
    Para = lexical_cast<uint>( MLinhas(i,2).real() ) ;
    // Checar a montagem Tipo 4
    // Xd ja foi adicionado aos elementos
    // Adiciona os elementos R + jX 
    AnaliseDeRedes::Tipo4(Zbarra, De, Para, MLinhas(i,3) + MLinhas(i,4) );
  }

  // --- FIM - Construcao da Zbarra ---//

  return Zbarra; 

}//MontaZbarra


