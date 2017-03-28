#ifndef CBMSTSWTQATREETABLE_H
#define CBMSTSWTQATREETABLE_H

#include <WTree>
#include <WTreeNode>
#include <WTreeTable>
#include <WTreeTableNode>

#include <set>

using namespace std;
using namespace Wt;

namespace Layout{
  
  typedef std::set<WTreeNode*> TreeNodeSet;

  class CbmStsWtQaTreeTableNode; 
  class CbmStsWtQaTreeTable : public Wt::WTreeTable
  {
  public:
    CbmStsWtQaTreeTable( Wt::WContainerWidget *parent = 0);
    virtual ~CbmStsWtQaTreeTable();
    
    CbmStsWtQaTreeTableNode* getTableNode(){ return fTableNode;}   
    void                     createLayout();
    void                     tableChanged();
    
  protected:
    CbmStsWtQaTreeTableNode * fTableNode;    
  };
  
}
 
#endif // CBMSTSWTQATREETABLE_H
