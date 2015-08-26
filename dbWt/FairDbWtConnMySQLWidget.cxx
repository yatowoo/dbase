
#include <sstream>
#include <iostream>
#include <boost/algorithm/string.hpp>

#include "FairDbWtConnMySQLWidget.h"
#include "FairDbWtItemsTpl.h"
#include "FairDbWtDeferredWidget.h"

#include "FairDbWtTreeView.h"

using namespace std;
using namespace Wt;


namespace {

int countSpaces(const std::string& line) {
  for (unsigned int pos=0; pos<line.length(); ++pos) {
    if (line[pos] != ' ')
      return pos;
  }
  return line.length();
}

std::string skipSpaces(const std::string& line, unsigned int count) {
  if (line.length() >= count)
    return line.substr(count);
  else
    return std::string();
}

}



FairDbWtConnMySQLWidget::FairDbWtConnMySQLWidget()
  : WContainerWidget()
{
  //addText(tr("formwidgets-intro"), this);
 cout << "------ FairDbWtConnMySQLWidget created " << endl;
 addWidget( new  Wt::WText("There is no better place!"));
 //addWidget( new  FairDbWtTreeView());

}

void FairDbWtConnMySQLWidget::populateSubMenu(WMenu *menu)
{
 // menu->addItem("Introduction", introduction())->setPathComponent("");
}



WText * FairDbWtConnMySQLWidget::addText(const WString& s,
        WContainerWidget *parent)
{
  WText *text = new WText(s, parent);
  bool literal;
  literal = s.literal();
  if (!literal)
    text->setInternalPathEncoding(true);
  return text;
}



WString  FairDbWtConnMySQLWidget::reindent(const WString& text)
{
  std::vector<std::string> lines;
  std::string s = text.toUTF8();
  boost::split(lines, s, boost::is_any_of("\n"));

  std::string result;
  int indent = -1;
  int newlines = 0;
  for (unsigned i = 0; i < lines.size(); ++i) {
    const std::string& line = lines[i];

    if (line.empty()) {
      ++newlines;
    } else {
      if (indent == -1) {
        indent = countSpaces(line);
      } else {
        for (int j = 0; j < newlines; ++j)
          result += '\n';
      }

      newlines = 0;

      if (!result.empty())
        result += '\n';

      result += skipSpaces(line, indent);
    }
  }
  return Wt::WString::fromUTF8(result);
}



WWidget *FairDbWtConnMySQLWidget::introduction()
{
 

 /* WTemplate *result = new FairDbWtItemsTpl("forms-introduction");
  result->bindWidget("SimpleForm", SimpleForm());
  result->bindWidget("FormModel", FormModel());

  // Show the XML-templates as text
  result->bindString("simpleForm-template",
                     reindent(tr("simpleForm-template")), Wt::PlainText);
  result->bindString("form-field",
                     reindent(tr("form-field")), Wt::PlainText);
  result->bindString("userForm-template",
                     reindent(tr("userForm-template")), Wt::PlainText);

 */

  return NULL;
}






