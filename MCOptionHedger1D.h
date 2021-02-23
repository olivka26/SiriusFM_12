//
//  MCOptionHedger1D.h
//  
//
//  Created by Рамиль Алиев on 12.02.2021.
//

#pragma once
#include "MCEngine1D.hpp"
#include "Option.h"
#include <functional>

namespace SiriusFM {
template<typename Diffusion1D,
typename A_Provider,
typename B_Provider,
typename AssetClassA,
typename AssetClassB>
class MCOptionHedger1D{
public:
    using DeltaFunc =
           std::function<double(double, double)>;
private:
    class OHPathEval{
        Option <AssetClassA,AssetClassB>const* const m_option;
        A_Provider const * const m_irpA;
        B_Provider const * const m_irpB;
        double* m_ratesA;
        double* m_ratesB;
        double const m_C0; // intitial option premium
        DeltaFunc const* const m_deltaFunc;
        double const m_DeltaAcc; //rounded to multiple of this
        long m_P; //total paths evaluated
        double m_sumPnL;
        double m_sumPnL2;
        double m_minPnL;
        double m_maxPnL;
    public:
        OHPathEval(Option <AssetClassA,AssetClassB>const* a_option,
                   A_Provider const  * a_irpA,
                   B_Provider const  * a_irpB,
                   double a_C0,
                   DeltaFunc const* a_deltaFunc,
                   double a_DeltaAcc)
        :m_option(a_option),
        m_irpA(a_irpA),
        m_irpB(a_irpB),
        m_ratesA(nullptr),
        m_ratesB(nullptr),
        m_C0(a_C0),
        m_deltaFunc(a_deltaFunc),
        m_DeltaAcc(a_DeltaAcc),
        m_P(0),
        m_sumPnL(0),
        m_sumPnL2(0),
        m_minPnL(INFINITY),
        m_maxPnL(-INFINITY)
        {assert(m_option!=nullptr &&
                m_deltaFunc!=nullptr &&
                m_DeltaAcc>=0 &&
                m_irpA!=nullptr &&
                m_irpB!=nullptr);}
        void operator()(long a_L,
                         long a_PM,
                         double const * a_paths,
                         double const * a_ts){
            //If rates aren't available, compute them
            if(m_ratesA==nullptr){
                m_ratesA=new double[a_L];
                for(long l=0;l<a_L;++l)
                m_ratesA[l]=m_irpA->r(m_option->assetA(),a_ts[l]);
            }
            if(m_ratesB==nullptr){
                m_ratesB=new double[a_L];
                for(long l=0;l<a_L;++l)
                m_ratesB[l]=m_irpB->r(m_option->assetB(),a_ts[l]);
            }
            for(long p=0;p<a_PM;++p){
                double const* path=a_paths+p*a_L;
                //perform delta-hedging
                double M=-m_C0;
                double delta=0.0;
                //double Sp=INFINITY; // we last hedged at this Sp
                for(long l=0;l<a_L;++l){
                    double St=path[l];
                    double t=a_ts[l];
                    if(l>0){
                        double tau=t-a_ts[l-1];
                        double Sp=path[l-1];
                        M+=M*tau*m_ratesB[l-1];
                        //dividends:
                        M+=Sp*tau*m_ratesA[l-1];
                    }
                    if(l < a_L - 1){
                        double deltaN=((*m_deltaFunc)(St,t));
                        deltaN=-round(deltaN/m_DeltaAcc)*m_DeltaAcc;
                        if (deltaN!=delta){
                            M-=(deltaN-delta)*St;
                            delta=deltaN;
                        }
                    }
                }
                double PnL=M+delta*path[a_L-1]+m_option->payoff(a_L,path,a_ts);
                m_sumPnL+=PnL;
                m_sumPnL2+=PnL*PnL;
                m_minPnL = std::min<double>(m_minPnL, PnL);
                m_maxPnL = std::max<double>(m_maxPnL, PnL);
            }
            m_P+=a_PM;
        }
        std::tuple<double,double,double,double> GetStats()const{
            if(m_P<2)
                throw std::runtime_error("empty OPPathEval");
            double mean=m_sumPnL/double(m_P);
            double var=(m_sumPnL2-double(m_P)*mean*mean)/(double(m_P-1));
            assert(var>=0);
            return std::make_tuple(mean,sqrt(var), m_minPnL, m_maxPnL);
        }
        ~OHPathEval(){
            delete[] (m_ratesA);
            delete[] (m_ratesB);
            m_ratesA = nullptr;
            m_ratesB = nullptr;
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
    OHPathEval>m_mce;
    bool m_useTimerSeed;
public:
    MCOptionHedger1D(
        Diffusion1D const * a_diff,
        const char * a_irs_fileA,
        const char * a_irs_fileB,
        bool a_useTimerSeed)
    :m_diff(a_diff),
    m_irpA(a_irs_fileA),
    m_irpB(a_irs_fileB),
    m_mce(102271,4096), //5-min points, 4096 paths
    m_useTimerSeed(a_useTimerSeed)
    {}
    std::tuple<double, double, double, double> SimulateHedging(Option <AssetClassA, AssetClassB> const* a_option,
                                                               time_t a_t0,
                                                               double a_C0,
                                                               DeltaFunc const* a_deltaFunc,
                                                               double              a_DeltaAcc,
                                                               int a_tau_mins=15,
                                                               long a_P=100000);
    double GetRateA(AssetClassA a_assetA, double a_ty) const
         { return m_irpA.r(a_assetA, a_ty); }
    double GetRateB(AssetClassB a_assetB, double a_ty) const
         { return m_irpB.r(a_assetB, a_ty); }
};
}
