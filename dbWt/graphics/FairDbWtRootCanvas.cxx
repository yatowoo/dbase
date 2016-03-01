#include "FairDbWtRootCanvas.h"
#include "FairDbWtTFileNode.h"

#include <WFitLayout>
#include <TClass.h>
#include <TH1F.h>
#include <TString.h>
#include <TBufferJSON.h>
#include <iostream>

using namespace std;
using namespace Wt;

FairDbWtRootCanvas::FairDbWtRootCanvas(Type t, TObject *obj,
	                                   int w, int h,
	                                   WContainerWidget* parent)
  : WContainerWidget(parent)
    ,okPressed_(this, "ta")
    ,cancelPressed_(this, "ti")
    ,fType(t)
    ,fObject(obj->Clone())
    ,fW(w)
		,fH(h)
{

// acceptDrops(FairDbWtTFileNode::FileSelectionMimeType);

	cout << "-I- Create WtROOT canvas with " << obj->GetName() << endl;
 // Specs for size
 resize(800,800);

 // activate layout resizing
 setLayoutSizeAware(true);

 // Set scripts
 setJavaScript();
}

void FairDbWtRootCanvas::setJavaScript()
{
	// Here put the dynamic js commands
	// if no object defined return
	if (fObject == nullptr) {
		cout << "-E- FairDbWtRootCanvas null Root Object !" << endl;
		return;
	}

	// Get Handle for Js
	std::string jv_ref = this->jsRef();
	// Inspect Root Object
	const char* obj_name= fObject->GetName();

  	// try to Jsonify the Object
	TString obj_json = TBufferJSON::ConvertToJSON(fObject,3);

	cout << "-I- FairDbWtRootCanvas JSON Convertion of Obj:" << endl;
	cout  << obj_json.Data() << endl;
	cout << "----------------------" << endl;

  // Get the JSon string
	std::string s = obj_json.Data();
	//s.erase(std::remove(s.begin(), s.end(), '\n'), s.end());

	std::stringstream ss;
	ss << "function(object, event) { ";
	ss << "function createHisto(){";
	ss << "var json_str = \'" << s <<"\';";
	ss << "var histo = JSROOT.parse(json_str);";

	switch (fType) {
		case TH1:
		{
			std::string cmd = ss.str() +  "JSROOT.redraw("
			+ jv_ref
			+ ", histo , \'colz\');"
			+ "}"
			+ "JSROOT.AssertPrerequisites(\' 2d \', createHisto,"
			+ jv_ref
			+ ")}";

			fShow.setJavaScript(cmd.c_str());
			fShow.exec();
			resize(WLength::Auto, WLength::Auto);
			break;
		}
		case TCanvas:
		{
			std::string cmd = ss.str() +  "JSROOT.redraw("
			+ jv_ref
			+ ", histo , \'colz\');"
			+ "}"
			+ "JSROOT.AssertPrerequisites(\' 2d \', createHisto,"
			+ jv_ref
			+ ")}";

			fShow.setJavaScript(cmd.c_str());
			fShow.exec();
			resize(WLength::Auto, WLength::Auto);
			break;
		}
		case TGraph:
		{
			std::string cmd = ss.str() +  "JSROOT.redraw("
			+ jv_ref
			+ ", histo , \'colz\');"
			+ "}"
			+ "JSROOT.AssertPrerequisites(\' 2d \', createHisto,"
			+ jv_ref
			+ ")}";

			fShow.setJavaScript(cmd.c_str());
			fShow.exec();
			resize(WLength::Auto, WLength::Auto);
			break;
		}
		case TTree:
		{
			break;
		}
		case TProfile:
		{
			break;
		}
    case TGeoVolume:
		{
			std::string cmd = ss.str() +  "JSROOT.redraw("
			+ jv_ref
			+ ", histo );"
			+ "}"
			+ "JSROOT.AssertPrerequisites(\' geo \', createHisto,"
			+ jv_ref
			+ ")}";

			fShow.setJavaScript(cmd.c_str());
			fShow.exec();
			resize(WLength::Auto, WLength::Auto);

			break;
		}



	}//!switch

}

FairDbWtRootCanvas* FairDbWtRootCanvas::createCanvas(TObject* obj,int w, int h,
	                                                   WContainerWidget* parent)
{
  if ( obj->IsA()->InheritsFrom("TGeoVolume") ) {
	 return new FairDbWtRootCanvas( TGeoVolume, obj, w, h, parent);
	}else if ( obj->IsA()->InheritsFrom("TH1") ) {
		return new FairDbWtRootCanvas( TH1, obj, w, h, parent);
	}else if ( obj->IsA()->InheritsFrom("TCanvas") ) {
		return new FairDbWtRootCanvas( TH1, obj, w, h, parent);
	}else  if ( obj->IsA()->InheritsFrom("TTree") ) {
		return new FairDbWtRootCanvas( TTree, obj, w, h, parent);
	} else{
		cout << "-I- FairDbWtCanvas Unknown object type, name: "
		<< obj->GetName() << " title: " << obj->GetTitle() << endl;
		return nullptr;
	}
}

void FairDbWtRootCanvas::layoutSizeChanged(int w, int h){
          cout << "-I- FairDbWtRootCanvas size changed w : h "
          << w << " : " << h << endl;
					fShow.exec();
					//resize(WLength::Auto, WLength::Auto);
  }


 // void FairDbWtRootCanvas::dropEvent(WDropEvent event)
// {
  // cout << "-------- DROPED EVT in Root Canvas ---------- " <<  endl;
  // if (event.mimeType() == FairDbWtTFileNode::FileSelectionMimeType) {
    // cout << "-I- FairDbWtRootCanvas dropEvent called with mimeType" << endl;
  //  }
// }
