#include <gtest/gtest.h>
#include "Simulation.h"
#include<unistd.h> 

#ifdef GOMC_CUDA
TEST(CellListGPU, CheckMETHANOL) {
    int result = chdir("./test/input/Systems/METHANOL_OPLSAA/Standard");
    Simulation base("in_GCMC.conf");
    std::vector<int> cellVector, cellStartIndex, mapParticleToCell;
    std::vector< std::vector<int> > neighborList;
    std::vector<int> cellVectorGPU, cellStartIndexGPU, mapParticleToCellGPU;
    std::vector< std::vector<int> > neighborListGPU;
    
    base.GetCPUCellList(cellVector,
                        cellStartIndex,
                        mapParticleToCell,
                        neighborList);

    base.GetGPUCellList(cellVectorGPU,
                        cellStartIndexGPU,
                        mapParticleToCellGPU,
                        neighborListGPU);

}
#endif