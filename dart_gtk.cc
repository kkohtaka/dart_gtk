// Copyright (c) 2013 Kazumasa Kohtaka. All rights reserved.
// This file is available under the MIT license.

#include "include/dart_api.h"
#include "include/dart_native_api.h"

#include <cstring>
#include <cstdio>
#include <string>
#include <gtk/gtk.h>

Dart_NativeFunction ResolveName(
    Dart_Handle name,
    int argc,
    bool *auto_setup_scope);

DART_EXPORT Dart_Handle dart_gtk_Init(Dart_Handle parent_library) {
  if (Dart_IsError(parent_library)) {
    return parent_library;
  }
  Dart_Handle result_code = Dart_SetNativeResolver(parent_library, ResolveName);
  if (Dart_IsError(result_code)) {
    return result_code;
  }
  return Dart_Null();
}

Dart_Handle HandleError(Dart_Handle handle) {
  if (Dart_IsError(handle)) {
    Dart_PropagateError(handle);
  }
  return handle;
}

std::string string(Dart_Handle handle) {
  if (Dart_IsString(handle)) {
    const char *cstr;
    HandleError(Dart_StringToCString(handle, &cstr));
    return std::string(cstr);
  }
  return std::string();
}

void GtkInit(Dart_NativeArguments arguments) {
  Dart_EnterScope();
  ::gtk_init(0, nullptr);
  Dart_ExitScope();
}

void GtkMain(Dart_NativeArguments arguments) {
  Dart_EnterScope();
  ::gtk_main();
  Dart_ExitScope();
}

void GtkMainQuit(Dart_NativeArguments arguments) {
  Dart_EnterScope();
  ::gtk_main_quit();
  Dart_ExitScope();
}

void GtkWindowNew(Dart_NativeArguments arguments) {
  Dart_EnterScope();
  Dart_Handle dhWindowType = HandleError(Dart_GetNativeArgument(arguments, 0));
  if (Dart_IsInstance(dhWindowType)) {
    Dart_Handle dhTypeString = HandleError(
        Dart_GetField(dhWindowType, Dart_NewStringFromCString("value")));
    std::string typeString = string(dhTypeString);
    ::GtkWindowType type = ::GTK_WINDOW_TOPLEVEL;
    if (typeString.compare("POPUP")) {
      type = ::GTK_WINDOW_POPUP;
    }
    ::GtkWidget *widget = ::gtk_window_new(type);
    Dart_SetReturnValue(
        arguments,
        HandleError(Dart_NewInteger(reinterpret_cast<int64_t>(widget))));
  }
  Dart_ExitScope();
}

void GtkWindowSetTitle(Dart_NativeArguments arguments) {
  Dart_EnterScope();
  Dart_Handle dhWidget = HandleError(Dart_GetNativeArgument(arguments, 0));
  Dart_Handle dhTitle = HandleError(Dart_GetNativeArgument(arguments, 1));
  if (Dart_IsInstance(dhWidget)) {
    Dart_Handle dhPeer = HandleError(
        Dart_GetField(dhWidget, Dart_NewStringFromCString("_peer")));
    int64_t peer;
    Dart_IntegerToInt64(dhPeer, &peer);
    ::GtkWidget *widget = reinterpret_cast< ::GtkWidget *>(peer);
    std::string title = string(dhTitle);
    ::gtk_window_set_title(GTK_WINDOW(widget), title.c_str());
  }
  Dart_ExitScope();
}

void GtkWidgetShow(Dart_NativeArguments arguments) {
  Dart_EnterScope();
  Dart_Handle dhWidget = HandleError(Dart_GetNativeArgument(arguments, 0));
  if (Dart_IsInstance(dhWidget)) {
    Dart_Handle dhPeer = HandleError(
        Dart_GetField(dhWidget, Dart_NewStringFromCString("_peer")));
    int64_t peer;
    Dart_IntegerToInt64(dhPeer, &peer);
    ::GtkWidget *widget = reinterpret_cast< ::GtkWidget *>(peer);
    ::gtk_widget_show(widget);
  }
  Dart_ExitScope();
}

void GSignalConnect(Dart_NativeArguments arguments) {
  Dart_EnterScope();
  Dart_Handle dhWidget = HandleError(Dart_GetNativeArgument(arguments, 0));
  Dart_Handle dhDetailedSignal = HandleError(Dart_GetNativeArgument(arguments, 1));
  if (Dart_IsInstance(dhWidget)) {
    Dart_Handle dhPeer = HandleError(
        Dart_GetField(dhWidget, Dart_NewStringFromCString("_peer")));
    int64_t peer;
    Dart_IntegerToInt64(dhPeer, &peer);
    std::string detailedSignal = string(dhDetailedSignal);
  }
  Dart_ExitScope();
}

struct FunctionLookup {
  const char *name;
  Dart_NativeFunction function;
};

FunctionLookup function_list[] = {
  { "GtkInit", GtkInit },
  { "GtkMain", GtkMain },
  { "GtkMainQuit", GtkMainQuit },
  { "GtkWindowNew", GtkWindowNew },
  { "GtkWindowSetTitle", GtkWindowSetTitle },
  { "GtkWidgetShow", GtkWidgetShow },
  { "GSignalConnect", GSignalConnect },
  { nullptr, nullptr }
};

FunctionLookup no_scope_function_list[] = {
  { nullptr, nullptr }
};

Dart_NativeFunction ResolveName(
    Dart_Handle name,
    int argc,
    bool* auto_setup_scope) {
  if (!Dart_IsString(name)) {
    return nullptr;
  }
  Dart_NativeFunction result = nullptr;
  if (auto_setup_scope == nullptr) {
    return nullptr;
  }
  Dart_EnterScope();
  const char *cname;
  HandleError(Dart_StringToCString(name, &cname));
  for (FunctionLookup *fn = function_list; fn->name != nullptr; ++fn) {
    if (strcmp(fn->name, cname) == 0) {
      *auto_setup_scope = true;
      result = fn->function;
      break;
    }
  }
  if (result != nullptr) {
    Dart_ExitScope();
    return result;
  }
  for (FunctionLookup *fn = no_scope_function_list; fn->name != nullptr; ++fn) {
    if (strcmp(fn->name, cname) == 0) {
      *auto_setup_scope = false;
      result = fn->function;
      break;
    }
  }
  Dart_ExitScope();
  return result;
}

