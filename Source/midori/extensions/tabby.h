/* tabby.h generated by valac 0.20.1, the Vala compiler, do not modify */


#ifndef __TABBY_H__
#define __TABBY_H__

#include <glib.h>
#include <glib-object.h>
#include "katze/katze.h"
#include "midori/midori.h"
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <float.h>
#include <math.h>
#include <gdk/gdk.h>

G_BEGIN_DECLS


#define TABBY_TYPE_ISTORAGE (tabby_istorage_get_type ())
#define TABBY_ISTORAGE(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TABBY_TYPE_ISTORAGE, TabbyIStorage))
#define TABBY_IS_ISTORAGE(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TABBY_TYPE_ISTORAGE))
#define TABBY_ISTORAGE_GET_INTERFACE(obj) (G_TYPE_INSTANCE_GET_INTERFACE ((obj), TABBY_TYPE_ISTORAGE, TabbyIStorageIface))

typedef struct _TabbyIStorage TabbyIStorage;
typedef struct _TabbyIStorageIface TabbyIStorageIface;

#define TABBY_BASE_TYPE_SESSION (tabby_base_session_get_type ())
#define TABBY_BASE_SESSION(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TABBY_BASE_TYPE_SESSION, TabbyBaseSession))
#define TABBY_BASE_SESSION_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TABBY_BASE_TYPE_SESSION, TabbyBaseSessionClass))
#define TABBY_BASE_IS_SESSION(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TABBY_BASE_TYPE_SESSION))
#define TABBY_BASE_IS_SESSION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TABBY_BASE_TYPE_SESSION))
#define TABBY_BASE_SESSION_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TABBY_BASE_TYPE_SESSION, TabbyBaseSessionClass))

typedef struct _TabbyBaseSession TabbyBaseSession;
typedef struct _TabbyBaseSessionClass TabbyBaseSessionClass;

#define TABBY_TYPE_ISESSION (tabby_isession_get_type ())
#define TABBY_ISESSION(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TABBY_TYPE_ISESSION, TabbyISession))
#define TABBY_IS_ISESSION(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TABBY_TYPE_ISESSION))
#define TABBY_ISESSION_GET_INTERFACE(obj) (G_TYPE_INSTANCE_GET_INTERFACE ((obj), TABBY_TYPE_ISESSION, TabbyISessionIface))

typedef struct _TabbyISession TabbyISession;
typedef struct _TabbyISessionIface TabbyISessionIface;

#define TABBY_TYPE_SESSION_STATE (tabby_session_state_get_type ())

#define TABBY_BASE_TYPE_STORAGE (tabby_base_storage_get_type ())
#define TABBY_BASE_STORAGE(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TABBY_BASE_TYPE_STORAGE, TabbyBaseStorage))
#define TABBY_BASE_STORAGE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TABBY_BASE_TYPE_STORAGE, TabbyBaseStorageClass))
#define TABBY_BASE_IS_STORAGE(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TABBY_BASE_TYPE_STORAGE))
#define TABBY_BASE_IS_STORAGE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TABBY_BASE_TYPE_STORAGE))
#define TABBY_BASE_STORAGE_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TABBY_BASE_TYPE_STORAGE, TabbyBaseStorageClass))

typedef struct _TabbyBaseStorage TabbyBaseStorage;
typedef struct _TabbyBaseStorageClass TabbyBaseStorageClass;
typedef struct _TabbyBaseStoragePrivate TabbyBaseStoragePrivate;
typedef struct _TabbyBaseSessionPrivate TabbyBaseSessionPrivate;

struct _TabbyIStorageIface {
	GTypeInterface parent_iface;
	KatzeArray* (*get_saved_sessions) (TabbyIStorage* self);
	TabbyBaseSession* (*get_new_session) (TabbyIStorage* self);
	void (*restore_last_sessions) (TabbyIStorage* self);
	void (*import_session) (TabbyIStorage* self, KatzeArray* tabs);
};

struct _TabbyISessionIface {
	GTypeInterface parent_iface;
	KatzeArray* (*get_tabs) (TabbyISession* self);
	void (*add_item) (TabbyISession* self, KatzeItem* item);
	void (*attach) (TabbyISession* self, MidoriBrowser* browser);
	void (*restore) (TabbyISession* self, MidoriBrowser* browser);
	void (*remove) (TabbyISession* self);
	void (*close) (TabbyISession* self);
};

typedef enum  {
	TABBY_SESSION_STATE_OPEN,
	TABBY_SESSION_STATE_CLOSED,
	TABBY_SESSION_STATE_RESTORING
} TabbySessionState;

struct _TabbyBaseStorage {
	GObject parent_instance;
	TabbyBaseStoragePrivate * priv;
};

