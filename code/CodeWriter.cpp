#include <stdio.h>
#include <fstream>
#include <map>
#include <string>

// R0: SP:   stackの最上位
// R1: LCL:  localのベース
// R2: ARG:  argumentのベース
// R3: THIS: thisのベース
// R4: THAT: thatのベース
// R5-12:    tempの値
// R13-15:   汎用レジスタ

class CodeWriter
{
  std::ofstream writing_file;
  std::string file_name;
  int sp = 256;

public:
  CodeWriter(std::string file_name) : file_name(file_name)
  {
    writing_file.open(file_name, std::ios::out);
  };

  void write_arithmetic(std::string command)
  {
    if (command == "add")
    {
      write_add();
    }
  }

  void write_push(std::string segment, int index)
  {
    int value = get_index_value(segment, index);
    writing_file << "@" + std::to_string(value) << std::endl;
    writing_file << "D=A" << std::endl;
    writing_file << "@" + std::to_string(sp) << std::endl;
    writing_file << "M=D" << std::endl;
    sp++;
  }

  void write_pop(std::string segment, int index)
  {
  }

  void close()
  {
    writing_file.close();
  }

private:
  int get_index_value(std::string segment, int index)
  {
    if (segment == "constant")
    {
      return index;
    }
    return 0;
  }

  void write_add()
  {
    sp--;
    writing_file << "@" + std::to_string(sp) << std::endl;
    writing_file << "D=M" << std::endl;
    sp--;
    writing_file << "D=D+M" << std::endl;
  }
};
