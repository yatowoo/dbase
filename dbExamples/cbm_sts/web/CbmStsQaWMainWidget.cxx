#include "CbmStsQaWMainWidget.h"
#include "CbmStsQaWTreeView.h"
#include "FairDbWtTreeView.h"

#include "CbmStsQaIvChartView.h"

#include "CsvUtil.h"
#include <iostream>

using namespace std;

CbmStsQaWMainWidget::CbmStsQaWMainWidget(WContainerWidget *parent):
  FairDbWtMainWidget(parent)
{
 
 // Create the Sts Qa main panel
 fNavigation = new WNavigationBar();
 fContentstack = new WStackedWidget();
 
 // Define the Main Navigation bar
 fNavigation->setTitle("FairDb Instances","");
 fNavigation->setResponsive(false);

 WAnimation animation(WAnimation::Fade,
                       WAnimation::Linear,
                       200);
 fContentstack->setTransitionAnimation(animation, true);

 fMenu = new WMenu(fContentstack, NULL);
 fMenu->setInternalPathEnabled();
 fMenu->setInternalBasePath("/");

 
 // Create and Add the menu 
 addToMenu(fMenu, "MySQL@demac013",  new CbmStsQaWTreeView());
 fNavigation->addMenu(fMenu);

 // Add it inside Layout
 WVBoxLayout *layout = new WVBoxLayout(this);
 layout->setContentsMargins(0, 0, 0, 0);
 layout->addWidget(fNavigation);
 layout->addWidget(fContentstack, 1);


}




