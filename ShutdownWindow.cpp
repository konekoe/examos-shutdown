#include <iostream>

#include "ShutdownWindow.hpp"


ShutdownWindow::ShutdownWindow()
: 
  mShutdownButton("Shutdown"),
  mRestartButton("Restart"),
  mCancelButton("Cancel"),
  mInfoLabel(""),
  mWidgetContainer(Gtk::ORIENTATION_HORIZONTAL),
  mBaseContainer(Gtk::ORIENTATION_VERTICAL),
  mWindow()
{

  /* Find screen size, set proper size for GUI window */
  Glib::RefPtr<Gdk::Screen> screen = Gdk::Screen::get_default();
  float scalingFactor = (screen->get_height() / 1080.0) * 0.30;

  /* Preset colors */
  GdkRGBA white = {1.0, 1.0, 1.0, 1.0};
  //GdkRGBA gray = {0.0, 0.0, 0.0, 1.0};
  //GdkRGBA white_gray = {0.0, 0.0, 0.0, 0.10};
  GdkRGBA black = {0.0, 0.0, 0.0, 1.0};
  //GdkRGBA red = {1.0, 0.0, 0.0, 1.0};

  /* Set window properties */
  mWindow.set_title("Shutdown");
  mWindow.set_default_size(0, 0);
  mWindow.set_size_request(-1, 1);
  mWindow.set_titlebar(mBaseContainer);
  mWindow.set_position(Gtk::WIN_POS_CENTER);
  mWindow.set_keep_above(true);
  mWindow.override_background_color(Gdk::RGBA(&white, true));


  /* Set base container properties */
  mBaseContainer.pack_start(mInfoLabel);
  mBaseContainer.pack_start(mWidgetContainer);
  mBaseContainer.override_background_color(Gdk::RGBA(&white, true));
  mBaseContainer.set_border_width(20);

  
  /* SET FONT COLOR OF LABELS AND TEXTBOXES */
  Pango::FontDescription font;
  int size = font.get_size();
  font.set_absolute_size(28 * Pango::SCALE);

  mInfoLabel.override_color(Gdk::RGBA(&black, true));
  mInfoLabel.override_font(font);

  
  /* Setup widget container(at bottom) */
  mWidgetContainer.set_border_width(5);
  mWidgetContainer.override_background_color(Gdk::RGBA(&white, true));

  mWidgetContainer.pack_start(mShutdownButton, Gtk::PACK_EXPAND_WIDGET, 5);
  mWidgetContainer.pack_start(mRestartButton, Gtk::PACK_EXPAND_WIDGET, 5);
  mWidgetContainer.pack_start(mCancelButton, Gtk::PACK_EXPAND_WIDGET, 5);

  /* Setup GUI signals */
  mShutdownButton.signal_clicked().connect(sigc::mem_fun(*this, &ShutdownWindow::Shutdown));
  mRestartButton.signal_clicked().connect(sigc::mem_fun(*this, &ShutdownWindow::Reboot));
  mCancelButton.signal_clicked().connect(sigc::mem_fun(*this, &ShutdownWindow::Cancel));

  /* Show initial window */
  mBaseContainer.show();
  mWidgetContainer.show();
  mInfoLabel.set_label("Please choose an action");
  mInfoLabel.show();
  mShutdownButton.show();
  mRestartButton.show();
  mCancelButton.show();

  /* Init DBus to system bus */
  DBus::init(false);
  mDispatcher = DBus::Dispatcher::create();
  mConn = mDispatcher->create_connection( DBus::BUS_SYSTEM );
}



ShutdownWindow::~ShutdownWindow()
{
  
}


void ShutdownWindow::Shutdown()
{
  if(SendShutdownRequest())
  {
    std::system("poweroff");
  }
}

void ShutdownWindow::Reboot()
{
  if(SendShutdownRequest())
  {
    std::system("reboot");
  }
}

void ShutdownWindow::Cancel()
{
  Gtk::Main::quit();
}

bool ShutdownWindow::SendShutdownRequest()
{
  //create an object proxy, which stands in for a real object.
  //a proxy exists over the dbus
  DBus::ObjectProxy::pointer object;
  try
  {
    object = mConn->create_object_proxy("examos.utils.examtool.server", "/examos/utils/examtool");
    DBus::MethodProxy<bool>& RequestShutdown = *(object->create_method<bool>("Examos.Utils.Examtool.Server", "RequestShutdown"));
    if(RequestShutdown())
    {
      return true;
    }
  }
  catch(std::shared_ptr<DBus::Error> er)
  {
    std::stringstream ss;
    ss << "The DBus could not be connected to. If this is the test version of the ExamOS image, "
          "you should not care about this message. However, on a release version, please contact "
          "the exam supervisor.\n\nThe error message was:\n" << er->message() << "\n\nWould you like to power off nevertheless?";
    Gtk::MessageDialog dialog(mWindow, "DBus error", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK_CANCEL);
    dialog.set_secondary_text(ss.str());
    if(dialog.run() == Gtk::RESPONSE_OK)
    {
      return true;
    }
  }
  return false;
}