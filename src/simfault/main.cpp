#include <iostream>
#include <fstream>

#include <vector>
#include <cstdio> // tratamento de erros

//#define EIGEN_USE_BLAS
#define EIGEN_USE_MKL_ALL
#include <Eigen/Dense> // Matrizes
#include <boost/lexical_cast.hpp> // conversao de tipos

// Minhas bibliotecas
#include "CLeXML.hpp" 
#include "Tipos.hpp"
#include "FuncoesAux.hpp"
#include "CFaltas.hpp"


using namespace std;
using namespace XML; // LeXML
using namespace AnaliseDeRedes; // LDU, Kron, Tipos(1,2,3 e 4)
using namespace AnaliseDeFaltas;

int main(int argc, char * argv[]){

  try{
    
    if(argc >= 2){

      // Para os arquivos de saida
      fstream arqsaida;
      std::string Relatorio;

      // Caso nao seja fornecido um nome para o relatorio
      if(argc < 3)
        Relatorio = "Relatorio.txt";
      else
        Relatorio = argv[2];


      // Objeto que armazena a tabela
      CLeXML TabelaDados(argv[1]);


      arqsaida.open (Relatorio, std::fstream::in | std::fstream::out | std::fstream::trunc);

       // 6- Parte 2 
      arqsaida << " Compilado em " << __DATE__ << " - " << __TIME__  << "\n";
      arqsaida << "\n\n############### Parte 2 - Curto Circuito ###############\n";

      
      // TabelaDados - retorna a lista de elementos do arquivo xml
      // MontaZbarra - funcao que monta a matriz Zbarra de sequencia +
      Matriz ZbarraDireto {MontaZbarra(TabelaDados)};

      // Pega os dados de barras
      TabelaDados.setElemento("Caso.DadosDeBarras");  // Pega os dados de barras
      Vetor Tensoes {TabelaDados.getMatriz().col(3)}; // Passa como vetor as tensoes das barras

      // Pega os dados de curto
      TabelaDados.setElemento("Caso.DadosDeCurto");
      int barra {boost::lexical_cast<int>(TabelaDados.getMatriz()(0,1).real())};
      

      arqsaida << "***** Curto 3-Fases na barra "<< barra <<" *****\n";

      // Classe para calcular o curto
      CFault CalCurto(ZbarraDireto,Tensoes);
      CalCurto.setBusFault(barra);

      // Converter a saida no formato (real, imag) para real, so pra facilitar
      // a leitura
      Matrizd Saida {Matrizd::Zero(ZbarraDireto.rows(),4)};

      // Arrumando a casa
      Saida.col(0).array() = CalCurto.getLLL().col(0).real().array();
      Saida.col(1).array() = CalCurto.getLLL().col(1).real().array();
      Saida.col(2).array() = CalCurto.getLLL().col(2).imag().array();
      Saida.col(3).array() = CalCurto.getLLL().col(3).imag().array();

      // Salva a saida no arquivo
      arqsaida << Saida << endl;
      
      arqsaida << "\n\nColuna 1 = Barra" << endl;
      arqsaida << "Coluna 2 = Tensão na barra para um curto na barra " << barra << endl;
      arqsaida << "Coluna 3 = Corrente na barra para um curto na barra " << barra << endl;
      arqsaida << "Coluna 4 = Ifalta caso o curto fosse na barra" << endl;

      // Retornar a posicao row e columm
      // do pior caso
      int r,c;
      Saida.col(3).minCoeff(&r,&c);
      arqsaida << "Pior caso ocorre para um curto na barra " << r+1 << endl;


      cout << "\n\nRelatorio salvo em "<< "\""<< Relatorio << "\"\n\n";
      arqsaida.close();

    }
    else if(isatty(fileno(stdin))){
      std::cerr<<R"(Utilização:

      Sintaxe:
      Executavel   Dados de entrada    Saida (opcional)
      ./main         arquivo.xml        Relatorio.txt

      Exemplos:

      ./main <arquivo>.xml  MeuArquivo.txt 
      ./main < <arquivo>.xml
      cat <arquivo>.xml | ./main

)";
      return EXIT_FAILURE;
    }

  } catch (std::exception &e){
      cout << "Erro: " << e.what() << "\n";
  } 


  return EXIT_SUCCESS;

}
