#ifndef CBMSTSWTQAPRESENTERVIEW_H
#define CBMSTSWTQAPRESENTERVIEW_H


#include <WToolBar>
#include <WPopupMenu>
#include <WBreak>
#include <WMenuItem>
#include <WMessageBox>
#include <WPopupMenu>
#include <WPushButton>
#include <WText>
#include <WTextArea>

#include <WTemplate>
#include <WContainerWidget>
#include <WTabWidget>
#include <WHBoxLayout>
#include <WVBoxLayout>

#include <string>

#include "CbmStsWtQaBaseView.h"

using namespace Wt;
using namespace std;

namespace Views{
  
  class  CbmStsWtQaPresenterView : public CbmStsWtQaBaseView
  {   
  public:

    CbmStsWtQaPresenterView(WContainerWidget *parent = 0);
    virtual ~CbmStsWtQaPresenterView();

    void init(); 
    void createLayout(WContainerWidget* parent = 0);
    void createTableView(const std::string& batch_id); 

    // Getters
    WTabWidget* getTabTop(){return fTab_top;}
    WTabWidget* getTabDown(){return fTab_down;}

    void showPopupTop(const WMouseEvent& event);
    void showPopupDown(const WMouseEvent& event);
    void popupActionTop();
    void popupActionDown();
    void dialogDoneTop();
    void dialogDoneDown();

    // Features Drawing
    void Show_Iv(int sid=0);
    
  protected:
    Wt::WVBoxLayout                *fVbox;
    Wt::WTabWidget                 *fTab_top;
    Wt::WTabWidget                 *fTab_down;

    WContainerWidget               *fFrame_top;
    WContainerWidget               *fFrame_down;

    WPopupMenu                     *fPopup;
    WMessageBox                    *fPopupActionBox;

  };
  
}

#endif //CBMSTSWTQAPRESENTERIEW_H
