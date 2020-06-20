#include <stdio.h>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "utils.h"

std::map<std::string, std::string> command_type_map = {
    {"add", "C_ARITHMETIC"},
    {"sub", "C_ARITHMETIC"},
    {"neg", "C_ARITHMETIC"},
    {"eq", "C_ARITHMETIC"},
    {"gt", "C_ARITHMETIC"},
    {"lt", "C_ARITHMETIC"},
    {"and", "C_ARITHMETIC"},
    {"or", "C_ARITHMETIC"},
    {"not", "C_ARITHMETIC"},
    {"push", "C_PUSH"},
    {"pop", "C_POP"},
    {"label", "C_LABEL"},
    {"goto", "C_GOTO"},
    {"if-goto", "C_IF"},
    {"return", "C_RETURN"},
    {"function", "C_FUNCTION"},
    {"call", "C_CALL"},
};

class Parser
{
  std::ifstream ifstream;

public:
  std::string command;
  std::string command_type;
  std::string arg1;
  int arg2;

  Parser(std::ifstream ifs) : ifstream(std::move(ifs)) {}

  bool has_more_command()
  {
    if (ifstream.peek() != EOF)
    {
      return true;
    }
    return false;
  }

  void advance()
  {
    // 行を読み取ってcommandにセット
    std::getline(ifstream, command);

    // コメント、前後空白を削除
    std::size_t comment_pos = command.find("//");
    if (comment_pos != std::string::npos)
    {
      command = command.substr(0, comment_pos);
    }
    Utils::trim(command);

    // パースして、コマンド、引数をセット
    parse();
  }

  void reset()
  {
    ifstream.seekg(0);
    command = "";
    command_type = "";
  }

private:
  void parse()
  {
    if (command.length() == 0)
    {
      command_type = "";
      return;
    }

    // コマンドを各要素に分解
    auto command_list = utils.split(command, " ");
    auto c = command_list.begin();
    while (c != command_list.end())
    {
      // std::cout << *c << std::endl;
      ++c;
    }

    // コマンドタイプをセット
    command_type = get_command_type(command_list[0]);

    // 引数をセット
    set_args(command_list);
  }

  std::string get_command_type(std::string command_string)
  {
    if (command_type_map[command_string] != "")
    {
      return command_type_map[command_string];
    }
    else
    {
      return "";
    }
  }

  void set_args(std::vector<std::string> command_list)
  {
    // C_ARITHMETICの場合はコマンド自体をarg1にセット
    if (command_type == "C_ARITHMETIC")
    {
      arg1 = command_list[0];
      arg2 = 0;
    }
    else if (command_type == "C_RETURN")
    {
      // C_RETURNは引数なし
      arg1 = "";
      arg2 = 0;
    }
    else
    {
      // C_ARITHMETIC以外の場合
      arg1 = command_list[1];
      // 下記の場合はarg2をセット
      if (command_type == "C_PUSH" || command_type == "C_POP" || command_type == "C_FUNCTION" || command_type == "C_CALL")
      {
        arg2 = stoi(command_list[2]);
      }
      else
      {
        arg2 = 0;
      }
    }
  }
};
