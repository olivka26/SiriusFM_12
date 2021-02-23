//
//  Test2.cpp
//  
//
//  Created by Рамиль Алиев on 10.02.2021.
//

#include <iostream>
#include "Option.h"
#include "VanillaOptions.h"
#include "DiffusionGBM.h"
#include "MCEngine1D.hpp"
#include "IRProvideConst.h"
#include "MCOptionPricer1D.hpp"

using namespace  SiriusFM;
using namespace std;

int main(int argc, const char * argv[]){
    if (argc != 9){
        cerr << "Params: mu, sigma, s0, T_days, tau_min, P,Call/Put,K" << endl;
        return 1;
    }
    double mu = atof(argv[1]);
    double sigma = atof(argv[2]);
    double S0 = atof(argv[3]);
    long T_days = atol(argv[4]);
    int tau_min = atoi(argv[5]);
    long P = atol(argv[6]);
    const char* OptType = argv[7];
    printf("%s\n",OptType);
    double K = atof(argv[8]);
    assert(sigma > 0 && S0 > 0 && T_days > 0 && tau_min > 0 && P > 0 && K > 0);
    CcyE ccyA = CcyE::USD;
    CcyE ccyB = CcyE::USD;
    IRPConst irp("file.txt");
    DiffusionGBM diff(mu, sigma,S0);
    OptionFX const * option = (strcmp(OptType,"Call")==0)? static_cast<Option*>(new CallOption(K,T_days,false))
    : (strcmp(OptType,"Put")==0) ? static_cast<Option*>(new PutOption(K,T_days,false)) : throw invalid_argument("Invalid option type");
    OPPathEval *pathEval(option);
    MCEngine1D<DiffusionGBM, decltype(irp), decltype(irp), CcyE, CcyE,OPPathEval> mce(20000, 20000);
    time_t t0 = time(nullptr);
    time_t T = t0 + 86400.00 * T_days;
    mce.Simulate<true>(t0, T, tau_min, P,true,  &diff, &irp, &irp, ccyA, ccyB,&pathEval);
    auto res=pathEval.GetPxStats();
    double px=res.first;
    double err=res.second;
    cout<<px<<" "<<err;
    double EST = 0.0;
    double EST2 = 0.0;
    int N = 0;
    for(int p = 0; p < P; ++p){
        double const* path = paths + p * L1;
        double ST = path[L1-1];
        if(ST <= 0)
            continue;
        ++N;
        double RT = option->payoff(L1, nullptr, path);
        EST += RT;
        EST2 += RT * RT;
    }
    assert(N > 1);
    EST /= double(N);
    double VarST = (EST2 - double(N) * EST * EST )/ double(N-1);
    cout << "Expected val. " << EST << endl;
    cout << "STD " << sqrt(VarST) << endl;
}

//expected value variance of payoff in option GBM
