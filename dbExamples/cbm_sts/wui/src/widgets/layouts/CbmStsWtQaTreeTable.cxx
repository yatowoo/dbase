#include "CbmStsWtQaTreeTable.h"
#include "CbmStsWtQaTreeTableNode.h"
#include "CbmStsWtQaLogger.h"
#include <WText>
#include <WTree>

using namespace Wt;

namespace Layout
{
  
  CbmStsWtQaTreeTable::CbmStsWtQaTreeTable(Wt::WContainerWidget *parent)
    : WTreeTable(parent)
    ,fTableNode(nullptr)  
  {
     QA_LOG(WLAYOUTS,DEBUG) << "CbmStsWtQaTreeTable ctor()"; 
    createLayout();
  }
  
  CbmStsWtQaTreeTable::~CbmStsWtQaTreeTable()
  {
    QA_LOG(WLAYOUTS,DEBUG)<< "CbmStsWtQaTreeTable dtor()";
  } 

  
  void
  CbmStsWtQaTreeTable::createLayout()
  {
    QA_LOG(WLAYOUTS,DEBUG)<< "CbmStsWtQaTreeTable::CreateLayout()";
    // Create the tree structure  
    WString topName="Browser";
    fTableNode = new CbmStsWtQaTreeTableNode(topName,1);
    fTableNode->createTree();
    setTreeRoot(fTableNode, "Vendor Batchs:");

    treeRoot()->setImagePack("icons/");
    treeRoot()->expand();
    treeRoot()->setNodeVisible(false);
    treeRoot()->setChildCountPolicy(WTreeNode::Enabled);

    // WTree selection mode  
    // Single 
    tree()->setSelectionMode(SingleSelection);
    // Mutliple
    //tree()->setSelectionMode(ExtendedSelection);
    
    tree()->itemSelectionChanged()
      .connect(this, &CbmStsWtQaTreeTable::tableChanged);
          
    // Resize inside Container
    resize(300,WLength::Auto);    
  } 

  void
  CbmStsWtQaTreeTable::tableChanged()
  {
    // Actions after clicking on treeitem
    QA_LOG(WLAYOUTS,DEBUG)<< "CbmStsWtQaTreeTable::tableChanged";
   
  }
  
}//!(namespace layout) 
