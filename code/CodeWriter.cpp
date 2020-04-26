#include <stdio.h>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

class CodeWriter
{
  std::ofstream writing_file;

  CodeWriter(std::string file_name)
  {
    writing_file.open(file_name, std::ios::out);
  };

  void close()
  {
    writing_file.close();
  }
};
