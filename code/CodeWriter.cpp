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
// 16~:      static

class CodeWriter
{
  std::ofstream writing_file;
  std::string file_name;
  int pointer_base = 3;
  int temp_base = 5;
  int lineNum = 1;

public:
  CodeWriter(std::string file_name) : file_name(file_name)
  {
    writing_file.open(file_name, std::ios::out);
  };

  void init()
  {
    write("@256");
    write("D=A");
    write("@SP");
    write("M=D");
    write_call("Sys.init", 0);
  }

  void write_comment(std::string comment)
  {
    writing_file << "// --- " << lineNum << "行目 " << comment << std::endl;
  }

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
    if (segment == "constant")
    {
      write("@" + std::to_string(index));
      write("D=A");
      write("@SP");
      write("A=M");
      write("M=D");
    }
    else if (segment == "pointer")
    {
      write("@" + std::to_string(index));
      write("D=A");
      write("@R3");
      write("A=D+A");
      write("D=M");
      write("@SP");
      write("A=M");
      write("M=D");
    }
    else if (segment == "temp")
    {
      write("@" + std::to_string(index));
      write("D=A");
      write("@R5");
      write("A=D+A");
      write("D=M");
      write("@SP");
      write("A=M");
      write("M=D");
    }
    else if (segment == "static")
    {
      write("@" + std::to_string(index));
      write("D=A");
      write("@16");
      write("A=D+A");
      write("D=M");
      write("@SP");
      write("A=M");
      write("M=D");
    }
    // それ以外の場合
    else
    {
      // 対象のアドレスの値を汎用レジスタに保存
      write("@" + std::to_string(index));
      write("D=A");

      // セグメント固有の行を挿入
      if (segment == "local")
      {
        write("@LCL");
      }
      else if (segment == "argument")
      {
        write("@ARG");
      }
      else if (segment == "this")
      {
        write("@THIS");
      }
      else if (segment == "that")
      {
        write("@THAT");
      }

      write("A=D+M");
      write("D=M");
      write("@SP");
      write("A=M");
      write("M=D");
    }
    add_stack();
  }

  void
  write_pop(std::string segment, int index)
  {
    sub_stack();
    // pointerの場合
    if (segment == "pointer")
    {
      // 対象のアドレスを汎用レジスタに保存
      write("@" + std::to_string(index));
      write("D=A");
      write("@R3");
      write("D=D+A");
      write("@R13");
      write("M=D");
      // stackの値を取得
      write("@SP");
      write("A=M");
      write("D=M");
      // 保存していたアドレスにstackの値を代入
      write("@R13");
      write("A=M");
      write("M=D");
    }
    // tempの場合
    else if (segment == "temp")
    {
      // 対象のアドレスを汎用レジスタに保存
      write("@" + std::to_string(index));
      write("D=A");
      write("@R5");
      write("D=D+A");
      write("@R13");
      write("M=D");
      // stackの値を取得
      write("@SP");
      write("A=M");
      write("D=M");
      // 保存していたアドレスにstackの値を代入
      write("@R13");
      write("A=M");
      write("M=D");
    }
    // staticの場合
    else if (segment == "static")
    {
      // 対象のアドレスを汎用レジスタに保存
      write("@" + std::to_string(index));
      write("D=A");
      write("@16");
      write("D=D+A");
      write("@R13");
      write("M=D");
      // stackの値を取得
      write("@SP");
      write("A=M");
      write("D=M");
      // 保存していたアドレスにstackの値を代入
      write("@R13");
      write("A=M");
      write("M=D");
    }
    // それ以外の場合
    else
    {
      // 対象のアドレスを汎用レジスタに保存
      write("@" + std::to_string(index));
      write("D=A");

      // セグメント固有の行を挿入
      if (segment == "local")
      {
        write("@LCL");
      }
      else if (segment == "argument")
      {
        write("@ARG");
      }
      else if (segment == "this")
      {
        write("@THIS");
      }
      else if (segment == "that")
      {
        write("@THAT");
      }

      write("D=D+M");
      write("@R13");
      write("M=D");
      // stackの値を取得
      write("@SP");
      write("A=M");
      write("D=M");
      // 保存していたアドレスにstackの値を代入
      write("@R13");
      write("A=M");
      write("M=D");
    }
  }

  void write_label(std::string label)
  {
    // LABELは行数に実行時削除されるためカウントしない
    writing_file << "(" + label + ")" << std::endl;
  }

  void write_goto(std::string label)
  {
    write("@" + label);
    write("0;JMP");
  }

  void write_if(std::string label)
  {
    sub_stack();
    write("@SP");
    write("A=M");
    write("D=M");
    write("@" + label);
    write("D;JNE");
  }

  void write_return()
  {
    // リターンアドレスを取得
    write("@LCL");
    write("D=M");
    write("@5");
    write("D=D-A");
    write("@R15");
    write("M=D");
    // 関数の返り値を呼び出し元スタックの上にセット
    write_pop("argument", 0);
    write("@ARG");
    write("D=M");
    // SPの設定
    write("@SP");
    write("M=D+1");
    // THATの設定
    write("@LCL");
    write("A=M-1");
    write("D=M");
    write("@THAT");
    write("M=D");
    // THISの設定
    write("@2");
    write("D=A");
    write("@LCL");
    write("A=M-D");
    write("D=M");
    write("@THIS");
    write("M=D");
    // ARGの設定
    write("@3");
    write("D=A");
    write("@LCL");
    write("A=M-D");
    write("D=M");
    write("@ARG");
    write("M=D");
    // LCLの設定
    write("@4");
    write("D=A");
    write("@LCL");
    write("A=M-D");
    write("D=M");
    write("@LCL");
    write("M=D");
    // return
    write("@R15");
    write("A=M");
    write("0;JMP");
  }

  void write_function(std::string function_name, int num_local)
  {
    // 関数宣言は行数に実行時削除されるためカウントしない
    writing_file << "(" + function_name + ")" << std::endl;
    for (int i = 0; i < num_local; i++)
    {
      write_push("constant", 0);
    }
  }

  void write_call(std::string function_name, int num_args)
  {
    // リターンアドレスをpush
    auto label = std::to_string(lineNum);
    write("@RA" + label);
    write("D=A");
    write("@SP");
    write("A=M");
    write("M=D");
    add_stack();
    // LCLをpush
    write("@LCL");
    write("D=M");
    write("@SP");
    write("A=M");
    write("M=D");
    add_stack();
    // ARGをpush
    write("@ARG");
    write("D=M");
    write("@SP");
    write("A=M");
    write("M=D");
    add_stack();
    // THISをpush
    write("@THIS");
    write("D=M");
    write("@SP");
    write("A=M");
    write("M=D");
    add_stack();
    // THATをpush
    write("@THAT");
    write("D=M");
    write("@SP");
    write("A=M");
    write("M=D");
    add_stack();
    // ARGを呼び出し側に移す
    write("@" + std::to_string(num_args));
    write("D=A");
    write("@SP");
    write("D=M-D");
    write("@5");
    write("D=D-A");
    write("@ARG");
    write("M=D");
    // LCLを呼び出し側に移す
    write("@SP");
    write("D=M");
    write("@LCL");
    write("M=D");
    // 関数へ移動
    write_goto(function_name);
    // リターンアドレスのラベル宣言
    write("(RA" + label + ")");
  }

  void close()
  {
    writing_file.close();
  }

