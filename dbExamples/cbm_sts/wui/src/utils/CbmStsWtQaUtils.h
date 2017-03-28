#ifndef CBMSTSWTQAUTILS_H
#define CBMSTSWTQAUTILS_H

#include <string>
#include <vector>
#include <list>
#include <sstream>

#include <boost/locale.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>


// <DB> Use unary operator+ for the conversion from enum classes to their underlying type:
#define UNSIGNED_ENUM_CLASS(name, ...) enum class name : unsigned { __VA_ARGS__ }; \
  inline constexpr unsigned operator+ (name const val) { return static_cast<unsigned>(val); }

namespace Utils{

  std::vector<std::string> splitString(std::string string, std::string separators);
  std::string stringToUTF8(const std::string& str);
 
  constexpr unsigned int str2int(const char* str, int h = 0)
  {
    return !str[h] ? 5381 : (str2int(str, h+1)*33) ^ str[h];
  }

  template<typename T>
    std::vector<T> parse_value(const std::string& s, const char* sep="_") {
    std::vector<std::string> parts;
    boost::split(parts, s, boost::is_any_of(sep));
    std::vector<T> res;
    std::transform(parts.begin(), parts.end(), std::back_inserter(res),
                   [](const std::string& as) { return boost::lexical_cast<T>(as); });
    return res;
  }  

  
}
 
#endif
