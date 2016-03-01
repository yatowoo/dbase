#include "FairDbWtRtopFrame.h"
#include "FairDbWtTFileNode.h"

#include <Wt/WContainerWidget>
#include <Wt/WText>
#include <iostream>

using namespace Wt;
using namespace std;

FairDbWtRtopFrame::FairDbWtRtopFrame(FairDbWtRootBrowser* browser, WContainerWidget *parent)
  : WContainerWidget(parent)
  ,fBrowser(browser)
{
 acceptDrops(FairDbWtTFileNode::FileSelectionMimeType);
}

void FairDbWtRtopFrame::dropEvent(WDropEvent event)
{
  if (event.mimeType() == FairDbWtTFileNode::FileSelectionMimeType) {
    FairDbWtTFileNode* cNode = dynamic_cast<FairDbWtTFileNode*>(event.source());
    //  cout << "-I- FairDbWtRtopFrame dropEvent name:"
    //       <<  cNode->label()->text() << endl;
    // Get Corresponding file node
    if (fBrowser && cNode) fBrowser->analyseTKey(cNode);
  }
}
