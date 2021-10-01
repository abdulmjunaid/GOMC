#include <gtest/gtest.h>
#include "PDBSetup.h"
#include "MolSetup.h"
#include "BondAdjacencyList.h"
#include "ConfigSetup.h"
#include "FFSetup.h"        //For geometry kinds
#include "FFConst.h"
#include "Reader.h"
#include "InputFileReader.h"
#include "Simulation.h"
#include<unistd.h> 
/* There are 4 cases for restarting from checkpoint 

1) Base Case:
    The first run.  
    Checkpoint Setup  - RestartFromCheckpoint = false
    Checkpoint Output - RestartFromCheckpoint = false

2) The first restart from checkpoint
    Checkpoint Setup  - RestartFromCheckpoint = true
    Checkpoint Output - RestartFromCheckpoint = true

3) The Nth restart from checkpoint from a prev checkpoint
    Checkpoint Setup  - RestartFromCheckpoint = true
    Checkpoint Output - RestartFromCheckpoint = true

*/
TEST(ConsistentTrajectoryTest, CheckPDBTrajCoordinates) {
    chdir("./test/input/ConsistentTrajectory/Base/");
    Simulation base("in.conf");
    base.RunSimulation();
    chdir("../K_1");
    Simulation K_1("in.conf");
    K_1.RunSimulation();
    chdir("../K_N");
    Simulation K_N("in.conf");
    K_N.RunSimulation();
    chdir("../../../..");

    config_setup::RestartSettings rsStart;
    config_setup::RestartSettings rsBase;
    config_setup::RestartSettings rs1;
    config_setup::RestartSettings rsN;


    std::string pdbnamesSTART[2], pdbnamesBase[2], pdbnames1[2], pdbnamesN[2];
    std::string pdbnamesBaseRestart[2], pdbnames1Restart[2], pdbnamesNRestart[2];

    pdbnamesSTART[0] = "./test/input/ConsistentTrajectory/Base/START_BOX_0.pdb";
    pdbnamesSTART[1] = "./test/input/ConsistentTrajectory/Base/START_BOX_1.pdb";

    pdbnamesBase[0] = "./test/input/ConsistentTrajectory/Base/base_BOX_0.pdb";
    pdbnamesBase[1] = "./test/input/ConsistentTrajectory/Base/base_BOX_1.pdb";

    pdbnames1[0] = "./test/input/ConsistentTrajectory/K_1/K_1_BOX_0.pdb";
    pdbnames1[1] = "./test/input/ConsistentTrajectory/K_1/K_1_BOX_1.pdb";

    pdbnamesN[0] = "./test/input/ConsistentTrajectory/K_N/K_N_BOX_0.pdb";
    pdbnamesN[1] = "./test/input/ConsistentTrajectory/K_N/K_N_BOX_1.pdb";

    pdbnamesBaseRestart[0] = "./test/input/ConsistentTrajectory/Base/base_BOX_0_restart.pdb";
    pdbnamesBaseRestart[1] = "./test/input/ConsistentTrajectory/Base/base_BOX_1_restart.pdb";

    pdbnames1Restart[0] = "./test/input/ConsistentTrajectory/K_1/K_1_BOX_0_restart.pdb";
    pdbnames1Restart[1] = "./test/input/ConsistentTrajectory/K_1/K_1_BOX_1_restart.pdb";

    pdbnamesNRestart[0] = "./test/input/ConsistentTrajectory/K_N/K_N_BOX_0_restart.pdb";
    pdbnamesNRestart[1] = "./test/input/ConsistentTrajectory/K_N/K_N_BOX_1_restart.pdb";

    PDBSetup pdbStart, pdbBase, pdb1, pdbN;
    PDBSetup pdbBaseRestart, pdb1Restart, pdbNRestart;

    rsStart.recalcTrajectory = false;
    rsBase.recalcTrajectory = true;
    rs1.recalcTrajectory = true;
    rsN.recalcTrajectory = true;

    // This is needed to get passed Remark 
    uint frameNum = 1;
    pdbStart.Init(rsStart, pdbnamesSTART);
    pdbBase.Init(rsBase, pdbnamesBase, frameNum);    
    pdb1.Init(rs1, pdbnames1, frameNum);
    pdbN.Init(rsN, pdbnamesN, frameNum);

    frameNum = 11;
    pdbBase.Init(rsBase, pdbnamesBase, frameNum);    
    pdb1.Init(rs1, pdbnames1, frameNum);
    pdbN.Init(rsN, pdbnamesN, frameNum);

    config_setup::RestartSettings rsBaseRestart;
    config_setup::RestartSettings rs1Restart;
    config_setup::RestartSettings rsNRestart;

    rsBaseRestart.restartFromCheckpoint = true;
    rs1Restart.restartFromCheckpoint = true;
    rsNRestart.restartFromCheckpoint = true;

    pdbBaseRestart.Init(rsBaseRestart, pdbnamesBaseRestart);    
    pdb1Restart.Init(rs1Restart, pdbnames1Restart);
    pdbNRestart.Init(rsNRestart, pdbnamesNRestart);
    

    // Checks if the coordinates of the traj match the restart file
    for (uint i = 0; i < pdbBase.atoms.count; ++i){
        /* Find mol i's chain index in restart output files */
        ptrdiff_t pos = find(pdbBaseRestart.atoms.chainLetter.begin(), 
            pdbBaseRestart.atoms.chainLetter.end(), pdbBase.atoms.chainLetter[i])
            - pdbBaseRestart.atoms.chainLetter.begin();
        if(pdbBase.atoms.x[i] != pdbBaseRestart.atoms.x[pos])
            std::cout << pdbBase.atoms.x[i] << " " << i << " " << pdbBaseRestart.atoms.x[pos] << " " << pos <<  std::endl;
        EXPECT_EQ(pdbBase.atoms.x[i] == pdbBaseRestart.atoms.x[pos], true);
    }

    // Checks if the coordinates of the traj match the restart file
    for (uint i = 0; i < pdb1.atoms.count; ++i){
        /* Find mol i's chain index in restart output files */
        ptrdiff_t pos = find(pdb1Restart.atoms.chainLetter.begin(), 
            pdb1Restart.atoms.chainLetter.end(), pdb1.atoms.chainLetter[i])
            - pdb1Restart.atoms.chainLetter.begin();
        if(pdb1.atoms.x[i] != pdb1Restart.atoms.x[pos])
            std::cout << pdbBase.atoms.x[i] << " " << i << " " << pdb1Restart.atoms.x[pos] << " " << pos <<  std::endl;
        EXPECT_EQ(pdb1.atoms.x[i] == pdb1Restart.atoms.x[pos], true);
    }

    // Checks if the coordinates of the traj match the restart file
    for (uint i = 0; i < pdbN.atoms.count; ++i){
        /* Find mol i's chain index in restart output files */
        ptrdiff_t pos = find(pdbNRestart.atoms.chainLetter.begin(), 
            pdbNRestart.atoms.chainLetter.end(), pdbN.atoms.chainLetter[i])
            - pdbNRestart.atoms.chainLetter.begin();
        if(pdbN.atoms.x[i] != pdbNRestart.atoms.x[pos])
            std::cout << pdbN.atoms.x[i] << " " << i << " " << pdbNRestart.atoms.x[pos] << " " << pos <<  std::endl;
        EXPECT_EQ(pdbN.atoms.x[i] == pdbNRestart.atoms.x[pos], true);
    }

    PDBSetup pdbBase_Base_To_K_1, pdb1_Base_To_K_1;

    // This is needed to get passed Remark 
    // Not sure why.
    frameNum = 1;
    pdbBase_Base_To_K_1.Init(rsBase, pdbnamesBase, frameNum);    
    pdb1_Base_To_K_1.Init(rs1, pdbnames1, frameNum);

    int lastFrame = 11;
    pdbBase_Base_To_K_1.Init(rsBase, pdbnamesBase, lastFrame);    

    // Checks if the last frame the base traj match the first frame of K_1 traj
    for (uint i = 0; i < pdbBase_Base_To_K_1.atoms.count; ++i){
        // Find mol i's chain index in restart output files 
        ptrdiff_t pos1 = find(pdb1_Base_To_K_1.atoms.chainLetter.begin(), 
            pdb1_Base_To_K_1.atoms.chainLetter.end(), pdbBase_Base_To_K_1.atoms.chainLetter[i])
            - pdb1_Base_To_K_1.atoms.chainLetter.begin();
         ptrdiff_t pos2 = find(pdbBase_Base_To_K_1.atoms.chainLetter.begin(), 
            pdbBase_Base_To_K_1.atoms.chainLetter.end(), pdb1_Base_To_K_1.atoms.chainLetter[i])
            - pdbBase_Base_To_K_1.atoms.chainLetter.begin();
        if(pdbBase_Base_To_K_1.atoms.x[i] != pdb1_Base_To_K_1.atoms.x[pos1])
            std::cout << pdbBase_Base_To_K_1.atoms.x[i] << " " << i << " " << pdbBaseRestart.atoms.x[pos1] << " " << pos1 <<  std::endl;
        EXPECT_EQ(pdbBase_Base_To_K_1.atoms.x[i] == pdb1_Base_To_K_1.atoms.x[pos1], true);
        EXPECT_EQ(pdbBase_Base_To_K_1.atoms.y[i] == pdb1_Base_To_K_1.atoms.y[pos1], true);
        EXPECT_EQ(pdbBase_Base_To_K_1.atoms.z[i] == pdb1_Base_To_K_1.atoms.z[pos1], true);

        EXPECT_EQ(pdb1_Base_To_K_1.atoms.x[i] == pdbBase_Base_To_K_1.atoms.x[pos2], true);
        EXPECT_EQ(pdb1_Base_To_K_1.atoms.y[i] == pdbBase_Base_To_K_1.atoms.y[pos2], true);
        EXPECT_EQ(pdb1_Base_To_K_1.atoms.z[i] == pdbBase_Base_To_K_1.atoms.z[pos2], true);

        EXPECT_EQ(pdbBase_Base_To_K_1.atoms.chainLetter[i] == pdb1_Base_To_K_1.atoms.chainLetter[pos1], true);
        EXPECT_EQ(pdbBase_Base_To_K_1.atoms.box[i] == pdb1_Base_To_K_1.atoms.box[pos1], true);
        EXPECT_EQ(pdbBase_Base_To_K_1.atoms.resNames[i] == pdb1_Base_To_K_1.atoms.resNames[pos1], true);
        EXPECT_EQ(pdb1_Base_To_K_1.atoms.beta[i] == pdbBase_Base_To_K_1.atoms.beta[pos2], true);

        EXPECT_EQ(pos1 == pos2, true);
    }

    PDBSetup pdbBase_Wrong_Frame_Base_To_K_1;
    // This is needed to get passed Remark 
    // Not sure why.
    frameNum = 1;
    pdbBase_Wrong_Frame_Base_To_K_1.Init(rsBase, pdbnamesBase, frameNum);  

    int middleFrame = 5;
    pdbBase_Wrong_Frame_Base_To_K_1.Init(rsBase, pdbnamesBase, middleFrame);    

    // Checks if the last frame the base traj match the first frame of K_1 traj
    for (uint i = 0; i < pdbBase_Wrong_Frame_Base_To_K_1.atoms.count; ++i){
        // Find mol i's chain index in restart output files 
        ptrdiff_t pos1 = find(pdb1_Base_To_K_1.atoms.chainLetter.begin(), 
            pdb1_Base_To_K_1.atoms.chainLetter.end(), pdbBase_Wrong_Frame_Base_To_K_1.atoms.chainLetter[i])
            - pdb1_Base_To_K_1.atoms.chainLetter.begin();
         ptrdiff_t pos2 = find(pdbBase_Wrong_Frame_Base_To_K_1.atoms.chainLetter.begin(), 
            pdbBase_Wrong_Frame_Base_To_K_1.atoms.chainLetter.end(), pdb1_Base_To_K_1.atoms.chainLetter[i])
            - pdbBase_Wrong_Frame_Base_To_K_1.atoms.chainLetter.begin();

        EXPECT_EQ(pdbBase_Wrong_Frame_Base_To_K_1.atoms.x[i] == pdb1_Base_To_K_1.atoms.x[pos1], false);
        EXPECT_EQ(pdbBase_Wrong_Frame_Base_To_K_1.atoms.y[i] == pdb1_Base_To_K_1.atoms.y[pos1], false);
        EXPECT_EQ(pdbBase_Wrong_Frame_Base_To_K_1.atoms.z[i] == pdb1_Base_To_K_1.atoms.z[pos1], false);

        EXPECT_EQ(pdb1_Base_To_K_1.atoms.x[i] == pdbBase_Wrong_Frame_Base_To_K_1.atoms.x[pos2], false);
        EXPECT_EQ(pdb1_Base_To_K_1.atoms.y[i] == pdbBase_Wrong_Frame_Base_To_K_1.atoms.y[pos2], false);
        EXPECT_EQ(pdb1_Base_To_K_1.atoms.z[i] == pdbBase_Wrong_Frame_Base_To_K_1.atoms.z[pos2], false);

        EXPECT_EQ(pos1 == pos2, true);
    }

    PDBSetup pdb1_K_1_To_K_N, pdnN_K_1_To_K_N;

    // This is needed to get passed Remark 
    // Not sure why.
    frameNum = 1;
    pdb1_K_1_To_K_N.Init(rs1, pdbnames1, frameNum);    
    pdnN_K_1_To_K_N.Init(rsN, pdbnamesN, frameNum);

    lastFrame = 11;
    pdb1_K_1_To_K_N.Init(rs1, pdbnames1, lastFrame);    

    // Checks if the last frame the base traj match the first frame of K_1 traj
    for (uint i = 0; i < pdb1_K_1_To_K_N.atoms.count; ++i){
        // Find mol i's chain index in restart output files 
        ptrdiff_t pos1 = find(pdnN_K_1_To_K_N.atoms.chainLetter.begin(), 
            pdnN_K_1_To_K_N.atoms.chainLetter.end(), pdb1_K_1_To_K_N.atoms.chainLetter[i])
            - pdnN_K_1_To_K_N.atoms.chainLetter.begin();
         ptrdiff_t pos2 = find(pdb1_K_1_To_K_N.atoms.chainLetter.begin(), 
            pdb1_K_1_To_K_N.atoms.chainLetter.end(), pdnN_K_1_To_K_N.atoms.chainLetter[i])
            - pdb1_K_1_To_K_N.atoms.chainLetter.begin();
        if(pdb1_K_1_To_K_N.atoms.x[i] != pdnN_K_1_To_K_N.atoms.x[pos1])
            std::cout << pdb1_K_1_To_K_N.atoms.x[i] << " " << i << " " << pdbBaseRestart.atoms.x[pos1] << " " << pos1 <<  std::endl;
        EXPECT_EQ(pdb1_K_1_To_K_N.atoms.x[i] == pdnN_K_1_To_K_N.atoms.x[pos1], true);
        EXPECT_EQ(pdb1_K_1_To_K_N.atoms.y[i] == pdnN_K_1_To_K_N.atoms.y[pos1], true);
        EXPECT_EQ(pdb1_K_1_To_K_N.atoms.z[i] == pdnN_K_1_To_K_N.atoms.z[pos1], true);

        EXPECT_EQ(pdnN_K_1_To_K_N.atoms.x[i] == pdb1_K_1_To_K_N.atoms.x[pos2], true);
        EXPECT_EQ(pdnN_K_1_To_K_N.atoms.y[i] == pdb1_K_1_To_K_N.atoms.y[pos2], true);
        EXPECT_EQ(pdnN_K_1_To_K_N.atoms.z[i] == pdb1_K_1_To_K_N.atoms.z[pos2], true);

        EXPECT_EQ(pdb1_K_1_To_K_N.atoms.chainLetter[i] == pdnN_K_1_To_K_N.atoms.chainLetter[pos1], true);
        EXPECT_EQ(pdb1_K_1_To_K_N.atoms.box[i] == pdnN_K_1_To_K_N.atoms.box[pos1], true);
        EXPECT_EQ(pdb1_K_1_To_K_N.atoms.resNames[i] == pdnN_K_1_To_K_N.atoms.resNames[pos1], true);
        EXPECT_EQ(pdnN_K_1_To_K_N.atoms.beta[i] == pdb1_K_1_To_K_N.atoms.beta[pos2], true);

        EXPECT_EQ(pos1 == pos2, true);
    }

    PDBSetup pdb1_Wrong_Frame_K_1_To_K_N;
    // This is needed to get passed Remark 
    // Not sure why.
    frameNum = 1;
    pdb1_Wrong_Frame_K_1_To_K_N.Init(rs1, pdbnames1, frameNum);  

    middleFrame = 5;
    pdb1_Wrong_Frame_K_1_To_K_N.Init(rs1, pdbnames1, middleFrame);    

    // Checks if the last frame the base traj match the first frame of K_1 traj
    for (uint i = 0; i < pdb1_Wrong_Frame_K_1_To_K_N.atoms.count; ++i){
        // Find mol i's chain index in restart output files 
        ptrdiff_t pos1 = find(pdnN_K_1_To_K_N.atoms.chainLetter.begin(), 
            pdnN_K_1_To_K_N.atoms.chainLetter.end(), pdb1_Wrong_Frame_K_1_To_K_N.atoms.chainLetter[i])
            - pdnN_K_1_To_K_N.atoms.chainLetter.begin();
         ptrdiff_t pos2 = find(pdb1_Wrong_Frame_K_1_To_K_N.atoms.chainLetter.begin(), 
            pdb1_Wrong_Frame_K_1_To_K_N.atoms.chainLetter.end(), pdnN_K_1_To_K_N.atoms.chainLetter[i])
            - pdb1_Wrong_Frame_K_1_To_K_N.atoms.chainLetter.begin();

        EXPECT_EQ(pdb1_Wrong_Frame_K_1_To_K_N.atoms.x[i] == pdnN_K_1_To_K_N.atoms.x[pos1], false);
        EXPECT_EQ(pdb1_Wrong_Frame_K_1_To_K_N.atoms.y[i] == pdnN_K_1_To_K_N.atoms.y[pos1], false);
        EXPECT_EQ(pdb1_Wrong_Frame_K_1_To_K_N.atoms.z[i] == pdnN_K_1_To_K_N.atoms.z[pos1], false);

        EXPECT_EQ(pdnN_K_1_To_K_N.atoms.x[i] == pdb1_Wrong_Frame_K_1_To_K_N.atoms.x[pos2], false);
        EXPECT_EQ(pdnN_K_1_To_K_N.atoms.y[i] == pdb1_Wrong_Frame_K_1_To_K_N.atoms.y[pos2], false);
        EXPECT_EQ(pdnN_K_1_To_K_N.atoms.z[i] == pdb1_Wrong_Frame_K_1_To_K_N.atoms.z[pos2], false);

        EXPECT_EQ(pos1 == pos2, true);
    }


}