private:
  void add_stack()
  {
    write("@SP");
    write("M=M+1");
  }

  void sub_stack()
  {
    write("@SP");
    write("M=M-1");
  }

  void write(std::string command)
  {
    // std::cout << std::to_string(lineNum) + "行目：" << command << std::endl;
    writing_file << command << std::endl;
    ++lineNum;
  }

  void write_add()
  {
    sub_stack();
    write("@SP");
    write("A=M");
    write("D=M");
    sub_stack();
    write("@SP");
    write("A=M");
    write("M=D+M");
    add_stack();
  }

  void write_sub()
  {
    sub_stack();
    write("@SP");
    write("A=M");
    write("D=M");
    sub_stack();
    write("@SP");
    write("A=M");
    write("M=M-D");
    add_stack();
  }

  void write_eq()
  {
    sub_stack();
    write("@SP");
    write("A=M");
    write("D=M");
    sub_stack();
    write("@SP");
    write("A=M");
    write("D=M-D");
    write("@" + std::to_string(lineNum + 7));
    write("D;JEQ");
    write("@SP");
    write("A=M");
    write("M=0");
    write("@" + std::to_string(lineNum + 5));
    write("0;JMP");
    write("@SP");
    write("A=M");
    write("M=-1");
    add_stack();
  }

  void write_lt()
  {
    sub_stack();
    write("@SP");
    write("A=M");
    write("D=M");
    sub_stack();
    write("@SP");
    write("A=M");
    write("D=M-D");
    write("@" + std::to_string(lineNum + 7));
    write("D;JLT");
    write("@SP");
    write("A=M");
    write("M=0");
    write("@" + std::to_string(lineNum + 5));
    write("0;JMP");
    write("@SP");
    write("A=M");
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
    write("A=M");
    write("D=M-D");
    write("@" + std::to_string(lineNum + 7));
    write("D;JGT");
    write("@SP");
    write("A=M");
    write("M=0");
    write("@" + std::to_string(lineNum + 5));
    write("0;JMP");
    write("@SP");
    write("A=M");
    write("M=-1");
    add_stack();
  }

  void write_neg()
  {
    sub_stack();
    write("@SP");
    write("A=M");
    write("M=-M");
    add_stack();
  }

  void write_and()
  {
    sub_stack();
    write("@SP");
    write("A=M");
    write("D=M");
    sub_stack();
    write("@SP");
    write("A=M");
    write("M=D&M");
    add_stack();
  }

  void write_or()
  {
    sub_stack();
    write("@SP");
    write("A=M");
    write("D=M");
    sub_stack();
    write("@SP");
    write("A=M");
    write("M=D|M");
    add_stack();
  }

  void write_not()
  {
    sub_stack();
    write("@SP");
    write("A=M");
    write("M=!M");
    add_stack();
  }
};
