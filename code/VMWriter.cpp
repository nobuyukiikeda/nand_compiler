#include <string>
#include <fstream>

class VMWriter
{
  std::ofstream output_file;
  std::string file_name;

public:
  VMWriter(std::string file_name) : file_name(file_name)
  {
    output_file.open(file_name, std::ios::out);
  }

  void writer_push(std::string segment, int index)
  {
  }
  void writer_pop(std::string segment, int index)
  {
  }
  void writer_arithmetic(std::string command)
  {
  }
  void writer_label(std::string label)
  {
  }
  void writer_goto(std::string label)
  {
  }
  void write_if(std::string label)
  {
  }
  void writer_call(std::string name, int num_args)
  {
  }
  void writer_function(std::string name, int num_locals)
  {
  }
  void close()
  {
    output_file.close();
  }
};