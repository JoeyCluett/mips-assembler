#pragma once

#include "assembler-data-tables.h"

class EvalException {
public:

    AssemblerData* ad;
    
    EvalException(AssemblerData* ad) {  
        this->ad = ad;
    }

};


