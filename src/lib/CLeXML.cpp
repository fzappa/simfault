//**********************************************
//  Class for read bus data from XML
//
//  Implemented for study purposes.
//  Uses the Eigen (http://eigen.tuxfamily.org/)
//  library to facilitate the matrix treatment.
//
//  Uses the Boost Library for XML (http://www.boost.org/)
//
//  Types T accepted: float, double e complex<double>
//
//  License: GPLv3
//  Author: Alan Franco - https://github.com/fzappa
//  Rev.: 06/05/2017 (PT_BR)
//*********************************************

#include <iostream>

#include <Eigen/Dense>
#include <boost/algorithm/string/trim.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "CLeXML.hpp"
#include "Definicoes.hpp"

#define ATTR_SET ".<xmlattr>"

using namespace boost::property_tree;

namespace XML {

// Constructor
CLeXML::CLeXML(const std::string &A) { pArquivo = A; }

CLeXML::CLeXML(const std::string &A, const std::string &E)
{
    pArquivo  = A;
    pElemento = E;
    pLeGera();
}

// Methods
void CLeXML::setArquivo(const std::string &A)
{
    pArquivo = A;
    pLeGera();
}
std::string CLeXML::getArquivo(void) const { return pArquivo; }

void CLeXML::setElemento(const std::string &B)
{
    pElemento = B;
    pLeGera();
}
std::string CLeXML::getElemento(void) const { return pElemento; }

Matriz CLeXML::getMatriz(void) const
{
    if (pArquivo == "")
        throw std::invalid_argument("Arquivo de entrada não definido!");
    else
        return pMatriz;
}

int CLeXML::getLin(void) const { return pLin; }
int CLeXML::getCol(void) const { return pCol; }

// Member function - XML
const inline ptree &CLeXML::limpa_ptree(void) const
{
    static ptree t;
    return t;
}
inline void CLeXML::pZera(void)
{
    pLin  = 0;
    pElem = 0;
    pCol  = 0;
}

void CLeXML::pLeGera(void)
{  // Le os dados do XML e gera a matriz
    try {
        pZera();
        ptree ArqXML;  // ptree do Boost
        read_xml(this->getArquivo(), ArqXML);
        std::vector<std::string> Dados;  // Recebe todos os atributos em sequencia

        // ptree le todas as linhas e ignora os comentarios
        const ptree &Elemento = ArqXML.get_child(this->getElemento(), limpa_ptree());
        BOOST_FOREACH (const ptree::value_type &f, Elemento) {
            if (f.first != "<xmlcomment>")
                pLin++;  // faz o incremento a cada linha que nao seja comentario

            // em cada linha, pega todos os atributos
            std::string at = f.first + ATTR_SET;
            const ptree &Atributos =
                f.second.get_child("<xmlattr>", limpa_ptree());  // limpa o ptree
            BOOST_FOREACH (const ptree::value_type &v, Atributos) {
                if (v.first != "<xmlcomment>" &&
                    !v.second.data().empty())          // se nao for comentario ou nao estiver vazio
                    Dados.push_back(v.second.data());  // Armazena todos os atributos em um vetor
                else {
                    // throw ExcecaoES(":LeXML::Nao ha atributos na tag \"" + this->getArquivo() +
                    // "\"");
                    std::cerr << "\n[LeXML] Nao ha atributos na tag \"" << v.first << "\" do ID "
                              << pLin << "\ndo elemento raiz " << this->getElemento()
                              << " do arquivo " << this->getArquivo() << "\n\n";
                    exit(100);
                }
            }
        }

        // Gera a matriz
        // Quantidade de atributos = numero de atributos por elemento x numero de elementos
        // dados por barra (pCol) = Total de dados / total de barras (pLin)
        // Como ja foi contado o numero de linhas e atributos por linha
        // vetor sera -> NumeroDeLinhas x NumeroDeAtributos(por linha) = Tamanho do vetor Dados
        if (Dados.size() > 0) {  // Se existirem dados para o elemento, gera a matriz

            pCol    = Dados.size() / pLin;
            pMatriz = Matriz::Zero(pLin, pCol);

            // Cria a matriz com os dados
            // Le todo o vetor Dados
            // e quando o numero de atributos por linha for o maximo,
            // muda a linha da matriz e continua a leitura
            uint k{0};
            for (uint i = 0; i < pLin; ++i) {
                for (uint j = 0; j < pCol; ++j) {
                    boost::algorithm::trim(Dados[k]);  // remover espacos
                    pMatriz(i, j) = boost::lexical_cast<tipo>(Dados[k]);
                    ++k;
                }
            }

        }  // if(Dados.size()
    }
    catch (const boost::property_tree::xml_parser::xml_parser_error &ex) {
        std::cerr << "Erro no arquivo " << ex.filename() << " linha " << ex.line() << std::endl;
        exit(100);
    }

}  // pLeGera

}  // namespace XML
