/*******************************************************************************
GPU OPTIMIZED MONTE CARLO (GOMC) 2.75
Copyright (C) 2022 GOMC Group
A copy of the MIT License can be found in License.txt
along with this program, also can be found at <https://opensource.org/licenses/MIT>.
********************************************************************************/
#pragma once
#include <iostream>
#include <fstream>
#include <vector>

class InputFileReader
{
private:
  std::fstream fs;
  std::vector<std::string> & split(const std::string &s, char delim, std::vector<std::string> &elems);

public:
  bool readNextLine(std::vector<std::string> & str);
  void Open(std::string fileName);
  InputFileReader(std::string fileName);
  InputFileReader(void);
  ~InputFileReader();
};
