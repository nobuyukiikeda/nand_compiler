#include <string>
#include <fstream>
#include <iostream>

const std::string op = "+-*/&|=";

class CompilationEngine
{
  std::string str;
  size_t indent_size = 0;
  std::string file_name;
  std::ifstream ifstream;
  std::ofstream output_file;

public:
  CompilationEngine(std::ifstream ifs, std::string file_name) : file_name(file_name), ifstream(std::move(ifs))
  {
    output_file.open(file_name, std::ios::out);
    compileClass();
  }

  void compileClass()
  {
    std::cout << "compileClass start" << std::endl;

    write("<class>");
    indent_size++;
    std::getline(ifstream, str);
    if (str.find("class") == std::string::npos)
    {
      std::cout << "compileClass Error" << std::endl;
    }
    write(str);

    std::getline(ifstream, str);
    if (str.find("identifier") == std::string::npos)
    {
      std::cout << "compileClass Error" << std::endl;
    }
    write(str);

    std::getline(ifstream, str);
    if (str.find("{") == std::string::npos)
    {
      std::cout << "compileClass Error" << std::endl;
    }
    write(str);

    std::getline(ifstream, str);
    while (str.find("static") != std::string::npos || str.find("field") != std::string::npos)
    {
      compileClassVarDec();
      std::getline(ifstream, str);
    }

    while (str.find("constructor") != std::string::npos || str.find("function") != std::string::npos || str.find("method") != std::string::npos)
    {
      compileSubroutine();
      std::getline(ifstream, str);
    }

    if (str.find("}") == std::string::npos)
    {
      std::cout << "compileClass Error" << std::endl;
    }
    write(str);

    indent_size--;
    write("</class>");
    std::cout << "compileClass end" << std::endl;
  }

  void close()
  {
    output_file.close();
  }

private:
  void write(std::string word)
  {
    for (size_t i = 0; i < indent_size; i++)
    {
      output_file << "  ";
    }
    output_file << word << std::endl;
    if (indent_size > 100)
    {
      throw "Endless Loop Error";
      output_file.close();
    }
  }

  void compileClassVarDec()
  {
    std::cout << "compileClassVarDec start" << std::endl;

    write("<classVarDec>");
    indent_size++;

    while (str.find(";") == std::string::npos)
    {
      write(str);
      std::getline(ifstream, str);
    }
    write(str);

    indent_size--;
    write("</classVarDec>");

    std::cout << "compileClassVarDec end" << std::endl;
  }

  void compileSubroutine()
  {
    std::cout << "compileSubroutine start" << std::endl;

    write("<subroutineDec>");
    indent_size++;

    write(str);
    std::getline(ifstream, str);
    write(str);
    std::getline(ifstream, str);
    write(str);
    std::getline(ifstream, str);
    if (str.find("(") == std::string::npos)
    {
      std::cout << "compileSubroutineDec Error" << std::endl;
    }
    write(str);

    compileParameterList();

    if (str.find(")") == std::string::npos)
    {
      std::cout << "compileSubroutineDec Error" << std::endl;
    }
    write(str);

    compileSubroutineBody();

    indent_size--;
    write("</subroutineDec>");

    std::cout << "compileSubroutine start" << std::endl;
  }

  // 1つ先読みして返す
  void compileParameterList()
  {
    write("<parameterList>");
    indent_size++;

    std::getline(ifstream, str);
    while (str.find(")") == std::string::npos)
    {
      write(str);
      std::getline(ifstream, str);
    }

    indent_size--;
    write("</parameterList>");
  }

  void compileSubroutineBody()
  {
    write("<subroutineBody>");
    indent_size++;

    std::getline(ifstream, str);
    if (str.find("{") == std::string::npos)
    {
      std::cout << "compileSubroutineBody Error" << std::endl;
    }
    write(str);

    std::getline(ifstream, str);
    while (str.find("var") != std::string::npos)
    {
      compileVarDec();
      std::getline(ifstream, str);
    }

    compileStatements();

    if (str.find("}") == std::string::npos)
    {
      std::cout << "compileSubroutineBody Error" << std::endl;
    }
    write(str);

    indent_size--;
    write("</subroutineBody>");
  }

  // 1つ先読みされた状態で呼ばれる
  void compileVarDec()
  {
    write("<varDec>");
    while (str.find(";") == std::string::npos)
    {
      write(str);
      std::getline(ifstream, str);
    }
    write(str);
    write("</varDec>");
  }

  // 先読みされた状態で呼ばれる
  // 1つ先読みして返す
  void compileStatements()
  {
    write("<statements>");
    indent_size++;

    while (str.find("}") == std::string::npos)
    {
      if (str.find("let") != std::string::npos)
      {
        compileLet();
      }
      else if (str.find("if") != std::string::npos)
      {
        compileIf();
      }
      else if (str.find("while") != std::string::npos)
      {
        compileWhile();
      }
      else if (str.find("do") != std::string::npos)
      {
        compileDo();
      }
      else if (str.find("return") != std::string::npos)
      {
        compileReturn();
      }
    }

    indent_size--;
    write("</statements>");
  }

