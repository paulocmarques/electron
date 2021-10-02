// Copyright (c) 2019 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef SHELL_BROWSER_UI_GTK_UTIL_H_
#define SHELL_BROWSER_UI_GTK_UTIL_H_

#include <gtk/gtk.h>

class SkBitmap;

namespace gtk_util {

const char* GettextPackage();
const char* GtkGettext(const char* str);

const char* GetCancelLabel();
const char* GetOpenLabel();
const char* GetSaveLabel();
const char* GetOkLabel();
const char* GetNoLabel();
const char* GetYesLabel();

// Convert and copy a SkBitmap to a GdkPixbuf. NOTE: this uses BGRAToRGBA, so
// it is an expensive operation.  The returned GdkPixbuf will have a refcount of
// 1, and the caller is responsible for unrefing it when done.
GdkPixbuf* GdkPixbufFromSkBitmap(const SkBitmap& bitmap);

}  // namespace gtk_util

#endif  // SHELL_BROWSER_UI_GTK_UTIL_H_
