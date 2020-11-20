#include <string>
#include <map>

class SymbolMap
{
public:
  std::string type;
  std::string kind;
  int index;
  SymbolMap(std::string type, std::string kind, int index) : type(type), kind(kind), index(index)
  {
  }
};

class SymbolTable
{
  std::map<std::string, SymbolMap> static_table = {};
  int static_index = 0;
  std::map<std::string, SymbolMap> subroutine_table = {};
  int subroutine_index = 0;

public:
  SymbolTable() {}

  void start_subroutine()
  {
    subroutine_index = 0;
    subroutine_table = {};
  }

  void define(std::string name, std::string type, std::string kind)
  {
    if (kind == "STATIC" || kind == "FIELD")
    {
      SymbolMap symbol(type, kind, static_index);
      static_table.insert(std::make_pair(name, symbol));
      static_index++;
    }
    else
    {
      SymbolMap symbol(type, kind, subroutine_index);
      subroutine_table.insert(std::make_pair(name, symbol));
      subroutine_index++;
    }
  }

  int var_count(std::string kind)
  {
    if (kind == "STATIC" || kind == "FIELD")
    {
      return static_table.size();
    }
    else
    {
      return subroutine_table.size();
    }
  }

  std::string kind_of(std::string name)
  {
    auto kind = subroutine_table.find(name);
    if (kind == subroutine_table.end())
    {
      kind = static_table.find(name);
      if (kind == static_table.end())
      {
        return "NONE";
      }
    }
    return kind->second.kind;
  }

  std::string type_of(std::string name)
  {
    auto kind = subroutine_table.find(name);
    if (kind == subroutine_table.end())
    {
      kind = static_table.find(name);
      if (kind == static_table.end())
      {
        return "NONE";
      }
    }
    return kind->second.type;
  }

  int index_of(std::string name)
  {
    auto kind = subroutine_table.find(name);
    if (kind == subroutine_table.end())
    {
      kind = static_table.find(name);
    }
    return kind->second.index;
  }
};