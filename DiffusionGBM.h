//
//  DiffusionGBM.h
//  SiriusFM
//
//  Created by Рамиль Алиев on 10.02.2021.
//

#pragma once
#include <stdexcept>
namespace SiriusFM{
    class DiffusionGBM{
        double const m_muBar;
        double const m_sigmaBar;
        double const m_S0;
    public:
        DiffusionGBM(double a_mu, double a_sigma,double a_S0)
        :m_muBar(a_mu),
        m_sigmaBar(a_sigma),
        m_S0(a_S0){
            if(a_sigma<0)
                throw std::invalid_argument("Sigma is non-negative");
        }
        double mu(double a_S,double a_t)const{
            return m_muBar*a_S;
        }
        double sigma(double a_S,double a_t)const{
            return m_sigmaBar * a_S;
        }
        double startpoint() const{
            return m_S0;
        }
    };
}
