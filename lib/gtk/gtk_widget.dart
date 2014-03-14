part of dart_gtk;

class GtkWindowType {
  static const TOPLEVEL = const GtkWindowType._('TOPLEVEL');
  static const POPUP = const GtkWindowType._('POPUP');
  final String value;
  const GtkWindowType._(this.value);
}

class GtkWidget {
  int _peer;
  GtkWidget._(this._peer);
}

GtkWidget gtk_window_new(GtkWindowType type) {
  return new GtkWidget._(_gtk_window_new(type));
}

int _gtk_window_new(GtkWindowType type) native 'GtkWindowNew';
void gtk_widget_show(GtkWidget widget) native 'GtkWidgetShow';

