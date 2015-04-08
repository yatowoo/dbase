#include <iostream>

#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WText>
#include <Wt/WPushButton>


#include "FairDbWtApp.h"
#include "FairDbPopup.h"


FairDbWtApplication::FairDbWtApplication(const WEnvironment& env)
  : WApplication(env)
{
  
// Title of Application
setTitle("FairDB Wt"); 
// Init the externals
//InitJS();

wApp->require("pre-ace/src-noconflict/ace.js");
// try it out 
WContainerWidget *editor = new WContainerWidget(root());
editor->resize(500, 500);

// is a text string that will be the element when executed in JS
std::string editor_ref = editor->jsRef(); 

std::string command = 
  editor_ref + ".editor = ace.edit(" + editor_ref + ");" +
  editor_ref + ".editor.setTheme(\"ace/theme/monokai\");" +
  editor_ref + ".editor.getSession().setMode(\"ace/mode/javascript\");";

editor->doJavaScript(command);


JSignal <std::string> *jsignal = new JSignal<std::string>(editor, "textChanged");
jsignal->connect(this, &FairDbWtApplication::textChanged);

WPushButton *b = new WPushButton("Save", root());

//command = "function(object, event) {" +
//  jsignal->createCall(editor_ref + ".editor.getValue()") +
//  ";}";
//command = "alert(\" vla \");";
command = "JSROOT.BuildSimpleGUI";

JSlot* show = new JSlot();
show->setJavaScript(command);
      
b->clicked().connect(*show);

}


void FairDbWtApplication::textChanged(std::string incoming)
{
 std::cout << "-IIIIIIIII" <<  incoming << std::endl;
}



void FairDbWtApplication::InitJS(){

wApp->useStyleSheet("jsroot/style/JSRootPainter.css");
wApp->useStyleSheet("jsroot/style/JSRootInterface.css");
wApp->useStyleSheet("jsroot/style/jquery-ui.css");

//Include the JS
wApp->require("jsroot/scripts/d3.v3.min.js");
wApp->require("jsroot/scripts/three.min.js");

wApp->require("jsroot/scripts/jquery.min.js");
wApp->require("jsroot/scripts/jquery-ui.min.js");
wApp->require("jsroot/scripts/jquery.mousewheel.js");

wApp->require("jsroot/scripts/touch-punch.min.js");
wApp->require("jsroot/scripts/rawinflate.js");

wApp->require("jsroot/scripts/helvetiker_bold.typeface.js");
wApp->require("jsroot/scripts/helvetiker_regular.typeface.js");

wApp->require("jsroot/scripts/JSRootCore.js");
wApp->require("jsroot/scripts/JSRootInterface.js");
wApp->require("jsroot/scripts/JSRootIOEvolution.js");
wApp->require("jsroot/scripts/JSRootPainter.js");
wApp->require("jsroot/scripts/JSRoot3DPainter.js");

//Include the CSS
//wApp->useStyleSheet("Ventus/examples/simple/css/normalize.css");
//wApp->useStyleSheet("Ventus/examples/simple/css/simple.css");
//wApp->useStyleSheet("Ventus/build/ventus.css");
//wApp->useStyleSheet("Ventus/examples/simple/css/browseralert.css");

//wApp->require("misc/require-min.js");
//Include the JS
//wApp->require("Ventus/vendor/modernizr.js");
//wApp->require("Ventus/vendor/jquery.js");
//wApp->require("Ventus/vendor/handlebars.js");
//wApp->require("Ventus/dist/ventus.js");
//wApp->require("Ventus/src/ventus/wm/window.js");
//wApp->require("Ventus/src/ventus/wm/windowmanager.js");




}



// 
void FairDbWtApplication::f(const std::string &msg)
{
  std::stringstream ss;
  ss << "alert(\"" << msg.c_str() << "\");";
  wApp->doJavaScript(ss.str());
}


//
void FairDbWtApplication::setAmount(const std::string amount)
{
  // Change the confirmation message to include the amount.
  confirmPay_->setMessage("Are you sure you want to pay $" + amount + " ?");

  // Change the default value for the prompt.
  promptAmount_->setDefaultValue(amount);

  // Change the text that shows the current amount.
  currentAmount_->setText("Current amount: $" + promptAmount_->defaultValue());
}


//
void FairDbWtApplication::confirmed()
{
  new WText("<br/>Just payed $" + promptAmount_->defaultValue() + ".", root());
}


//
void FairDbWtApplication::greet()
{
  greeting_->setText("Hello there, " + nameEdit_->text());
}

//
void FairDbWtApplication::TestJS1()
{
 promptAmount_ = FairDbPopup::createPrompt("You need to pay for FairDb, how much?", "",
                      this);
 promptAmount_->okPressed().connect(this, &FairDbWtApplication::setAmount);

 confirmPay_ = FairDbPopup::createConfirm("", this);
 confirmPay_->okPressed().connect(this, &FairDbWtApplication::confirmed);


 new WText("<h2> FairDb Wt </h2>"
        "<p> Database Web Interface "
        " </p>", root());

 currentAmount_
    = new WText("Current amount: $" + promptAmount_->defaultValue(), root());

 WPushButton *amountButton = new WPushButton("Change ...", root());
 amountButton->setMargin(10, Left | Right);

 new WBreak(root());

 WPushButton *confirmButton = new WPushButton("Pay now.", root());
 confirmButton->setMargin(10, Top | Bottom);

// Connect the event handlers to a JSlot: this will execute the JavaScript
// immediately, without a server round trip.
  amountButton->clicked().connect(promptAmount_->show);
  confirmButton->clicked().connect(confirmPay_->show);

  // Set the initial amount
  setAmount("1000");    

}

