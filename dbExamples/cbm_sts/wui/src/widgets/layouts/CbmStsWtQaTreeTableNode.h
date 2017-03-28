#ifndef CBMSTSWTQATREETABLENODE_H
#define CBMSTSWTQATREETABLENODE_H

#include <map> 
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>


#include <WContainerWidget>
#include <WTable>
#include <WTableView>
#include <WTreeTableNode>
#include <WString>
#include <WIconPair>
#include <WStringUtil>
#include <WText>
#include <WPushButton>

// <DB> define the number of vendors 


using namespace Wt;

namespace Layout
{
  class CbmStsWtQaTreeTableNode : public Wt::WTreeTableNode 
  {
  public:
    CbmStsWtQaTreeTableNode(WString& pName, bool isDir);
    virtual ~CbmStsWtQaTreeTableNode();

    void createTree();
    
  protected:
    static WIconPair *createIcon(bool isDir);
    
  };
}
#endif // CBMSTSWTQATREETABLENODE_H
