#pragma once

#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/box.h>
#include <gtkmm/textview.h>
#include <gtkmm/label.h>
#include <gtkmm/image.h>
#include <gtkmm/invisible.h>
#include <gtkmm/messagedialog.h>
#include <gdkmm/rgba.h>
#include <gdkmm/screen.h>
#include <gdk/gdk.h>
#include <pangomm/fontdescription.h>
#include <dbus-cxx.h>




class ShutdownWindow
{
public:
  ShutdownWindow();
  virtual ~ShutdownWindow();
  Gtk::Window& GetWindow() { return mWindow; }

protected:
  Gtk::Button mShutdownButton;
  Gtk::Button mRestartButton;
  Gtk::Button mCancelButton;
  Gtk::Label mInfoLabel;

  Gtk::Box mWidgetContainer;
  Gtk::Box mBaseContainer;

private:
  Gtk::Window mWindow;
  void Shutdown();
  void Reboot();
  void Cancel();
  bool SendShutdownRequest();

  /* DBus related stuff */
  DBus::Dispatcher::pointer mDispatcher;
  DBus::Connection::pointer mConn;
};
