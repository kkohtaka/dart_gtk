library dart_gtk;

import 'dart-ext:dart_gtk';
part 'gtk/gtk_widget.dart';
part 'gtk/gtk_window.dart';

void gtk_init() native 'GtkInit';
void gtk_main() native 'GtkMain';
void gtk_main_quit() native 'GtkMainQuit';

long g_signal_connect(
    GtkWidget instance,
    String detailedSignal,
    Function handler,
    dynamic data) native 'GSignalConnect';

