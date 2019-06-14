#ifndef FUNCOESAUX_H_
#define FUNCOESAUX_H_
//**********************************************
//  Functions for Modifications of existing Ybus and Zbus. (C++14)
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

#include "CLeXML.hpp"
#include "Definicoes.hpp"

void AdequaValores(Matriz &, const double &);
void OrganizaLinhas(Matriz &);
void OrganizaBarras(Matriz &);

Matriz MontaYbarra(XML::CLeXML &);
Matriz MontaZbarra(XML::CLeXML &);

#endif
