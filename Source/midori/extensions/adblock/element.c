/* element.c generated by valac 0.20.1, the Vala compiler
 * generated from element.vala, do not modify */

/*
 Copyright (C) 2014 Christian Dywan <christian@twotoasts.de>

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 See the file COPYING for the full license text.
*/

#include <glib.h>
#include <glib-object.h>
#include <stdlib.h>
#include <string.h>
#include <gio/gio.h>
#include <stdio.h>


#define ADBLOCK_TYPE_FEATURE (adblock_feature_get_type ())
#define ADBLOCK_FEATURE(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), ADBLOCK_TYPE_FEATURE, AdblockFeature))
#define ADBLOCK_FEATURE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), ADBLOCK_TYPE_FEATURE, AdblockFeatureClass))
#define ADBLOCK_IS_FEATURE(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ADBLOCK_TYPE_FEATURE))
#define ADBLOCK_IS_FEATURE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), ADBLOCK_TYPE_FEATURE))
#define ADBLOCK_FEATURE_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), ADBLOCK_TYPE_FEATURE, AdblockFeatureClass))

typedef struct _AdblockFeature AdblockFeature;
typedef struct _AdblockFeatureClass AdblockFeatureClass;
typedef struct _AdblockFeaturePrivate AdblockFeaturePrivate;

#define ADBLOCK_TYPE_DIRECTIVE (adblock_directive_get_type ())

#define ADBLOCK_TYPE_ELEMENT (adblock_element_get_type ())
#define ADBLOCK_ELEMENT(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), ADBLOCK_TYPE_ELEMENT, AdblockElement))
#define ADBLOCK_ELEMENT_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), ADBLOCK_TYPE_ELEMENT, AdblockElementClass))
#define ADBLOCK_IS_ELEMENT(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ADBLOCK_TYPE_ELEMENT))
#define ADBLOCK_IS_ELEMENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), ADBLOCK_TYPE_ELEMENT))
#define ADBLOCK_ELEMENT_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), ADBLOCK_TYPE_ELEMENT, AdblockElementClass))

typedef struct _AdblockElement AdblockElement;
typedef struct _AdblockElementClass AdblockElementClass;
typedef struct _AdblockElementPrivate AdblockElementPrivate;
#define _g_hash_table_unref0(var) ((var == NULL) ? NULL : (var = (g_hash_table_unref (var), NULL)))
#define _g_free0(var) (var = (g_free (var), NULL))

typedef enum  {
	ADBLOCK_DIRECTIVE_ALLOW,
	ADBLOCK_DIRECTIVE_BLOCK
} AdblockDirective;

struct _AdblockFeature {
	GObject parent_instance;
	AdblockFeaturePrivate * priv;
};

struct _AdblockFeatureClass {
	GObjectClass parent_class;
	gboolean (*header) (AdblockFeature* self, const gchar* key, const gchar* value);
	gboolean (*parsed) (AdblockFeature* self, GFile* file);
	AdblockDirective* (*match) (AdblockFeature* self, const gchar* request_uri, const gchar* page_uri, GError** error);
	void (*clear) (AdblockFeature* self);
};

struct _AdblockElement {
	AdblockFeature parent_instance;
	AdblockElementPrivate * priv;
	GHashTable* blockcssprivate;
};

struct _AdblockElementClass {
	AdblockFeatureClass parent_class;
};

struct _AdblockElementPrivate {
	gboolean debug_element;
};


static gpointer adblock_element_parent_class = NULL;

GType adblock_feature_get_type (void) G_GNUC_CONST;
GType adblock_directive_get_type (void) G_GNUC_CONST;
GType adblock_element_get_type (void) G_GNUC_CONST;
#define ADBLOCK_ELEMENT_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), ADBLOCK_TYPE_ELEMENT, AdblockElementPrivate))
enum  {
	ADBLOCK_ELEMENT_DUMMY_PROPERTY
};
AdblockElement* adblock_element_new (void);
AdblockElement* adblock_element_construct (GType object_type);
AdblockFeature* adblock_feature_construct (GType object_type);
static void adblock_element_real_clear (AdblockFeature* base);
static void _g_free0_ (gpointer var);
gchar* adblock_element_lookup (AdblockElement* self, const gchar* domain);
void adblock_element_insert (AdblockElement* self, const gchar* domain, const gchar* value);
static void adblock_element_finalize (GObject* obj);


