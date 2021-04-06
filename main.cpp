#include <gtkmm/main.h>
#include "ShutdownWindow.hpp"
#include <X11/Xlib.h>


int main(int argc, char *argv[])
{
  /* We run the application with GUI */
  XInitThreads();

  /* Gtk Initialization */
  Gtk::Main kit(argc, argv);
  ShutdownWindow shutdown;

  /* gtkmm main loop */
  Gtk::Main::run(shutdown.GetWindow());
    
  return 0;
}