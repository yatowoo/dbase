#include "CbmStsWtQaTreeTableNode.h"
#include "CbmStsWtQaDataObjects.h"
#include "CbmStsDbQa.h"
#include "CbmStsWtQaLogger.h"

#include "FairDbWtParFactory.h"

#include <WContainerWidget>
#include <WStandardItemModel>
#include <WStandardItem>
#include <WDoubleSpinBox>
#include <WLabel>
#include <WText>

#include <map>
#include <tuple>
#include <iostream>



typedef tuple <int, int, int> tuple_uid;
typedef multimap<string, tuple_uid> batch_uid_map;

using namespace Database;
using namespace std;
using namespace Wt;

namespace Layout
{
  
  CbmStsWtQaTreeTableNode::CbmStsWtQaTreeTableNode(WString& pName, bool isDir)
    : WTreeTableNode(pName, createIcon(isDir))
  {
    QA_LOG(WLAYOUTS,DEBUG) << "CbmStsWtQaTreeTableNode::ctor()";
    label()->setTextFormat(PlainText);
  }
  
  
  CbmStsWtQaTreeTableNode::~CbmStsWtQaTreeTableNode()
  {
    QA_LOG(WLAYOUTS,DEBUG) << "CbmStsWtQaTreeTableNode::dtor()";
  }

  WIconPair *CbmStsWtQaTreeTableNode::createIcon(bool isDir)
  {
    if (isDir)
      return new WIconPair("icons/yellow-folder-closed.png",
                           "icons/yellow-folder-open.png", false);
    else
      return new WIconPair("icons/document.png",
                           "icons/yellow-folder-open.png", false);
  }
  
  
  void CbmStsWtQaTreeTableNode::createTree()
  {
    QA_LOG(WLAYOUTS,DEBUG) << "CbmStsWtQaTreeTableNode::createTree()";
    
    const CbmStsDbQa& stsQa =  CbmStsWtQaDataObjects::instance().getStsQa();
    
    QA_LOG(WLAYOUTS,INFO) << "CbmStsTreeTableNode:: n_suid: "
                          << stsQa.GetNumSUID() << " n_sensors: "
                          << stsQa.GetNumSensors(); 
    
    // Create the Node tree
    for ( int i = 0 ; i< CbmStsWtQaDataObjects::instance().getNVendors(); i++){  
      // Create Main Vendor node
      const string vendor_name =  CbmStsWtQaDataObjects::instance().getVendorName(i);
      const BatchIdMap b_uid_map =  CbmStsWtQaDataObjects::instance().getBatchIdMap(i);
      WTreeTableNode *pNode = new WTreeTableNode(vendor_name.data(),createIcon(true));
      addChildNode(pNode);
      
      QA_LOG(WLAYOUTS, INFO) << " CbmStsTreeTableNode:: Vendor: " << vendor_name << " with batch map size: "<< b_uid_map.size();
      
      if (b_uid_map.size() == 0 ) continue;
      
      // Loop over the batch_ids 
      BatchIdMap::const_iterator it = b_uid_map.begin();
      string batch_str="";
      while(it != b_uid_map.end())
        {
          IdTuple val = it->second;
          string abatch = static_cast<string>(it->first);
          //cout << "Key = " << abatch <<"  tuple: " <<  std::get<0>(val) 
          //     << " : "    << std::get<1>(val)     << " : " << std::get<2>(val)  <<endl;
          it++;
          // Create Batch sub-node
          if ( abatch.compare(batch_str) !=0 ){
            WTreeTableNode *tNode = new WTreeTableNode(abatch.data(),createIcon(false));
            tNode->setSelectable(true);
            tNode->setInteractive(true);
            pNode->addChildNode(tNode);
            batch_str = abatch;
          }
        }
    }//!(for vendor_id)
    
    const vector<IdTuple>& v =  CbmStsWtQaDataObjects::instance().getBatchIdTuples("CiS","351135");
    for(auto it = v.begin(); it != v.end(); ++it) {
      IdTuple val = *it;
      cout << " 351135 subset  tuple: " <<  std::get<0>(val) 
           << " : "    << std::get<1>(val)     << " : " << std::get<2>(val)  <<endl;
      
    }      
    
  }
  
}
