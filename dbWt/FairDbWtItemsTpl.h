#ifndef FAIRDBWTITEMSTPL_H
#define FAIRDBWTITEMSTPL_H


#include <WTemplate>

using namespace Wt;

class FairDbWtItemsTpl : public WTemplate
{
public:
  FairDbWtItemsTpl(const char *trKey);

  virtual void resolveString(const std::string& varName,
			     const std::vector<Wt::WString>& args,
			     std::ostream& result);

private:
  std::string docUrl(const std::string& className);
  std::string getString(const std::string& varName);
  
  static std::string escape(const std::string& name);
};

#endif // FAIRDBWTITEMSTPL_H
