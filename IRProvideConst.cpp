//
//  IRProvideConst.cpp
//  SiriusFM
//
//  Created by Рамиль Алиев on 06.02.2021.
//

#include "IRProvideConst.h"

namespace SiriusFM{
    IRProvider <IRModeE::Const>::IRProvider(char const* a_file){
        if(!a_file)
            return;
        FILE *f=fopen(a_file, "r");
        if(!f)
            throw std::invalid_argument("Bad file");
        for(int i=0; i<=int(CcyE::JPY); ++i){
            char buff[4];
            double r;
            fscanf(f,"%s %lf",buff,&r);
            CcyE a_ccy=Str2CcyE(buff);
            m_IRS[int(a_ccy)]=r;
        }
        fclose(f);
    }
}
