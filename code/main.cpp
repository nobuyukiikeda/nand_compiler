#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <bitset>
#include <dirent.h>

#include "utils.h"
Utils utils;

#include "Parser.cpp"
#include "CodeWriter.cpp"

void read_file(std::ifstream &ifs, CodeWriter &code_writer)
{
  Parser parser(std::move(ifs));
  while (parser.has_more_command())
  {
    parser.advance();
    // std::cout << parser.command + " " << parser.arg1 << std::endl;

    if (parser.command_type == "C_PUSH")
    {
      code_writer.write_comment(parser.command);
      code_writer.write_push(parser.arg1, parser.arg2);
    }
    else if (parser.command_type == "C_POP")
    {
      code_writer.write_comment(parser.command);
      code_writer.write_pop(parser.arg1, parser.arg2);
    }
    else if (parser.command_type == "C_ARITHMETIC")
    {
      code_writer.write_comment(parser.command);
      code_writer.write_arithmetic(parser.command);
    }
    else if (parser.command_type == "C_LABEL")
    {
      code_writer.write_comment(parser.command);
      code_writer.write_label(parser.command, parser.arg1);
    }
    else if (parser.command_type == "C_GOTO")
    {
      code_writer.write_comment(parser.command);
      code_writer.write_goto(parser.command, parser.arg1);
    }
    else if (parser.command_type == "C_IF")
    {
      code_writer.write_comment(parser.command);
      code_writer.write_if(parser.command, parser.arg1);
    }
    else if (parser.command_type == "C_RETURN")
    {
      code_writer.write_comment(parser.command);
      code_writer.write_return();
    }
    else if (parser.command_type == "C_FUNCTION")
    {
      code_writer.write_comment(parser.command);
      code_writer.write_function(parser.command, parser.arg1);
    }
    else if (parser.command_type == "C_CALL")
    {
      code_writer.write_comment(parser.command);
      code_writer.write_call(parser.command, parser.arg1);
    }
  }
}

int main()
{
  // ファイルの読み込み
  std::cout << "ディレクトリを入力してください。" << std::endl;
  // dirディレクトリ直下に含まれる全ファイルを取得
  std::vector<std::string> paths = {};
  const char *dir = "";
  auto dp = opendir(dir);
  if (dp != NULL)
  {
    struct dirent *dent;
    do
    {
      dent = readdir(dp);
      if (dent)
      {
        auto ex = utils.get_extention(dent->d_name);
        if (ex == "vm")
        {
          paths.push_back(std::string(dir) + std::string(dent->d_name));
        }
      }
    } while (dent != NULL);
  }

  // ファイル書き込みの準備
  std::string output_filename = std::string(dir) + "Main.asm";
  CodeWriter code_writer(output_filename);

  for (size_t i = 0; i < paths.size(); i++)
  {
    auto path = paths[i];
    std::ifstream ifs(path);
    if (ifs.fail())
    {
      std::cerr << "失敗" << std::endl;
      continue;
    }
    read_file(ifs, code_writer);
  }

  code_writer.close();

  return 0;
}