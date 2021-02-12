//
//  DiffusionCEV.h
//  SiriusFM
//
//  Created by Рамиль Алиев on 10.02.2021.
//

#pragma once
#include <stdexcept>
#include <cmath>
namespace SiriusFM{
    class DiffusionCEV{
        double const m_mu;
        double const m_sigma;
        double const m_beta;
    public:
        DiffusionCEV(double a_mu, double a_sigma, double a_beta): m_mu(a_mu), m_sigma(a_sigma), m_beta(a_beta){
            if(a_beta<0)
                throw std::invalid_argument("Beta must be non-negative");
            if(a_sigma<0)
                throw std::invalid_argument("Sigma must be non-negative");
        }
        double mu(double a_S,double a_t)const{
            return m_mu*a_S;
        }
        double sigma(double a_S, double a_t)const{
            return m_sigma*pow(a_S,m_beta);
        }
    };
}