  // 1つ先読みして返す
  void compileLet()
  {
    write("<letStatement>");
    indent_size++;

    write(str);
    std::getline(ifstream, str);
    write(str);
    std::getline(ifstream, str);
    if (str.find("[") != std::string::npos)
    {
      write(str);
      std::getline(ifstream, str);
      compileExpression();
      write(str);
      std::getline(ifstream, str);
    }
    write(str);
    std::getline(ifstream, str);
    compileExpression();
    write(str);
    std::getline(ifstream, str);

    indent_size--;
    write("</letStatement>");
  }

  // 1つ先読みして返す
  void compileIf()
  {
    write("<ifStatement>");
    indent_size++;
    write(str); // if
    std::getline(ifstream, str);
    write(str); // (
    std::getline(ifstream, str);
    compileExpression();
    write(str); // )
    std::getline(ifstream, str);
    write(str); // {
    std::getline(ifstream, str);
    compileStatements();
    write(str); // }
    std::getline(ifstream, str);
    if (str.find("else") != std::string::npos)
    {
      write(str); // else
      std::getline(ifstream, str);
      write(str); // {
      std::getline(ifstream, str);
      compileStatements();
      write(str); // }
      std::getline(ifstream, str);
    }

    indent_size--;
    write("</ifStatement>");
  }

  // 1つ先読みして返す
  void compileWhile()
  {
    write("<whileStatement>");
    indent_size++;
    write(str);
    std::getline(ifstream, str);
    write(str);
    std::getline(ifstream, str);
    compileExpression();
    write(str);
    std::getline(ifstream, str);
    write(str);
    std::getline(ifstream, str);
    compileStatements();
    write(str);
    std::getline(ifstream, str);
    indent_size--;
    write("</whileStatement>");
  }

  // 1つ先読みして返す
  void compileDo()
  {
    write("<doStatement>");
    indent_size++;
    write(str); // do
    std::getline(ifstream, str);
    write(str); // subroutineName
    std::getline(ifstream, str);
    compileSubroutineCall();
    write(str); // ;
    std::getline(ifstream, str);
    indent_size--;
    write("</doStatement>");
  }

  // 1つ先読みして返す
  void compileReturn()
  {
    write("<returnStatement>");
    indent_size++;
    write(str);
    std::getline(ifstream, str);
    if (str.find(";") == std::string::npos)
    {
      compileExpression();
    }
    write(str);
    std::getline(ifstream, str);
    indent_size--;
    write("</returnStatement>");
  }

  // 1つ先読みされた状態で呼ばれる
  // 1つ先読みして返す
  void compileExpression()
  {
    write("<expression>");
    indent_size++;
    compileTerm();
    while (
        str.find_first_of(op) - 1 != str.find_last_of("<") &&
        str.find_first_of(op) != std::string::npos)
    {
      write(str);
      std::getline(ifstream, str);
      compileTerm();
    }
    indent_size--;
    write("</expression>");
  }

  // 1つ先読みされた状態で呼ばれる
  // 1つ先読みして返す
  void compileTerm()
  {
    std::cout << "term start:" << str << std::endl;
    write("<term>");
    indent_size++;
    if (str.find("integerConstant") == 1 ||
        str.find("stringConstant") == 1)
    {
      write(str);
      std::getline(ifstream, str);
    }
    else if (str.find_first_of("symbol") == 1 && str.find("-~") != std::string::npos)
    {
      write(str);
      std::getline(ifstream, str);
      compileTerm();
    }
    else if (str.find("(") != std::string::npos)
    {
      write(str); // (
      std::getline(ifstream, str);
      compileExpression();
      write(str); // )
      std::getline(ifstream, str);
    }
    else
    {
      write(str);
      std::getline(ifstream, str);
      if (str.find_first_of("(.") != std::string::npos)
      {
        compileSubroutineCall();
      }
      else if (str.find("[") != std::string::npos)
      {
        write(str);
        std::getline(ifstream, str);
        compileExpression();
        write(str);
        std::getline(ifstream, str);
      }
    }
    indent_size--;
    write("</term>");
    std::cout << "term end:" << str << std::endl;
  }

  // 1つ先読みされた状態で呼ばれる
  // 1つ先読みして返す
  void compileExpressionList()
  {
    write("<expressionList>");
    indent_size++;
    if (str.find(")") == std::string::npos)
    {
      compileExpression();
    }
    while (str.find(")") == std::string::npos)
    {
      write(str);
      std::getline(ifstream, str);
      compileExpression();
    }
    indent_size--;
    write("</expressionList>");
  }

  // subroutineNameは記述された状態で呼ばれる
  // 1つ先読みして返す
  void compileSubroutineCall()
  {
    if (str.find(".") != std::string::npos)
    {
      write(str); // .
      std::getline(ifstream, str);
      write(str); // functionName
      std::getline(ifstream, str);
    }
    write(str); // (
    std::getline(ifstream, str);
    compileExpressionList();
    write(str);
    std::getline(ifstream, str);
  }
};
