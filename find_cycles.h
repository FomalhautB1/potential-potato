#ifndef FIND_CYCLES_H
#define FIND_CYCLES_H

#include <openbabel/obconversion.h>
#include <openbabel/mol.h>
#include <iostream>
#include <vector>
#include <openbabel/ring.h>

#ifdef __cplusplus
extern "C" {
#endif

void count_cycles(const std::string& filename);

#ifdef __cplusplus
}
#endif

#endif