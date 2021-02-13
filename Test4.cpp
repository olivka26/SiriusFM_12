//
//  Test4.cpp
//  
//
//  Created by Рамиль Алиев on 12.02.2021.
//

#include "DiffusionGBM.h"
#include "VanillaOptions.h"
#include "MCOptionHedger1D.hpp"
#include "Time.h"
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <cmath>
using namespace std;
using namespace SiriusFM

namespace{
//CDF OF the standard normal
inline double Phi(double x){
    return 0.5*(1+erf(x/M_SQRT2));
}
//BSM Pricer
inline double BSMPxCall(double a_S0, double a_K, double a_TTE,  double a_rateA,  double  a_rateB, double a_sigma){
    assert(a_S0>0 && a_K>0 && a_sigma>0);
    if(a_TTE<=0)
        return std::max<double>(a_S0 - a_K, 0);
    double xd=a_sigma*sqrt(a_Ty);
    double x1=(log(a_S0/a_K)+(a_rateB-a_rateA+a_sigma*a_sigma/2.0)*a_TTE)/xd;
    double x2=x1-xd;
    double px=a_S0*exp(-a_rateA*a_TTE)*Phi(x1)-a_K*exp(-a_rateB*a_TTE)*Phi(x2);
    return px;
}
inline double BSMPxPut(double a_S0, double a_K, double a_TTE,  double a_rateA,  double  a_rateB, double a_sigma){
    //assert(a_S0>0 && a_K>0 && a_TTE>0 && a_sigma>0);
    double px=BSMPxCall(a_S0, a_K, a_TTE, a_rateA, a_rateB, a_sigma) - a_S0 + exp(-a_rateB * a_TTE) * a_K;
    assert(px>0.0);
    return px;
}
inline double BSMDeltaPut(double a_S0, double a_K, double a_TTE, double a_rateA, double a_rateB, double a_sigma){
    assert(a_S0 > 0 && a_K > 0 && a_sigma > 0);
        if (a_TTE <= 0)
          return (a_S0 < a_K) ? 0 : (a_S0 > a_K) ? 1 : 0.5;
        double xd = a_sigma * sqrt(a_TTE);
        double x1 =
          (log(a_S0 / a_K) +
            (a_rateB - a_rateA + a_sigma * a_sigma / 2.0) * a_TTE) / xd;
        return Phi(x1);}
inline double BSMDeltaPut (double a_S0,double a_K,double a_TTE,double a_rateA, double a_rateB, double a_sigma)
  {
    return BSMDeltaCall(a_S0, a_K, a_TTE, a_rateA, a_rateB, a_sigma) - 1.0;
  }
}

int main(int argc, const char * argv[]){
    if (argc != 10){
        cerr << "Params: mu, sigma, s0, T_days, tau_min, P,Call/Put,K" << endl;
        return 1;
    }
    double mu = atof(argv[1]);
    double sigma = atof(argv[2]);
    double S0 = atof(argv[3]);
    const char* OptType = argv[4];
    double K = atof(argv[5]);
    long T_days = atol(argv[6]);
    double deltaAcc = atof(argv[7]);
    int tau_min = atoi(argv[8]);
    long P = atol(argv[9]);
   // printf("%s\n",OptType);
    assert(sigma > 0 &&
           S0 > 0 &&
           T_days > 0 &&
           tau_min > 0 &&
           P > 0 &&
           K > 0);
    CcyE ccyA = CcyE::USD;
    CcyE ccyB = CcyE::USD;
    
    char const* ratesFileA = nullptr;
    char const* ratesFileB = nullptr;
    
    bool useTimerSeed = true;
    DiffusionGBM diff(mu, sigma,S0);
    MCOptionHedger1D<decltype(diff),IRPConst,IRPConst,CcyE,CcyE>hedger(&diff,ratesFileA, ratesFileB,useTimerSeed);
    time_t t0 = time(nullptr);
    time_t T = t0 + SEC_IN_DAY * T_days;
    double TTE = YearFracInt(T - t0);
    double Ty=EPOCH_BEGIN + double(T) / AVG_SEC_IN_YEAR;
    OptionFX const * option = nullptr;
    decltype(hedger)::DeltaFunc const* deltaFunc = nullptr;
    double C0=0.0;
    double rateA=hedger.GetRateA(ccyA,0.0);
    double rateB=hedger.GetRateB(ccyB,0.0);
    function<double (double,double)> deltaCall(
[K, Ty, rateA, rateB, sigma](double a_St,double a_t)->double{ //a_t - current time not expiration
        double currTTE = Ty - a_t;
        return BSMDeltaCall(a_St,K,currTTE,rateA,rateB,sigma);
    }
    );
    function<double(double,double)> deltaPut
        (
          [K, Ty, rateA, rateB, sigma]
          (double a_St, double a_t) -> double
          {
            double currTTE = Ty - a_t;
            return BSMDeltaPut (a_St, K, currTTE, rateA, rateB, sigma);
          }
        );
    if(strcmp(OptType,"Call")==0){
        option = new EurCallOptionFX(ccyA, ccyB, K, T);
        C0=BSMPxCall(S0,K,TTE,rateA,rateB,sigma);
        deltaFunc=&deltaCall;
    }
    else if(strcmp(OptType,"Put")==0){
        option = new EurPutOptionFX(ccyA, ccyB, K, T);
        C0=BSMPxPut(S0,K,TTE,rateA,rateB,sigma);
        deltaFunc=&deltaPut;
    }
    else
        throw invalid_argument("Invalid option type");
    auto res=hedger.SimulateHedging(option,t0,C0,deltaFunc,deltaAcc,tau_min,P);
  //  OPPathEval *pathEval(option);
    
    double EPnL   = get<0>(res);
    double StDPnL = get<1>(res);
    double MinPnL = get<2>(res);
    double MaxPnL = get<3>(res);
    cout<<EPnL<<" "<<StdPnL<<" "<<MinPnL<<" "<<MaxPnL;
    delete option;
    return 0;
}
