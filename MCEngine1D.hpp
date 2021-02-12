//
//  MCEngine1D.hpp
//  SiriusFM
//
//  Created by Рамиль Алиев on 10.02.2021.
//

#pragma once
#include <cassert>
#include <random>
#include "MCEngine1D.h"
#include "Time.h"

namespace SiriusFM{
template<
typename Diffusion1D,
typename A_Provider,
typename B_Provider,
typename AssetClassA,
typename AssetClassB,
typename PathEvaluator>
template <bool IsRN>
inline void MCEngine1D<Diffusion1D,
A_Provider,
B_Provider,
AssetClassA,
AssetClassB,
PathEvaluator>::Simulate
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
  PathEvaluator* a_pathEval)
{
assert(a_diff!=nullptr &&
       a_rateA!=nullptr &&
       a_rateB!=nullptr &&
       a_t0<=a_T &&
       a_A!=AssetClassA::UNDEFINED &&
       a_B!=AssetClassB::UNDEFINED &&
       a_tau_mins>0 &&
       a_P>0 &&
       a_pathEval != nullptr);
    double y0=YearFrac(a_t0);
    time_t T_sec = a_T - a_t0;
    time_t tau_sec = a_tau_mins * SEC_IN_MIN;
    long L_segm = (T_sec % tau_sec == 0) ? T_sec/tau_sec : T_sec / tau_sec + 1;
    double tau = YearFracInt(tau_sec);
    long L = L_segm + 1;
    long P = 2 * a_P;
    if(L>m_MaxL)
        throw std::invalid_argument("too many steps");
    double stau = sqrt(tau);
    double tlast = (T_sec % tau_sec == 0) ? tau : YearFracInt(T_sec - (L - 1) * tau_sec);
    assert(tlast <= tau && tlast > 0);
    double slast=sqrt(tlast);
    assert(slast<=stau && slast>0);
    for(long l=0;l<L-1;++l)
        m_ts[l]=y0+double(l)*tau;
    m_ts[L-1]=m_ts[L-2]+tlast;
    std::normal_distribution <>N01(0.0,1.0);
    std::mt19937_64 U(a_useTimerSeed ? time(nullptr) :0);
    long PM=(m_MaxL * m_MaxPM)/L; //Number of paths we can store in memory
    if(PM%2!=0)
        --PM;
    assert(PM>0 && PM%2==0);
    long PMh=PM/2;
    long PI=(P%PM==0) ? (P/PM) : (P/PM+1); //max number of outer p iterations
  //  P=PI*PM;
    for(long i=0;i<PI;++i){
    for(long p=0;p<PMh;++p){
        double * path0=m_paths+2*p*L;
        double * path1=path0+L;
        path0[0]=a_diff->startpoint();
        path1[0]=a_diff->startpoint();
        double Sp0 = a_diff->startpoint();
        double Sp1 = a_diff->startpoint();
        for(long l=1;l<L;++l){
            double mu0=0.0, mu1=0.0,y=m_ts[L-1];
            if(IsRN){
                double delta_r=a_rateB->r(a_B,y)-a_rateA->r(a_A,y);
                mu0=delta_r*Sp0;
                mu1=delta_r*Sp1;
            }else{
                mu0=a_diff->mu(Sp0,y);
                mu1=a_diff->mu(Sp1,y);
            }
            double sigma0=a_diff->sigma(Sp0,y);
            double sigma1=a_diff->sigma(Sp1,y);
            double Z=N01(U),Sn0=0,Sn1=0;
            if(l==L-1){
                Sn0=Sp0+mu0*tlast+sigma0*Z*slast;
                Sn1=Sp1+mu1*tlast-sigma1*Z*slast;
            }else{
                Sn0=Sp0+mu0*tau+sigma0*Z*stau;
                Sn1=Sp1+mu1*tau-sigma1*Z*stau;
            }
            path0[l]=Sn0;
            path1[l]=Sn1;
            Sp0=Sn0;
            Sp1=Sn1;
        } //end of l loop, evaluate the in-memory paths
    } (*a_pathEval)(L,PM,m_paths,m_ts);//end of p loop
    } //end of i loop
}
}
