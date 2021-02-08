//
//  diffusion.h
//  SiriusFM
//
//  Created by Рамиль Алиев on 05.02.2021.
//

#pragma once
#include <stdexcept>
#include <cmath>
namespace SiriusFM{
    class DiffusionGBM{
        double const m_mu;
        double const m_sigma;
    public:
        double mu(double a_S,double a_t)const{
            return m_mu*a_S;
        }
        double sigma(double a_S,double a_t)const{
            return m_sigma * a_S;
        }
        DiffusionGBM(double a_mu, double a_sigma) :m_mu(a_mu), m_sigma(a_sigma)
        {
            if(a_sigma<0)
                throw std::invalid_argument("Sigma is non-negative");
        }
    };
    class DiffusionLipton{
        double const m_mu;
        double const m_sigma0;
        double const m_sigma1;
        double const m_sigma2;
    public:
        double mu(double a_S,double a_t)const{
            return m_mu*a_S;
        }
        double sigma(double a_S, double a_t)const{
            return m_sigma0+a_S*m_sigma1+a_S*a_S*m_sigma2;
        }
        DiffusionLipton(double a_mu, double a_sigma0, double a_sigma1, double a_sigma2) :m_mu(a_mu), m_sigma0(a_sigma0), m_sigma1(a_sigma1), m_sigma2(a_sigma2){
            if(a_sigma0<0)
                throw std::invalid_argument("Sigma0 must be non-negative");
            if(a_sigma2<0)
                throw std::invalid_argument("Sigma2 must be non-negative");
            if(a_sigma1*a_sigma1*-4*a_sigma2*a_sigma0>=0)
                throw std::invalid_argument("Discriminant must be negative");
        }
    };
    class DiffusionOU{
        double const m_kappa;
        double const m_theta;
        double const m_sigma;
    public:
        double mu(double S,double t)const{
            return m_kappa*(m_theta-S);
        }
        DiffusionOU(double a_kappa, double a_theta, double a_sigma): m_kappa(a_kappa), m_theta(a_theta), m_sigma(a_sigma){
            if(a_kappa<0)
                throw std::invalid_argument("Kappa must be non-negative");
            if(a_sigma<0)
                throw std::invalid_argument("Sigma must be non-negative");
        }
    };
    class DiffusionCIR{
        double const m_kappa;
        double const m_theta;
        double const m_sigma;
    public:
        double mu(double S,double t)const{
            return m_kappa*(m_theta-S);
        }
        double sigma(double a_S,double a_t)const{
            return m_sigma*sqrt(a_S);
        }
        DiffusionCIR(double a_kappa, double a_theta, double a_sigma): m_kappa(a_kappa), m_theta(a_theta), m_sigma(a_sigma){
            if(a_kappa<0)
                throw std::invalid_argument("Kappa must be non-negative");
            if(a_theta<0)
                throw std::invalid_argument("Theta must be non-negative");
            if(a_sigma<0)
                throw std::invalid_argument("Sigma must be non-negative");
        }
    };
    class DiffusionCEV{
        double const m_mu;
        double const m_sigma;
        double const m_beta;
    public:
        double mu(double a_S,double a_t)const{
            return m_mu*a_S;
        }
        double sigma(double a_S, double a_t)const{
            return m_sigma*pow(a_S,m_beta);
        }
        DiffusionCEV(double a_mu, double a_sigma, double a_beta): m_mu(a_mu), m_sigma(a_sigma), m_beta(a_beta){
            if(a_beta<0)
                throw std::invalid_argument("Beta must be non-negative");
            if(a_sigma<0)
                throw std::invalid_argument("Sigma must be non-negative");
        }
    };
}

