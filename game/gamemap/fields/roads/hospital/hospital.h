/* Copyright (C) 2017 Mikhail Masyagin */

#ifndef HOSPITAL_H_
#define HOSPITAL_H_

#include <cinttypes>
#include "../roads.h"

struct Hospital : public Roads {
    explicit Hospital(int32_t my_aids);

    int32_t my_aids;
};

#endif  // HOSPITAL_H_
