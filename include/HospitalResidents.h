#ifndef HOSPITAL_RESIDENTS_H
#define HOSPITAL_RESIDENTS_H

#include "MatchingAlgorithm.h"

class HospitalResidents : public MatchingAlgorithm {
public:
    HospitalResidents(BipartiteGraph* G);
    virtual ~HospitalResidents();

    void compute_matching();
};

#endif
