/*
 * Copyright (c) 2011 Motorola Mobility, Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * Neither the name of Motorola Mobility, Inc. nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#if !defined(__WEBKIT2_H_INSIDE__) && !defined(WEBKIT2_COMPILATION)
#error "Only <webkit2/webkit2.h> can be included directly."
#endif

#ifndef WebKitSettings_h
#define WebKitSettings_h

#include <glib-object.h>
#include <webkit2/WebKitDefines.h>

G_BEGIN_DECLS

#define WEBKIT_TYPE_SETTINGS            (webkit_settings_get_type())
#define WEBKIT_SETTINGS(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_TYPE_SETTINGS, WebKitSettings))
#define WEBKIT_SETTINGS_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_TYPE_SETTINGS, WebKitSettingsClass))
#define WEBKIT_IS_SETTINGS(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_TYPE_SETTINGS))
#define WEBKIT_IS_SETTINGS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_TYPE_SETTINGS))
#define WEBKIT_SETTINGS_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_TYPE_SETTINGS, WebKitSettingsClass))

typedef struct _WebKitSettings WebKitSettings;
typedef struct _WebKitSettingsClass WebKitSettingsClass;
typedef struct _WebKitSettingsPrivate WebKitSettingsPrivate;

struct _WebKitSettings {
    GObject parent_instance;
    WebKitSettingsPrivate *priv;
    GtkWidget *parent_uriEntry;

    guint32 fontNum;
    guint32 fontSizeNum;
    guint32 pageZoomNum;
    //Put all GtkWidget here ,so we can use later. sunhaiming add.
    GtkWidget *window;
    GtkWidget *entry1_general;
    GtkWidget *button1_general;
    GtkWidget *radiobutton1_general;
    GtkWidget *radiobutton2_general;
    GtkWidget *radiobutton3_general;
    GtkWidget *radiobutton4_general;
    GtkWidget *radiobutton5_general;
    GtkWidget *radiobutton6_general;
    GtkWidget * checkbutton1_general;
    GtkWidget *checkbutton2_general;
    GtkWidget *checkbutton3_general;
    GtkWidget *checkbutton4_general;
    GtkWidget *checkbutton5_general;
    GtkWidget *comboboxtext1_content;
    GtkWidget *comboboxtext2_content;
    GtkWidget *comboboxtext3_content;
    GtkWidget *checkbutton1_content;
    GtkWidget *radiobutton1_content;
    GtkWidget *radiobutton2_content;
    GtkWidget *radiobutton3_content;
    GtkWidget *radiobutton4_content;
    GtkWidget *checkbutton3_privacy;
    GtkWidget *comboboxtext1_privacy;
    GtkWidget *checkbutton4_privacy;
    GtkWidget *checkbutton5_privacy;
    GtkWidget *checkbutton6_privacy;
    GtkWidget *checkbutton7_privacy;
    GtkWidget *checkbutton8_privacy;
    GtkWidget *radiobutton1_privacy;
    GtkWidget *radiobutton2_privacy;
    GtkWidget *radiobutton3_privacy;
    GtkWidget *checkbutton9_privacy;
    GtkWidget *radiobutton4_privacy;
    GtkWidget *radiobutton5_privacy;
    GtkWidget *radiobutton6_privacy;
    GtkWidget *button3_privacy;
    GtkWidget *checkbutton1_security;
    GtkWidget *radiobutton1_security;
    GtkWidget *radiobutton2_security;
//lianxx add 14.11.13
    GtkWidget *button1_advanced;
    GtkWidget *button2_advanced;    
	 GtkWidget *button3_advanced;	 
    GtkWidget *button4_advanced;
    GtkWidget *checkbutton1_advanced;
	 GtkWidget *entry1_advanced;
};

enum {
    PROP_0_settings,

    //PROP_ENABLE_JAVASCRIPT, put later   
    //PROP_AUTO_LOAD_IMAGES,  put later
    PROP_LOAD_ICONS_IGNORING_IMAGE_LOAD_SETTING,
    PROP_ENABLE_OFFLINE_WEB_APPLICATION_CACHE,
    PROP_ENABLE_HTML5_LOCAL_STORAGE,
    PROP_ENABLE_HTML5_DATABASE,
    PROP_ENABLE_XSS_AUDITOR,
    PROP_ENABLE_FRAME_FLATTENING,
    PROP_ENABLE_PLUGINS,
    PROP_ENABLE_JAVA,
    PROP_JAVASCRIPT_CAN_OPEN_WINDOWS_AUTOMATICALLY,
    PROP_ENABLE_HYPERLINK_AUDITING,
    //PROP_DEFAULT_FONT_FAMILY, put later
    PROP_MONOSPACE_FONT_FAMILY,
    PROP_SERIF_FONT_FAMILY,
    PROP_SANS_SERIF_FONT_FAMILY,
    PROP_CURSIVE_FONT_FAMILY,
    PROP_FANTASY_FONT_FAMILY,
    PROP_PICTOGRAPH_FONT_FAMILY,
    //PROP_DEFAULT_FONT_SIZE,   put later
    PROP_DEFAULT_MONOSPACE_FONT_SIZE,
    PROP_MINIMUM_FONT_SIZE,
    PROP_DEFAULT_CHARSET,
    PROP_ENABLE_PRIVATE_BROWSING,
    PROP_ENABLE_DEVELOPER_EXTRAS,
    PROP_ENABLE_RESIZABLE_TEXT_AREAS,
    PROP_ENABLE_TABS_TO_LINKS,
    PROP_ENABLE_DNS_PREFETCHING,
    PROP_ENABLE_CARET_BROWSING,
    PROP_ENABLE_FULLSCREEN,
    PROP_PRINT_BACKGROUNDS,
    PROP_ENABLE_WEBAUDIO,
    PROP_ENABLE_WEBGL,
    PROP_ALLOW_MODAL_DIALOGS,
    //PROP_ZOOM_TEXT_ONLY,  put later
    PROP_JAVASCRIPT_CAN_ACCESS_CLIPBOARD,
    PROP_MEDIA_PLAYBACK_REQUIRES_USER_GESTURE,
    PROP_MEDIA_PLAYBACK_ALLOWS_INLINE,
    PROP_DRAW_COMPOSITING_INDICATORS,
    PROP_ENABLE_SITE_SPECIFIC_QUIRKS,
    PROP_ENABLE_PAGE_CACHE,
    PROP_USER_AGENT,
    PROP_ENABLE_SMOOTH_SCROLLING,
    PROP_ENABLE_ACCELERATED_2D_CANVAS,
    PROP_ENABLE_WRITE_CONSOLE_MESSAGES_TO_STDOUT,
    PROP_ENABLE_MEDIA_STREAM,
    PROP_ENABLE_SPATIAL_NAVIGATION,
    PROP_ENABLE_MEDIASOURCE,

 
    //property save start
    PROP_SAVE_START,
    //bool property
    PROP_BOOL_START,
    PROP_MUCH_TAB_WARNING,
    PROP_SHOW_HOMEPAGE_BUTTON,
    PROP_SHOW_BOOKMARKBAR,
    PROP_SHOW_TITLEBAR_AND_MENUBAR,
    PROP_SHOW_FULLSCREEN,
    PROP_ZOOM_TEXT_ONLY, 
    PROP_AUTO_LOAD_IMAGES, 
    PROP_ENABLE_JAVASCRIPT, 
    PROP_PAGE_CONTENT_CACHE,
    PROP_CLEAR_BROWSE_RECORD,
    PROP_CLEAR_DOWNLOAD_RECORD,
    PROP_CLEAR_COOKIE_AND_OTHERS,
    PROP_CLEAR_CACHED_IMAGES_AND_FILES,
    PROP_CLEAR_PASSWORDS,
    PROP_CERTIFICATE_REVOCATION,
	 PROP_ASK_EVERYTIME_BEFORE_DOWN,//lxx add, 14.11.17
    PROP_BOOL_END,
     
    //integer property
    PROP_INTEGER_START,
    PROP_ON_STARTUP,
    PROP_OPEN_NEWPAGE,
    PROP_DEFAULT_FONT_SIZE,
    PROP_HISTORY_SETTING,
    PROP_COOKIE_SETTING,
    PROP_TRACK_LOCATION,
    PROP_MEDIA_ACCESS,
    PROP_INTEGER_END,

    //string property
    PROP_STRING_START,
    PROP_HOME_PAGE,
    PROP_DEFAULT_FONT_FAMILY,
	 PROP_STORE_PATH_OF_DOWNFILE,//lxx add, 14.11.17
    PROP_STRING_END,

    //double property
    PROP_DOUBLE_START,
    PROP_PAGE_ZOOM,
    PROP_DOUBLE_END,
    //property save start
    PROP_SAVE_END
};


extern const gchar* key[];
extern const gchar* font[];
extern const  guint32 font_size[];
extern const gdouble zoom_factor[];

struct _WebKitSettingsClass {
    GObjectClass parent_class;

    void (*_webkit_reserved0) (void);
    void (*_webkit_reserved1) (void);
    void (*_webkit_reserved2) (void);
    void (*_webkit_reserved3) (void);
};

WEBKIT_API bool 
CheckReadValue                                                 (WebKitSettings* settings);

WEBKIT_API void 
SaveInitValueToFile                                            (WebKitSettings* settings);

WEBKIT_API void 
ReSetProperty                                                  (WebKitSettings* settings);

WEBKIT_API void 
InitSettingsWithFile                                           (WebKitSettings* settings);

//lxx add +, delete the preferences file
WEBKIT_API gboolean
DeletePreferencesFile														(void);
//lxx add -, delete the preferences file

WEBKIT_API GType
webkit_settings_get_type(void);

WEBKIT_API WebKitSettings *
webkit_settings_new                                            (void);

WEBKIT_API WebKitSettings *
webkit_settings_new_with_settings                              (const gchar    *first_setting_name,
                                                                ...);

WEBKIT_API gboolean
webkit_settings_get_enable_javascript                          (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_javascript                          (WebKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_auto_load_images                           (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_auto_load_images                           (WebKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_load_icons_ignoring_image_load_setting     (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_load_icons_ignoring_image_load_setting     (WebKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_offline_web_application_cache       (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_offline_web_application_cache       (WebKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_html5_local_storage                 (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_html5_local_storage                 (WebKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_html5_database                      (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_html5_database                      (WebKitSettings *settings,
                                                                gboolean        enabled);
WEBKIT_API gboolean
webkit_settings_get_enable_xss_auditor                         (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_xss_auditor                         (WebKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_frame_flattening                    (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_frame_flattening                    (WebKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_plugins                             (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_plugins                             (WebKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_java                                (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_java                                (WebKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_javascript_can_open_windows_automatically  (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_javascript_can_open_windows_automatically  (WebKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_hyperlink_auditing                  (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_hyperlink_auditing                  (WebKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API const gchar *
webkit_settings_get_default_font_family                        (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_default_font_family                        (WebKitSettings *settings,
                                                                const gchar    *default_font_family);

WEBKIT_API const gchar *
webkit_settings_get_monospace_font_family                      (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_monospace_font_family                      (WebKitSettings *settings,
                                                                const gchar    *monospace_font_family);

WEBKIT_API const gchar *
webkit_settings_get_serif_font_family                          (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_serif_font_family                          (WebKitSettings *settings,
                                                                const gchar    *serif_font_family);

WEBKIT_API const gchar *
webkit_settings_get_sans_serif_font_family                     (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_sans_serif_font_family                     (WebKitSettings *settings,
                                                                const gchar    *sans_serif_font_family);

WEBKIT_API const gchar *
webkit_settings_get_cursive_font_family                        (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_cursive_font_family                        (WebKitSettings *settings,
                                                                const gchar    *cursive_font_family);

WEBKIT_API const gchar *
webkit_settings_get_fantasy_font_family                        (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_fantasy_font_family                        (WebKitSettings *settings,
                                                                const gchar    *fantasy_font_family);

WEBKIT_API const gchar *
webkit_settings_get_pictograph_font_family                     (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_pictograph_font_family                     (WebKitSettings *settings,
                                                                const gchar    *pictograph_font_family);

WEBKIT_API guint32
webkit_settings_get_default_font_size                          (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_default_font_size                          (WebKitSettings *settings,
                                                                guint32         font_size);

WEBKIT_API guint32
webkit_settings_get_default_monospace_font_size                (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_default_monospace_font_size                (WebKitSettings *settings,
                                                                guint32         font_size);

WEBKIT_API guint32
webkit_settings_get_minimum_font_size                          (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_minimum_font_size                          (WebKitSettings *settings,
                                                                guint32         font_size);

WEBKIT_API const gchar *
webkit_settings_get_default_charset                            (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_default_charset                            (WebKitSettings *settings,
                                                                const gchar    *default_charset);

WEBKIT_API gboolean
webkit_settings_get_enable_private_browsing                    (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_private_browsing                    (WebKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_developer_extras                    (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_developer_extras                    (WebKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_resizable_text_areas                (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_resizable_text_areas                (WebKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_tabs_to_links                       (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_tabs_to_links                       (WebKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_dns_prefetching                     (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_dns_prefetching                     (WebKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_caret_browsing                      (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_caret_browsing                      (WebKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_fullscreen                          (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_fullscreen                          (WebKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_print_backgrounds                          (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_print_backgrounds                          (WebKitSettings *settings,
                                                                gboolean        print_backgrounds);

WEBKIT_API gboolean
webkit_settings_get_enable_webaudio                            (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_webaudio                            (WebKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_webgl                               (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_webgl                               (WebKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API void
webkit_settings_set_allow_modal_dialogs                        (WebKitSettings *settings,
                                                                gboolean        allowed);

WEBKIT_API gboolean
webkit_settings_get_allow_modal_dialogs                        (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_zoom_text_only                             (WebKitSettings *settings,
                                                                gboolean        zoom_text_only);

WEBKIT_API gboolean
webkit_settings_get_zoom_text_only                             (WebKitSettings *settings);

WEBKIT_API gboolean
webkit_settings_get_javascript_can_access_clipboard            (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_javascript_can_access_clipboard            (WebKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_media_playback_requires_user_gesture       (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_media_playback_requires_user_gesture       (WebKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_media_playback_allows_inline               (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_media_playback_allows_inline               (WebKitSettings *settings,
                                                                gboolean        enabled);
WEBKIT_API gboolean
webkit_settings_get_draw_compositing_indicators                (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_draw_compositing_indicators                (WebKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_site_specific_quirks                (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_site_specific_quirks                (WebKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_page_cache                          (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_page_cache                          (WebKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API const gchar *
webkit_settings_get_user_agent                                 (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_user_agent                                 (WebKitSettings *settings,
                                                                const gchar    *user_agent);
WEBKIT_API void
webkit_settings_set_user_agent_with_application_details        (WebKitSettings *settings,
                                                                const gchar    *application_name,
                                                                const gchar    *application_version);

WEBKIT_API gboolean
webkit_settings_get_enable_smooth_scrolling                    (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_smooth_scrolling                    (WebKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_accelerated_2d_canvas               (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_accelerated_2d_canvas               (WebKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_write_console_messages_to_stdout    (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_write_console_messages_to_stdout    (WebKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_media_stream                        (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_media_stream                        (WebKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_spatial_navigation                  (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_spatial_navigation                  (WebKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_mediasource                         (WebKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_mediasource                         (WebKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_much_tab_warning                          (WebKitSettings* settings);

WEBKIT_API void 
webkit_settings_set_much_tab_warning                          (WebKitSettings* settings, 
                                                               gboolean muchTabWarning);

WEBKIT_API gboolean 
webkit_settings_get_show_homepage_button                      (WebKitSettings* settings);

WEBKIT_API void 
webkit_settings_set_show_homepage_button                      (WebKitSettings* settings, 
                                                               gboolean showHomepageButton);

WEBKIT_API gboolean 
webkit_settings_get_show_bookmarkbar                          (WebKitSettings* settings);

WEBKIT_API void 
webkit_settings_set_show_bookmarkbar                          (WebKitSettings* settings, 
                                                               gboolean showbookmarkbar);

WEBKIT_API gboolean 
webkit_settings_get_show_titlebar_and_menubar                 (WebKitSettings* settings);

WEBKIT_API void 
webkit_settings_set_show_titlebar_and_menubar                 (WebKitSettings* settings, 
                                                               gboolean showTitlebarAndMenubar);

WEBKIT_API gboolean 
webkit_settings_get_show_fullscreen                            (WebKitSettings* settings);

WEBKIT_API void 
webkit_settings_set_show_fullscreen                            (WebKitSettings* settings, 
                                                                gboolean showFullscreen);

WEBKIT_API gboolean 
webkit_settings_get_page_content_cache                          (WebKitSettings* settings);


WEBKIT_API void 
webkit_settings_set_page_content_cache                          (WebKitSettings* settings, 
                                                                 gboolean pageContentCache);

WEBKIT_API gboolean 
webkit_settings_get_clear_browse_record                         (WebKitSettings* settings);



WEBKIT_API void 
webkit_settings_set_clear_browse_record                         (WebKitSettings* settings, 
                                                                 gboolean clearBrowseRecord);

WEBKIT_API gboolean 
webkit_settings_get_clear_download_record                         (WebKitSettings* settings);



WEBKIT_API void 
webkit_settings_set_clear_download_record                         (WebKitSettings* settings, 
                                                                 gboolean clearDownloadRecord);

WEBKIT_API gboolean 
webkit_settings_get_clear_cookie_and_others                         (WebKitSettings* settings);



WEBKIT_API void 
webkit_settings_set_clear_cookie_and_others                      (WebKitSettings* settings, 
                                                                 gboolean clearCookieAndOthers);

WEBKIT_API gboolean 
webkit_settings_get_clear_cached_images_and_files               (WebKitSettings* settings);



WEBKIT_API void 
webkit_settings_set_clear_cached_images_and_files               (WebKitSettings* settings, 
                                                                 gboolean clearCachedImagesAndFiles);

WEBKIT_API gboolean 
webkit_settings_get_clear_passwords                             (WebKitSettings* settings);



WEBKIT_API void 
webkit_settings_set_clear_passwords                              (WebKitSettings* settings, 
                                                                 gboolean clearPasswords);

WEBKIT_API gboolean 
webkit_settings_get_certificate_revocation                      (WebKitSettings* settings);



WEBKIT_API void 
webkit_settings_set_certificate_revocation                       (WebKitSettings* settings, 
                                                                  gboolean certificateRevocation);
//lxx add +, 14.11.17
WEBKIT_API gboolean 
webkit_settings_get_ask_everytime_before_down                     (WebKitSettings* settings);



WEBKIT_API void 
webkit_settings_set_ask_everytime_before_down                    (WebKitSettings* settings, 
                                                                  gboolean certificateRevocation);
//lxx add -, 14.11.17

WEBKIT_API guint32
webkit_settings_get_on_startup                                 (WebKitSettings *settings);


WEBKIT_API void
webkit_settings_set_on_startup                                 (WebKitSettings *settings,
                                                                guint32        onStartup);

WEBKIT_API guint32
webkit_settings_get_open_newpage                              (WebKitSettings* settings);

WEBKIT_API void 
webkit_settings_set_open_newpage                              (WebKitSettings* settings, 
                                                               guint32 openNewpage);

WEBKIT_API gdouble 
webkit_settings_get_page_zoom                                  (WebKitSettings* settings);

WEBKIT_API void 
webkit_settings_set_page_zoom                                  (WebKitSettings* settings, 
                                                                gdouble pageZoom);


WEBKIT_API guint32 
webkit_settings_get_history_setting                             (WebKitSettings* settings);


WEBKIT_API void 
webkit_settings_set_history_setting                             (WebKitSettings* settings, 
                                                                 guint32 historySetting);

WEBKIT_API guint32 
webkit_settings_get_cookie_setting                              (WebKitSettings* settings);


WEBKIT_API void 
webkit_settings_set_cookie_setting                              (WebKitSettings* settings, 
                                                                guint32 cookieSetting);

WEBKIT_API guint32 
webkit_settings_get_track_location                              (WebKitSettings* settings);


WEBKIT_API void 
webkit_settings_set_track_location                              (WebKitSettings* settings, 
                                                                 guint32 trackLocation);

WEBKIT_API guint32 
webkit_settings_get_media_access                                 (WebKitSettings* settings);


WEBKIT_API void 
webkit_settings_set_media_access                                 (WebKitSettings* settings, 
                                                                  guint32 mediaAccess);

WEBKIT_API const gchar *
webkit_settings_get_home_page                                 (WebKitSettings *settings);


WEBKIT_API void
webkit_settings_set_home_page                                 (WebKitSettings *settings,
                                                                const gchar    *homepage);
//lxx add, 14.11.17
WEBKIT_API const gchar *
webkit_settings_get_path_of_download_file							  (WebKitSettings* settings);

WEBKIT_API void
webkit_settings_set_path_of_download_file							  (WebKitSettings* settings, 
																					const char* storePath);
//lxx add, 14.11.17

G_END_DECLS

#endif /* WebKitSettings_h */
