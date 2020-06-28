#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <cstring>
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
      code_writer.write_label(parser.arg1);
    }
    else if (parser.command_type == "C_GOTO")
    {
      code_writer.write_comment(parser.command);
      code_writer.write_goto(parser.arg1);
    }
    else if (parser.command_type == "C_IF")
    {
      code_writer.write_comment(parser.command);
      code_writer.write_if(parser.arg1);
    }
    else if (parser.command_type == "C_RETURN")
    {
      code_writer.write_comment(parser.command);
      code_writer.write_return();
    }
    else if (parser.command_type == "C_FUNCTION")
    {
      code_writer.write_comment(parser.command);
      code_writer.write_function(parser.arg1, parser.arg2);
    }
    else if (parser.command_type == "C_CALL")
    {
      code_writer.write_comment(parser.command);
      code_writer.write_call(parser.arg1, parser.arg2);
    }
  }
}

int main()
{
  // 入力ファイルのあるディレクトリ, 出力ファイル名を入力
  std::cout << "ディレクトリを入力してください。" << std::endl;
  std::string dir_name;
  std::cin >> dir_name;
  std::cout << "出力するファイル名を入力してください。" << std::endl;
  std::string file_name;
  std::cin >> file_name;

  // dirディレクトリ直下に含まれる全ファイルを取得
  std::vector<std::string> paths = {};
  // ディレクトリ名をchar型に変換
  const char *dir = dir_name.c_str();
  std::cout << *dir << std::endl;
  auto dp = opendir(dir);
  if (dp != NULL)
  {
    struct dirent *dent;
    do
    {
      dent = readdir(dp);
      if (dent)
      {
        auto extention = utils.get_extention(dent->d_name);
        if (extention == "vm")
        {
          // Sys.vmは先頭に入れる
          if (std::string(dent->d_name) == "Sys.vm")
          {
            paths.insert(paths.begin(), std::string(dir) + std::string(dent->d_name));
          }
          else
          {
            paths.push_back(std::string(dir) + std::string(dent->d_name));
          }
        }
      }
    } while (dent != NULL);
  }

  // ファイル書き込みの準備
  std::string output_filename = std::string(dir) + file_name + ".asm";
  CodeWriter code_writer(output_filename);

  // 起動時のコード
  code_writer.init();

  for (size_t i = 0; i < paths.size(); i++)
  {
    auto path = paths[i];
    std::ifstream ifs(path);
    if (ifs.fail())
    {
      std::cerr << "失敗" << std::endl;
      continue;
    }
    code_writer.set_local_static_base();
    read_file(ifs, code_writer);
  }

  code_writer.close();

  return 0;
}