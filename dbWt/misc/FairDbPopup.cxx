
#include "FairDbPopup.h"

using namespace Wt;

FairDbPopup::FairDbPopup(Type t, const WString& message, std::string defaultValue,
	     WObject *parent)
  : WObject(parent),
    okPressed_(this, "ok"),
    cancelPressed_(this, "cancel"),
    t_(t),
    message_(message),
    defaultValue_(defaultValue)
{
  setJavaScript();
}

void FairDbPopup::setJavaScript()
{
  /*
   * Sets the JavaScript code.
   *
   * Notice how Wt.emit() is used to emit the okPressed or cancelPressed
   * signal, and how arguments may be passed to it, matching the number and
   * type of arguments in the JSignal definition.
   */
  switch (t_) {
  case Confirm:
    show.setJavaScript
      ("function(){ if (confirm('" + message_.narrow() + "')) {"
       + okPressed_.createCall("''") +
       "} else {"
       + cancelPressed_.createCall() +
       "}}");
    break;
  case Alert:
    show.setJavaScript
      ("function(){ alert('" + message_.narrow() + "');"
       + okPressed_.createCall("''") +
       "}");
    break;
  case Prompt:
    show.setJavaScript
      ("function(){var n = prompt('" + message_.narrow() + "', '"
       + defaultValue_ + "');"
       "if (n != null) {"
       + okPressed_.createCall("n") +
       "} else {"
       + cancelPressed_.createCall() +
       "}}");
  }
}

void FairDbPopup::setMessage(const WString& message)
{
  message_ = message;
  setJavaScript();
}

void FairDbPopup::setDefaultValue(const std::string defaultValue)
{
  defaultValue_ = defaultValue;
  setJavaScript();
}

FairDbPopup *FairDbPopup::createConfirm(const WString& message, WObject *parent)
{
  return new FairDbPopup(Confirm, message, std::string(), parent);
}

FairDbPopup *FairDbPopup::createAlert(const WString& message, WObject *parent)
{
  return new FairDbPopup(Alert, message, std::string(), parent);
}

FairDbPopup *FairDbPopup::createPrompt(const WString& message,
			   const std::string defaultValue, WObject *parent)
{
  return new FairDbPopup(Prompt, message, defaultValue, parent);
}
