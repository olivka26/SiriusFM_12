//
//  DiffusionCIR.h
//  SiriusFM
//
//  Created by Рамиль Алиев on 10.02.2021.
//

#pragma once
#include <stdexcept>
#include <cmath>
namespace SiriusFM{
    class DiffusionCIR{
        double const m_kappa;
        double const m_theta;
        double const m_sigma;
    public:
        DiffusionCIR(double a_kappa,
                     double a_theta,
                     double a_sigma):
        m_kappa(a_kappa),
        m_theta(a_theta),
        m_sigma(a_sigma){
            if(a_kappa<0)
                throw std::invalid_argument("Kappa must be non-negative");
            if(a_theta<0)
                throw std::invalid_argument("Theta must be non-negative");
            if(a_sigma<0)
                throw std::invalid_argument("Sigma must be non-negative");
        }
        double mu(double a_S,double a_t)const{
            return m_kappa*(m_theta-a_S);
        }
        double sigma(double a_S,double a_t)const{
            return m_sigma*sqrt(a_S);
        }
    };
}
