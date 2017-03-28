#include "CbmStsWtQaBaseView.h"
#include "CbmStsWtQaLogger.h"

namespace Views{
  
  CbmStsWtQaBaseView::CbmStsWtQaBaseView(WContainerWidget *parent)
    : WContainerWidget(parent)
  {
    QA_LOG(WVIEWS,DEBUG) << "CbmStsWtQaBaseView::ctor()"; 
    
  }
  
  CbmStsWtQaBaseView::~CbmStsWtQaBaseView(){
   QA_LOG(WVIEWS,DEBUG) << "CbmStsWtQaBaseView::dtor";

   fSignal.clear();
  }
  
  
}
