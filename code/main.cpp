#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <bitset>

#include "utils.h"
Utils utils;

#include "Parser.cpp"
#include "CodeWriter.cpp"

int main()
{
  // ファイルの読み込み
  std::cout << "ファイルパスを入力してください。" << std::endl;
  std::string path;
  std::cin >> path;
  path = "/mnt/c/Users/nobu/htdocs/nand2tetris/projects/07/StackArithmetic/SimpleAdd/SimpleAdd.vm";
  // path = "/mnt/c/Users/nobu/htdocs/nand2tetris/projects/07/StackArithmetic/StackTest/StackTest.vm";
  // path = "/mnt/c/Users/nobu/htdocs/nand2tetris/projects/07/MemoryAccess/BasicTest/BasicTest.vm";
  std::ifstream ifs(path);
  if (ifs.fail())
  {
    std::cerr << "失敗" << std::endl;
  }
  Parser parser(std::move(ifs));

  // ファイル書き込みの準備
  std::string filename = path;
  std::size_t extention_pos = filename.find_last_of(".");
  std::string output_filename = filename.erase(extention_pos) + ".asm";
  CodeWriter code_writer(output_filename);

  while (parser.has_more_command())
  {
    parser.advance();

    if (parser.command_type == "C_PUSH")
    {
      code_writer.write_push(parser.arg1, parser.arg2);
    }
    else if (parser.command_type == "C_POP")
    {
      code_writer.write_pop(parser.arg1, parser.arg2);
    }
    else if (parser.command_type == "C_ARITHMETIC")
    {
      code_writer.write_arithmetic(parser.command);
    }
  }

  code_writer.close();

  return 0;
}
