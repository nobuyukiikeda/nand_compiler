#include "JackTokenizer.cpp"
#include "CompilationEngine.cpp"

int main()
{
  std::cout << "ファイル名を入力してください";
  std::string path;
  std::cin >> path;
  std::ifstream ifs(path);

  JackTokenizer tokenizer(std::move(ifs), "MainT.xml");
  while (tokenizer.has_more_tokens())
  {
    tokenizer.advance();
  }
  tokenizer.close();

  std::cout << "tokenizer end" << std::endl;

  std::ifstream ifs2("MainT.xml");
  CompilationEngine compiler(std::move(ifs2), "Main.xml");
  compiler.close();

  return 0;
}