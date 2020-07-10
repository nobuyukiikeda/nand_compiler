#pragma once
#ifndef utils_h
#define utils_h

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

class Utils
{
public:
  static void ltrim(std::string &s)
  {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) { return !std::isspace(ch); }));
  }

  static void rtrim(std::string &s)
  {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) { return !std::isspace(ch); }).base(), s.end());
  }

  static void trim(std::string &s)
  {
    ltrim(s);
    rtrim(s);
  }

  static std::vector<std::string> split(std::string &string, std::string separator)
  {
    auto list = std::vector<std::string>();
    if (separator.length() == 0)
    {
      list.push_back(string);
    }
    else
    {
      auto offset = std::string::size_type(0);
      while (1)
      {
        auto pos = string.find(separator, offset);
        if (pos == std::string::npos)
        {
          list.push_back(string.substr(offset));
          break;
        }
        list.push_back(string.substr(offset, pos - offset));
        offset = pos + separator.length();
      }
    }
    return list;
  }

  static std::string get_extention(const char *str)
  {
    auto st = std::string(str);
    auto list = split(st, ".");
    auto v = list.end();
    v--;
    return *v;
  }
};

#endif /* utils_h */
