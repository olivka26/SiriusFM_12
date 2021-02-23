//
//  MCEngine1D.h
//  SiriusFM
//
//  Created by Рамиль Алиев on 10.02.2021.
//

#pragma once
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <ctime>
#include"Time.h"

namespace SiriusFM {
template<typename Diffusion1D,
typename A_Provider,
typename B_Provider,
typename AssetClassA,
typename AssetClassB,
typename PathEvaluator>
class MCEngine1D{
    long const m_MaxL; //Max Path Length
    long const m_MaxPM; //Max Number of Stored
    double* const m_paths; //Stored Paths
    double* const m_ts; //Timeline
    /*
    bool m_isRN;
    double m_tau;
    double m_t0;
    Diffusion1D const* m_diff;
    A_Provider const* m_rateA;
    B_Provider const* m_rateB;
    AssetClassA m_A;
    AssetClassB m_B;
     */
public:
    MCEngine1D(long a_MaxL, long a_MaxPM)
    :m_MaxL(a_MaxL),
    m_MaxPM(a_MaxPM),
    m_paths(new double[m_MaxL* m_MaxPM]),
    m_ts(new double[m_MaxL]){
         if(m_MaxL <=0 || m_MaxPM<=0)
            throw std::invalid_argument("bad MaxL or MaxPM");
         for (long l=0; l<m_MaxL; ++l){
             m_ts[l]=0;
             long lp=l*m_MaxPM;
             for(long p=0;p<m_MaxPM;++p)
                 m_paths[lp+p]=0;
         }
    }
    ~MCEngine1D(){
        delete[] m_paths;
        delete[] m_ts;
    }
    MCEngine1D (MCEngine1D const&) = delete;
    MCEngine1D& operator=(MCEngine1D const&) = delete;
    template <bool IsRN>
    void Simulate
    (time_t a_t0,
     time_t a_T,
     int a_tau_mins,
     long a_P,
     bool a_useTimerSeed,
     Diffusion1D const* a_diff,
     A_Provider const *a_rateA,
     B_Provider const* a_rateB,
     AssetClassA a_A,
     AssetClassB a_B,
     PathEvaluator* a_pathEval);
};
}
