#ifndef CBMSTSWTQABASEVIEW_H
#define CBMSTSWTQABASEVIEW_H

#include <WContainerWidget>
#include <string>
#include <map>

using namespace Wt;
using namespace std;

namespace Views {

  typedef enum { sig_browser = 0,  sig_tabdown = 1 } signal_t;

  
  class CbmStsWtQaSigStr : public Wt::WObject
  {
  public:
    Wt::Signal<string>& get_str() {return sigStr;} 
    void emit_str(std::string& str) { sigStr.emit(str); }
    
  protected:  
    Wt::Signal<string> sigStr;
    
  };
  
  
  class  CbmStsWtQaBaseView : public WContainerWidget
  {
    typedef  std::map<signal_t , CbmStsWtQaSigStr* > SigMap;
    
  public:
    CbmStsWtQaBaseView(WContainerWidget *parent = 0);
    virtual ~CbmStsWtQaBaseView();

    virtual CbmStsWtQaSigStr* getSignal(signal_t s_type) {return fSignal[s_type];}
    virtual void setSignal(CbmStsWtQaSigStr* sig, signal_t s_type)
    {
      //auto val = std::shared_ptr<CbmStsWtQaSigStr> (sig);
      fSignal.insert(std::make_pair(s_type, sig));
    }
    
  protected:
    SigMap fSignal;
  };
  
}

#endif //CBMSTSWTQABASEVIEW_H
