#include <string>
#include <fstream>
#include <iostream>
#include <map>

#include "utils.h"

const std::string symbols = " (){}[].,;+-*/&<>=|~";
const std::map<std::string, std::string> keyword_dictionary{
    {"class", "CLASS"},
    {"method", "METHOD"},
    {"function", "FUNCTION"},
    {"constructor", "CONSTRUCTOR"},
    {"int", "INT"},
    {"boolean", "BOOLEAN"},
    {"char", "CHAR"},
    {"void", "VOID"},
    {"var", "VAR"},
    {"static", "STATIC"},
    {"field", "FIELD"},
    {"let", "LET"},
    {"do", "DO"},
    {"if", "IF"},
    {"else", "ELSE"},
    {"while", "WHILE"},
    {"return", "RETURN"},
    {"true", "TRUE"},
    {"false", "FALSE"},
    {"null", "NULL"},
    {"this", "THIS"}};

class JackTokenizer
{
  std::string file_name;
  std::string token_type = "";
  std::string token = "";
  bool comment_mode = false;
  std::ifstream ifstream;
  std::ofstream tokens_file;

public:
  JackTokenizer(std::ifstream ifs, std::string file_name) : file_name(file_name), ifstream(std::move(ifs))
  {
    tokens_file.open(file_name, std::ios::out);
  }

  bool has_more_tokens()
  {
    if (ifstream.peek() != EOF)
    {
      return true;
    }
    return false;
  }

  void advance()
  {
    std::string str;
    std::getline(ifstream, str);

    // コメント処理
    remove_comment(str);

    // 記述処理
    while (str.length() > 0)
    {
      Utils::ltrim(str);
      if (str.empty())
      {
        break;
      }

      // 文字列のケース
      if (str.find("\"") == 0)
      {
        auto second_pos = str.find("\"", 1);
        token = str.substr(0, second_pos + 1);
        set_token_type();
        write();
        str = str.substr(second_pos + 1);
        continue;
      }

      size_t symbol_pos = str.find_first_of(symbols);
      if (symbol_pos == 0)
      {
        token = str.substr(0, 1);
        set_token_type();
        write();
        str = str.substr(symbol_pos + 1);
      }
      else
      {
        token = str.substr(0, symbol_pos);
        set_token_type();
        write();
        str = str.substr(symbol_pos);
      }
    }
  }

  void close()
  {
    {
      tokens_file.close();
    }
  }

private:
  void set_token_type()
  {
    try
    {
      std::stoi(token);
      token_type = "integerConstant";
      return;
    }
    catch (...)
    {
      if (token.find("\"") == 0)
      {
        token_type = "stringConstant";
        token = token.substr(1, token.length() - 2);
        return;
      }

      size_t symbol_pos = token.find_first_of(symbols);
      if (symbol_pos != std::string::npos)
      {
        token_type = "symbol";
        if (token == ">")
        {
          token = "&gt;";
        }
        else if (token == "<")
        {
          token = "&lt;";
        }
        else if (token == "&")
        {
          token = "&amp;";
        }
        return;
      }

      try
      {
        keyword_dictionary.at(token);
        token_type = "keyword";
      }
      catch (...)
      {
        token_type = "identifier";
      }
    }
  }

  void remove_comment(std::string &str)
  {
    size_t comment_pos;
    if (comment_mode)
    {
      comment_pos = str.find("*/");
      if (comment_pos != std::string::npos)
      {
        str.erase(0, comment_pos + 2);
        comment_mode = false;
      }
      else
      {
        str.erase(0);
      }
    }
    else
    {
      comment_pos = str.find("/**");
      if (comment_pos != std::string::npos)
      {
        auto comment_pos2 = str.find("*/");
        if (comment_pos2 != std::string::npos)
        {
          str.erase(comment_pos, comment_pos2 + 2);
        }
        else
        {
          str.erase(comment_pos);
          comment_mode = true;
        }
      }
      else
      {
        comment_pos = str.find("/*");
        if (comment_pos != std::string::npos)
        {
          auto comment_pos2 = str.find("*/");
          if (comment_pos2 != std::string::npos)
          {
            str.erase(comment_pos, comment_pos2);
          }
          else
          {
            str.erase(comment_pos);
            comment_mode = true;
          }
        }
      }
    }

    comment_pos = str.find("//");
    if (comment_pos != std::string::npos)
    {
      str.erase(comment_pos);
    }
  }

  void write()
  {
    tokens_file << "<" + token_type + "> ";
    tokens_file << token;
    tokens_file << " </" + token_type + ">" << std::endl;
    ;
  }
};