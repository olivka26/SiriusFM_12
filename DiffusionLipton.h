//
//  DiffusionLipton.h
//  SiriusFM
//
//  Created by Рамиль Алиев on 10.02.2021.
//

#pragma once
#include <stdexcept>
#include <cmath>
namespace SiriusFM{
    class DiffusionLipton{
        double const m_mu;
        double const m_sigma0;
        double const m_sigma1;
        double const m_sigma2;
    public:
        DiffusionLipton(double a_mu, double a_sigma0, double a_sigma1, double a_sigma2) :m_mu(a_mu), m_sigma0(a_sigma0), m_sigma1(a_sigma1), m_sigma2(a_sigma2){
            if(a_sigma0<0)
                throw std::invalid_argument("Sigma0 must be non-negative");
            if(a_sigma2<0)
                throw std::invalid_argument("Sigma2 must be non-negative");
            if(a_sigma1*a_sigma1*-4*a_sigma2*a_sigma0>=0)
                throw std::invalid_argument("Discriminant must be negative");
        }
        double mu(double a_S,double a_t)const{
            return m_mu*a_S;
        }
        double sigma(double a_S, double a_t)const{
            return m_sigma0+a_S*m_sigma1+a_S*a_S*m_sigma2;
        }
    };
}
