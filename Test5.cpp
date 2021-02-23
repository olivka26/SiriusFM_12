//
//  Test5.cpp
//  
//
//  Created by Рамиль Алиев on 13.02.2021.
//

#include "IRProvideConst.h"
#include "DiffusionGBM.h"
#include "VanillaOptions.h"
#include "GridNOP1D_S3_RKC1.hpp"
#include "BSM.hpp"
#include <iostream>
#include <cstring>
#include <cmath>

using namespace SiriusFM;
using namespace std;

int main(int argc, char** argv){
    if(argc != 10){
        cerr << "PARAMS:\nsigma, S0,\n{Call/Put}, K, Tdays,\nNS, tauMins\n";
        return 1;
    }
    double sigma=atof(argv[1]);
    double S0=atof(argv[2]);
    const char* OptType=argv[3];
    double K=atof(argv[4]);
    long Tdays=atol(argv[5]);
    bool isAmerican=bool(atoi(argv[6]));
    char const* ratesFile=argv[7];
    int NS=atol(argv[8]);
    int tauMins=atoi(argv[9]);
    assert(sigma > 0 && S0 > 0 && K > 0 && Tdays > 0 && NS > 0 && tauMins > 0);
    CcyE ccyA = CcyE::USD;
    CcyE ccyB = CcyE::RUB;
    DiffusionGBM diff(0.0, sigma, S0);
    time_t t0 =time(nullptr);
    time_t T=t0+SEC_IN_DAY*Tdays;
    OptionFX const* opt=nullptr;
    if(strcmp(OptType, "Call")==0)
        opt=new CallOptionFX(ccyA, ccyB, K, T,isAmerican);
    else if(strcmp(OptType, "Put")==0)
            opt=new PutOptionFX(ccyA, ccyB, K, T,isAmerican);
        else
            throw invalid_argument("Bad option type");
    GridNOP1D_S3_RKC1<decltype(diff), IRPConst, IRPConst, CcyE, CcyE>grid(ratesFile, ratesFile);
    grid.Run<false>(opt, &diff, S0, t0, NS, tauMins);
    auto res=grid.GetPxDeltaGamma0();
    double px=get<0>(res);
    double delta=get<1>(res);
    double gamma=get<2>(res);
    cout << "Px=" << px << "\nDelta=" << delta << "\nGamma=" << gamma << endl;
    delete opt;
    return 0;
}
//Test 5 supposed to calculate DF*convolution between the density and payoff, OP=DF*convolution