static gboolean string_contains (const gchar* self, const gchar* needle) {
	gboolean result = FALSE;
	const gchar* _tmp0_;
	gchar* _tmp1_ = NULL;
#line 1238 "/usr/share/vala-0.20/vapi/glib-2.0.vapi"
	g_return_val_if_fail (self != NULL, FALSE);
#line 1238 "/usr/share/vala-0.20/vapi/glib-2.0.vapi"
	g_return_val_if_fail (needle != NULL, FALSE);
#line 1239 "/usr/share/vala-0.20/vapi/glib-2.0.vapi"
	_tmp0_ = needle;
#line 1239 "/usr/share/vala-0.20/vapi/glib-2.0.vapi"
	_tmp1_ = strstr ((gchar*) self, (gchar*) _tmp0_);
#line 1239 "/usr/share/vala-0.20/vapi/glib-2.0.vapi"
	result = _tmp1_ != NULL;
#line 1239 "/usr/share/vala-0.20/vapi/glib-2.0.vapi"
	return result;
#line 118 "element.c"
}


AdblockElement* adblock_element_construct (GType object_type) {
	AdblockElement * self = NULL;
	const gchar* _tmp0_ = NULL;
	gchar* _tmp1_;
	gchar* _tmp2_;
	const gchar* _tmp3_;
	const gchar* _tmp5_;
	gboolean _tmp6_ = FALSE;
#line 18 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	self = (AdblockElement*) adblock_feature_construct (object_type);
#line 19 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	_tmp0_ = g_getenv ("MIDORI_DEBUG");
#line 19 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	_tmp1_ = g_strdup (_tmp0_);
#line 19 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	_tmp2_ = _tmp1_;
#line 19 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	_tmp3_ = _tmp2_;
#line 19 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	if (_tmp3_ == NULL) {
#line 142 "element.c"
		gchar* _tmp4_;
#line 19 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
		_tmp4_ = g_strdup ("");
#line 19 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
		_g_free0 (_tmp2_);
#line 19 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
		_tmp2_ = _tmp4_;
#line 150 "element.c"
	}
#line 19 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	_tmp5_ = _tmp2_;
#line 19 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	_tmp6_ = string_contains (_tmp5_, "adblock:element");
#line 19 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	self->priv->debug_element = _tmp6_;
#line 17 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	_g_free0 (_tmp2_);
#line 17 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	return self;
#line 162 "element.c"
}


AdblockElement* adblock_element_new (void) {
#line 17 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	return adblock_element_construct (ADBLOCK_TYPE_ELEMENT);
#line 169 "element.c"
}


static void _g_free0_ (gpointer var) {
#line 23 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	var = (g_free (var), NULL);
#line 176 "element.c"
}


static void adblock_element_real_clear (AdblockFeature* base) {
	AdblockElement * self;
	GHashFunc _tmp0_;
	GEqualFunc _tmp1_;
	GHashTable* _tmp2_;
#line 22 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	self = (AdblockElement*) base;
#line 23 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	_tmp0_ = g_str_hash;
#line 23 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	_tmp1_ = g_str_equal;
#line 23 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	_tmp2_ = g_hash_table_new_full (_tmp0_, _tmp1_, _g_free0_, _g_free0_);
#line 23 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	_g_hash_table_unref0 (self->blockcssprivate);
#line 23 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	self->blockcssprivate = _tmp2_;
#line 197 "element.c"
}


