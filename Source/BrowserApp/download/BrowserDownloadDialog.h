/*
 * Copyright (C) 2011 Igalia S.L.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef BrowserDownloadDialog_h
#define BrowserDownloadDialog_h

#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

G_BEGIN_DECLS

#define BROWSER_TYPE_DOWNLOAD_DIALOG            (browser_download_dialog_get_type())
#define BROWSER_DOWNLOAD_DIALOG(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), BROWSER_TYPE_DOWNLOAD_DIALOG, BrowserDownloadDialog))
#define BROWSER_DOWNLOAD_DIALOG_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  BROWSER_TYPE_DOWNLOAD_DIALOG, BrowserDownloadDialogClass))
#define BROWSER_IS_DOWNLOAD_DIALOG(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), BROWSER_TYPE_DOWNLOAD_DIALOG))
#define BROWSER_IS_DOWNLOAD_DIALOG_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  BROWSER_TYPE_DOWNLOAD_DIALOG))
#define BROWSER_DOWNLOAD_DIALOG_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  BROWSER_TYPE_DOWNLOAD_DIALOG, BrowserDownloadDialogClass))

typedef struct _BrowserDownloadDialog        BrowserDownloadDialog;
typedef struct _BrowserDownloadDialogClass   BrowserDownloadDialogClass;

GType browser_download_dialog_get_type(void);

GtkWidget* browser_download_dialog_new(WebKitDownload *);

#define BROWSER_TYPE_DOWNLOAD (browser_download_get_type())
#define BROWSER_DOWNLOAD(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), BROWSER_TYPE_DOWNLOAD, BrowserDownload))

typedef struct _BrowserDownload BrowserDownload;
typedef struct _BrowserDownloadClass BrowserDownloadClass;

static GType browser_download_get_type();


G_END_DECLS

#endif
