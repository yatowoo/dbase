#include "CbmStsWtQaUtils.h"

namespace Utils{

  std::vector<std::string> splitString(std::string string, std::string separators)
  {
    std::vector<std::string> res;
    boost::algorithm::split(res, string, boost::is_any_of(separators), boost::token_compress_on);
    return res;
  }
  
  std::string stringToUTF8(const std::string& str)
  {
    return boost::locale::conv::to_utf<char>(str, "UTF-8");
  }

  
}
