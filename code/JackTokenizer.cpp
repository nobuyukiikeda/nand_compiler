#include <string>
#include <fstream>
#include <regex>
#include <iostream>

#include "utils.h"

const std::regex re(" +|\\(|\\)|\\{|\\}|\\.|\\,|\\;|\\+|\\-|\\*|\\/|\\&|\\||\\<|\\>|\\=|\\~");
// const std::regex re("//");

class JackTokenizer
{
  std::ifstream ifstream;
  std::ofstream tokens_file;
  std::string file_name;
  std::string token_type = "";
  std::string token = "";
  bool comment_mode = false;

public:
  JackTokenizer(std::ifstream ifs, std::string file_name) : file_name(file_name), ifstream(std::move(ifs))
  {
    tokens_file.open(file_name, std::ios::out);
    tokens_file << "<tokens>" << std::endl;
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

    std::smatch m;

    // 記述処理
    while (str.length() > 0)
    {
      Utils::ltrim(str);
      if (str.empty())
      {
        break;
      }

      std::regex_search(str, m, re);
      if (!m.ready() || m.empty())
      {
        break;
      }

      if (m.position(0) == 0)
      {
        token = str.substr(0, 1);
        set_token_type();
        write();
        str = str.substr(m.position(0) + 1);
      }
      else
      {
        token = str.substr(0, m.position(0));
        set_token_type();
        write();
        str = str.substr(m.position(0));
      }
    }
  }

  std::string keyword()
  {
    return "CLASS";
    return "METHOD";
    return "FUNCTION";
    return "CONSTRUCTOR";
    return "INT";
    return "BOOLEAN";
    return "CHAR";
    return "VOID";
    return "VAR";
    return "STATIC";
    return "FIELD";
    return "LET";
    return "DO";
    return "IF";
    return "ELSE";
    return "WHILE";
    return "RETURN";
    return "TRUE";
    return "FALSE";
    return "NULL";
    return "THIS";
  }

  std::string symbol()
  {
    return "";
  }

  std::string identifier()
  {
    return "";
  }

  int intVal()
  {
    return 0;
  }

  std::string stringVal()
  {
    return "";
  }

  void close()
  {
    {
      tokens_file << "</tokens>" << std::endl;
      tokens_file.close();
    }
  }

private:
  void set_token_type()
  {
    int int_token = std::stoi(token);
    if (int_token >= 0)
    {
      token_type = "INT_CONST";
    }
    else
    {

      token_type = "KEYWORD";
      token_type = "SYMBOL";
      token_type = "IDENTIFIER";
      token_type = "STRING_CONST";
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