struct _TabbyBaseStorageClass {
	GObjectClass parent_class;
	KatzeArray* (*get_saved_sessions) (TabbyBaseStorage* self);
	TabbyBaseSession* (*get_new_session) (TabbyBaseStorage* self);
	void (*import_session) (TabbyBaseStorage* self, KatzeArray* tabs);
};

struct _TabbyBaseSession {
	GObject parent_instance;
	TabbyBaseSessionPrivate * priv;
	GSList* tab_sorting;
};

struct _TabbyBaseSessionClass {
	GObjectClass parent_class;
	void (*add_item) (TabbyBaseSession* self, KatzeItem* item);
	void (*uri_changed) (TabbyBaseSession* self, MidoriView* view, const gchar* uri);
	void (*data_changed) (TabbyBaseSession* self, MidoriView* view);
	void (*tab_added) (TabbyBaseSession* self, MidoriBrowser* browser, MidoriView* view);
	void (*tab_removed) (TabbyBaseSession* self, MidoriBrowser* browser, MidoriView* view);
	void (*tab_switched) (TabbyBaseSession* self, MidoriView* old_view, MidoriView* new_view);
	void (*tab_reordered) (TabbyBaseSession* self, GtkWidget* tab, guint pos);
	void (*remove) (TabbyBaseSession* self);
	KatzeArray* (*get_tabs) (TabbyBaseSession* self);
	gdouble (*get_max_sorting) (TabbyBaseSession* self);
	void (*close) (TabbyBaseSession* self);
};


GType tabby_base_session_get_type (void) G_GNUC_CONST;
GType tabby_istorage_get_type (void) G_GNUC_CONST;
KatzeArray* tabby_istorage_get_saved_sessions (TabbyIStorage* self);
TabbyBaseSession* tabby_istorage_get_new_session (TabbyIStorage* self);
void tabby_istorage_restore_last_sessions (TabbyIStorage* self);
void tabby_istorage_import_session (TabbyIStorage* self, KatzeArray* tabs);
GType tabby_isession_get_type (void) G_GNUC_CONST;
KatzeArray* tabby_isession_get_tabs (TabbyISession* self);
void tabby_isession_add_item (TabbyISession* self, KatzeItem* item);
void tabby_isession_attach (TabbyISession* self, MidoriBrowser* browser);
void tabby_isession_restore (TabbyISession* self, MidoriBrowser* browser);
void tabby_isession_remove (TabbyISession* self);
void tabby_isession_close (TabbyISession* self);
GType tabby_session_state_get_type (void) G_GNUC_CONST;
GType tabby_base_storage_get_type (void) G_GNUC_CONST;
KatzeArray* tabby_base_storage_get_saved_sessions (TabbyBaseStorage* self);
TabbyBaseSession* tabby_base_storage_get_new_session (TabbyBaseStorage* self);
void tabby_base_storage_start_new_session (TabbyBaseStorage* self);
void tabby_base_storage_import_session (TabbyBaseStorage* self, KatzeArray* tabs);
TabbyBaseStorage* tabby_base_storage_construct (GType object_type);
MidoriApp* tabby_base_storage_get_app (TabbyBaseStorage* self);
void tabby_base_session_add_item (TabbyBaseSession* self, KatzeItem* item);
void tabby_base_session_uri_changed (TabbyBaseSession* self, MidoriView* view, const gchar* uri);
void tabby_base_session_data_changed (TabbyBaseSession* self, MidoriView* view);
void tabby_base_session_tab_added (TabbyBaseSession* self, MidoriBrowser* browser, MidoriView* view);
void tabby_base_session_tab_removed (TabbyBaseSession* self, MidoriBrowser* browser, MidoriView* view);
void tabby_base_session_tab_switched (TabbyBaseSession* self, MidoriView* old_view, MidoriView* new_view);
void tabby_base_session_tab_reordered (TabbyBaseSession* self, GtkWidget* tab, guint pos);
void tabby_base_session_remove (TabbyBaseSession* self);
KatzeArray* tabby_base_session_get_tabs (TabbyBaseSession* self);
gdouble tabby_base_session_get_max_sorting (TabbyBaseSession* self);
void tabby_base_session_close (TabbyBaseSession* self);
gboolean tabby_base_session_delete_event (TabbyBaseSession* self, GtkWidget* widget, GdkEventAny* event);
gdouble tabby_base_session_get_tab_sorting (TabbyBaseSession* self, MidoriView* view);
TabbyBaseSession* tabby_base_session_construct (GType object_type);
MidoriBrowser* tabby_base_session_get_browser (TabbyBaseSession* self);
void tabby_base_session_set_browser (TabbyBaseSession* self, MidoriBrowser* value);
TabbySessionState tabby_base_session_get_state (TabbyBaseSession* self);
void tabby_base_session_set_state (TabbyBaseSession* self, TabbySessionState value);
MidoriExtension* extension_init (void);


G_END_DECLS

#endif