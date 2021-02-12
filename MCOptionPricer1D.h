//
//  MCOptionPricer.h
//  
//
//  Created by Рамиль Алиев on 11.02.2021.
//

#pragma once
#include "MCEngine1D.hpp"
#include "Option.h"
#include <cmath>

namespace SiriusFM {
template<typename Diffusion1D,
typename A_Provider,
typename B_Provider,
typename AssetClassA,
typename AssetClassB>
class MCOptionPricer1D{
    class OPPathEval{
        Option <AssetClassA,AssetClassB>const* const m_option;
        long m_P; //total paths evaluated
        double m_sum;//sum of payoffs
        double m_sum2;//sum of payoff^2 -s
        double m_minP0;//min payoff
        double m_maxP0;//max payoff
    public:
        OPPathEval(Option <AssetClassA,AssetClassB> const* a_option)
        :m_option(a_option),
        m_P(0),
        m_sum(0),
        m_sum2(0),
        m_minP0(INFINITY),
        m_maxP0(-INFINITY)
        {assert(m_option!=nullptr);}
        void operator()(long a_L, long a_PM, double const * a_paths, double const * a_ts){
            for(long p=0;p<a_PM;++p){
                double const* path=a_paths+p*a_L;
                double payoff=m_option->payoff(a_L,path,a_ts);
                m_sum+=payoff;
                m_sum2+=payoff*payoff;
                m_minP0 = std::min<double>(m_minP0, payoff);
                m_maxP0 = std::max<double>(m_maxP0, payoff);
            }
            m_P+=a_PM;
        } //returns (EPx,StdDevPx)
        double GetPx() const{
            if (m_P < 2)
                throw std::runtime_error("Empty OPPathEval");
                return m_sum / double(m_P);
        }
        std::tuple<double,double,double> GetPxStats()const{
            if(m_P<2)
                throw std::runtime_error("empty OPPathEval");
            double px=m_sum/double(m_P);
            double var=(m_sum2-double(m_P)*px*px)/(double(m_P-1));
            assert(var>=0);
           // double err=(px!=0)? sqrt(var)/fabs(px) : sqrt(var);
            return std::make_pair(sqrt(var), m_minP0, m_maxP0);
        }
    };
    Diffusion1D const* const m_diff;
    A_Provider m_irpA;
    B_Provider m_irpB;
    MCEngine1D <Diffusion1D,
    A_Provider,
    B_Provider,
    AssetClassA,
    AssetClassB,
    OPPathEval>m_mce;
    bool m_useTimerSeed;
public:
    MCOptionPricer1D(
        Diffusion1D const * a_diff,
        const char * a_irs_fileA,
        const char * a_irs_fileB,
        bool a_useTimerSeed)
    :m_diff(a_diff),
    m_irpA(a_irs_fileA),
    m_irpB(a_irs_fileB),
    m_mce(102271,4096),
    m_useTimerSeed(a_useTimerSeed)
    {}
    double Px(Option <AssetClassA, AssetClassB> const* a_option,
              time_t a_t0,
              int a_tau_mins=15,
              long a_P=100000);
};
}
