import 'package:dart_gtk/dart_gtk.dart';

void main() {
  gtk_init();
  GtkWidget window = gtk_window_new(GtkWindowType.TOPLEVEL);
  gtk_window_set_title(window, 'Window');
  gtk_widget_show(window);
  gtk_main();
}