gchar* adblock_element_lookup (AdblockElement* self, const gchar* domain) {
	gchar* result = NULL;
	GHashTable* _tmp0_;
	const gchar* _tmp1_;
	gconstpointer _tmp2_ = NULL;
	gchar* _tmp3_;
#line 26 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	g_return_val_if_fail (self != NULL, NULL);
#line 26 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	g_return_val_if_fail (domain != NULL, NULL);
#line 27 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	_tmp0_ = self->blockcssprivate;
#line 27 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	_tmp1_ = domain;
#line 27 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	_tmp2_ = g_hash_table_lookup (_tmp0_, _tmp1_);
#line 27 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	_tmp3_ = g_strdup ((const gchar*) _tmp2_);
#line 27 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	result = _tmp3_;
#line 27 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	return result;
#line 223 "element.c"
}


void adblock_element_insert (AdblockElement* self, const gchar* domain, const gchar* value) {
	gboolean _tmp0_;
	GHashTable* _tmp4_;
	const gchar* _tmp5_;
	gchar* _tmp6_;
	const gchar* _tmp7_;
	gchar* _tmp8_;
#line 30 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	g_return_if_fail (self != NULL);
#line 30 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	g_return_if_fail (domain != NULL);
#line 30 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	g_return_if_fail (value != NULL);
#line 31 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	_tmp0_ = self->priv->debug_element;
#line 31 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	if (_tmp0_) {
#line 244 "element.c"
		FILE* _tmp1_;
		const gchar* _tmp2_;
		const gchar* _tmp3_;
#line 32 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
		_tmp1_ = stdout;
#line 32 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
		_tmp2_ = domain;
#line 32 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
		_tmp3_ = value;
#line 32 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
		fprintf (_tmp1_, "Element to be blocked %s => %s\n", _tmp2_, _tmp3_);
#line 256 "element.c"
	}
#line 33 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	_tmp4_ = self->blockcssprivate;
#line 33 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	_tmp5_ = domain;
#line 33 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	_tmp6_ = g_strdup (_tmp5_);
#line 33 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	_tmp7_ = value;
#line 33 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	_tmp8_ = g_strdup (_tmp7_);
#line 33 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	g_hash_table_insert (_tmp4_, _tmp6_, _tmp8_);
#line 270 "element.c"
}


static void adblock_element_class_init (AdblockElementClass * klass) {
#line 13 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	adblock_element_parent_class = g_type_class_peek_parent (klass);
#line 13 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	g_type_class_add_private (klass, sizeof (AdblockElementPrivate));
#line 13 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	ADBLOCK_FEATURE_CLASS (klass)->clear = adblock_element_real_clear;
#line 13 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	G_OBJECT_CLASS (klass)->finalize = adblock_element_finalize;
#line 283 "element.c"
}


static void adblock_element_instance_init (AdblockElement * self) {
#line 13 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	self->priv = ADBLOCK_ELEMENT_GET_PRIVATE (self);
#line 290 "element.c"
}


static void adblock_element_finalize (GObject* obj) {
	AdblockElement * self;
#line 13 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	self = G_TYPE_CHECK_INSTANCE_CAST (obj, ADBLOCK_TYPE_ELEMENT, AdblockElement);
#line 14 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	_g_hash_table_unref0 (self->blockcssprivate);
#line 13 "/home/zrl/epiphany_webkit/Webkit2Browser/Source/midori/extensions/adblock/element.vala"
	G_OBJECT_CLASS (adblock_element_parent_class)->finalize (obj);
#line 302 "element.c"
}


GType adblock_element_get_type (void) {
	static volatile gsize adblock_element_type_id__volatile = 0;
	if (g_once_init_enter (&adblock_element_type_id__volatile)) {
		static const GTypeInfo g_define_type_info = { sizeof (AdblockElementClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) adblock_element_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (AdblockElement), 0, (GInstanceInitFunc) adblock_element_instance_init, NULL };
		GType adblock_element_type_id;
		adblock_element_type_id = g_type_register_static (ADBLOCK_TYPE_FEATURE, "AdblockElement", &g_define_type_info, 0);
		g_once_init_leave (&adblock_element_type_id__volatile, adblock_element_type_id);
	}
	return adblock_element_type_id__volatile;
}



