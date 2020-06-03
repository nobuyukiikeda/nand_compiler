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
  int lcl_base = 0;
  int arg_base = 0;
  int this_base = 0;
  int that_base = 0;
  int pointer_base = 3;
  int temp_base = 5;
  int lineNum = 0;

public:
  CodeWriter(std::string file_name) : file_name(file_name)
  {
    writing_file.open(file_name, std::ios::out);
  };

  void write_arithmetic(std::string command)
  {
    ;
    if (command == "add")
    {
      write_add();
    }
    else if (command == "sub")
    {
      write_sub();
    }
    else if (command == "eq")
    {
      write_eq();
    }
    else if (command == "lt")
    {
      write_lt();
    }
    else if (command == "gt")
    {
      write_gt();
    }
    else if (command == "neg")
    {
      write_neg();
    }
    else if (command == "and")
    {
      write_and();
    }
    else if (command == "or")
    {
      write_or();
    }
    else if (command == "not")
    {
      write_not();
    }
  }

  void write_push(std::string segment, int index)
  {
    int value = get_index_value(segment, index);
    if (segment == "constant")
    {
      write("@" + std::to_string(value));
      write("D=A");
    }
    else
    {
      write("D=M");
    }
    write("@SP");
    write("M=D");
    add_stack();
  }

  void write_pop(std::string segment, int index)
  {
    sub_stack();
    int value = get_index_value(segment, index);
    write("@SP");
    write("D=M");
    write("@" + std::to_string(value));
    write("M=D");
  }

  void close()
  {
    write("@" + std::to_string(lineNum + 1));
    write("0;JMP");
    writing_file.close();
  }

private:
  void add_stack()
  {
    ++sp;
    write("@0");
    write("M=M+1");
  }

  void sub_stack()
  {
    --sp;
    write("@0");
    write("M=M-1");
  }

  int get_index_value(std::string segment, int index)
  {
    if (segment == "constant")
    {
      return index;
    }
    else if (segment == "local")
    {
      return lcl_base + index;
    }
    else if (segment == "argument")
    {
      return arg_base + index;
    }
    else if (segment == "this")
    {
      write("@" + std::to_string(index));
      write("D=A");
      write("@THIS");
      write("D=D+A");
      // return this_base + index;
    }
    else if (segment == "that")
    {
      write("@" + std::to_string(index));
      write("D=A");
      write("@THAT");
      write("D=D+A");
      // return that_base + index;
    }
    else if (segment == "pointer")
    {
      return pointer_base + index;
    }
    else if (segment == "temp")
    {
      return temp_base + index;
    }
    return 0;
  }

  void write(std::string command)
  {
    // std::cout << std::to_string(lineNum) + "行目：" << command << std::endl;
    ++lineNum;
    writing_file << command << std::endl;
  }

  void write_add()
  {
    sub_stack();
    write("@SP");
    write("D=M");
    sub_stack();
    write("@SP");
    write("M=D+M");
    add_stack();
  }

  void write_sub()
  {
    sub_stack();
    write("@SP");
    write("D=M");
    sub_stack();
    write("@SP");
    write("M=M-D");
    add_stack();
  }

  void write_eq()
  {
    sub_stack();
    write("@SP");
    write("D=M");
    sub_stack();
    write("@SP");
    write("D=M-D");
    write("@" + std::to_string(lineNum + 6));
    write("D;JEQ");
    write("@SP");
    write("M=0");
    write("@" + std::to_string(lineNum + 4));
    write("0;JMP");
    write("@SP");
    write("M=-1");
    add_stack();
  }

  void write_lt()
  {
    sub_stack();
    write("@SP");
    write("D=M");
    sub_stack();
    write("@SP");
    write("D=M-D");
    write("@" + std::to_string(lineNum + 6));
    write("D;JLT");
    write("@SP");
    write("M=0");
    write("@" + std::to_string(lineNum + 4));
    write("0;JMP");
    write("@SP");
    write("M=-1");
    add_stack();
  }

  void write_gt()
  {
    sub_stack();
    write("@SP");
    write("D=M");
    sub_stack();
    write("@SP");
    write("D=M-D");
    write("@" + std::to_string(lineNum + 6));
    write("D;JGT");
    write("@SP");
    write("M=0");
    write("@" + std::to_string(lineNum + 4));
    write("0;JMP");
    write("@SP");
    write("M=-1");
    add_stack();
  }

  void write_neg()
  {
    sub_stack();
    write("@SP");
    write("M=-M");
    add_stack();
  }

  void write_and()
  {
    sub_stack();
    write("@SP");
    write("D=M");
    sub_stack();
    write("@SP");
    write("M=D&M");
    add_stack();
  }

  void write_or()
  {
    sub_stack();
    write("@SP");
    write("D=M");
    sub_stack();
    write("@SP");
    write("M=D|M");
    add_stack();
  }

  void write_not()
  {
    sub_stack();
    write("@SP");
    write("M=!M");
    add_stack();
  }
};
