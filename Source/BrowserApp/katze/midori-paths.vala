/*
 Modified by ZRL
 2014.12.04 实现获取资源路径接口 get_res_dir()
*/

extern const string LIBDIR;
extern const string MDATADIR;
extern const string PACKAGE_NAME;
extern const string SYSCONFDIR;
extern const string MIDORI_VERSION_SUFFIX;
const string MODULE_PREFIX = "lib";
const string MODULE_SUFFIX = "." + GLib.Module.SUFFIX;

namespace Midori {
    public enum RuntimeMode {
        UNDEFINED,
        NORMAL,
        APP,
        PRIVATE,
        PORTABLE
    }

    namespace Paths {
        static string? exec_path = null;
        static string[] command_line = null;
        static string? runtime_dir = null;
        static RuntimeMode mode = RuntimeMode.UNDEFINED;

        static string? config_dir = null;
        static string? readonly_dir = null;
        static string? cache_dir = null;
        static string? cache_dir_for_reading = null;
        static string? user_data_dir = null;
        static string? user_data_dir_for_reading = null;
        static string? tmp_dir = null;

        namespace Test {
            public void reset_runtime_mode () {
                mode = RuntimeMode.UNDEFINED;
            }
        }

        public static string get_config_dir_for_reading () {
            assert (mode != RuntimeMode.UNDEFINED);
            return readonly_dir ?? config_dir;
        }

        /* returns the path to a user configuration file whose contents should not be modified.
        to get the path to save settings, use get_config_filename() */
        public static string get_config_filename_for_reading (string filename) {
            assert (mode != RuntimeMode.UNDEFINED);
            return Path.build_path (Path.DIR_SEPARATOR_S,
                readonly_dir ?? config_dir, filename);
        }

        public bool is_readonly () {
            assert (mode != RuntimeMode.UNDEFINED);
            return readonly_dir != null;
        }

        public RuntimeMode get_runtime_mode () {
            assert (mode != RuntimeMode.UNDEFINED);
            return mode;
        }

        public static unowned string get_runtime_dir () {
            if (runtime_dir != null)
                return runtime_dir;

            #if HAVE_WIN32
            runtime_dir = Environment.get_variable ("XDG_RUNTIME_DIR");
            if (runtime_dir == null || runtime_dir == "")
                runtime_dir = Environment.get_user_data_dir ();
            #else
            runtime_dir = Environment.get_variable ("XDG_RUNTIME_DIR");
            if (runtime_dir == null || runtime_dir == "") {
                runtime_dir = Path.build_path (Path.DIR_SEPARATOR_S,
                    Environment.get_tmp_dir (), PACKAGE_NAME + "-" + Environment.get_user_name ());
                mkdir_with_parents (runtime_dir);
                stdout.printf ("ZRL runtime_dir = %s \n", runtime_dir);
                return runtime_dir;
            }
            #endif
            runtime_dir = Path.build_path (Path.DIR_SEPARATOR_S, runtime_dir, PACKAGE_NAME);
            mkdir_with_parents (runtime_dir);
            stdout.printf ("ZRL runtime_dir2 = %s \n", runtime_dir);
            return runtime_dir;
        }

        public static void init (RuntimeMode new_mode, string? config) {
            assert (mode == RuntimeMode.UNDEFINED);
            assert (new_mode != RuntimeMode.UNDEFINED);
            mode = new_mode;
            if (mode == RuntimeMode.PORTABLE || mode == RuntimeMode.PRIVATE)
                Gtk.Settings.get_default ().gtk_recent_files_max_age = 0;
            if (mode == RuntimeMode.PORTABLE) {
                config_dir = Path.build_path (Path.DIR_SEPARATOR_S,
                    exec_path, "profile", "config");
                cache_dir = Path.build_path (Path.DIR_SEPARATOR_S,
                    exec_path, "profile", "cache");
                user_data_dir = Path.build_path (Path.DIR_SEPARATOR_S,
                    exec_path, "profile", "misc");
                tmp_dir = Path.build_path (Path.DIR_SEPARATOR_S,
                    exec_path, "profile", "tmp");
            }
            else if (mode == RuntimeMode.APP) {
                config_dir = Path.build_path (Path.DIR_SEPARATOR_S,
                    Environment.get_user_data_dir (), PACKAGE_NAME, "apps",
                    Checksum.compute_for_string (ChecksumType.MD5, config, -1));
                cache_dir = Path.build_path (Path.DIR_SEPARATOR_S,
                    Environment.get_user_cache_dir (), PACKAGE_NAME);
                user_data_dir_for_reading = Environment.get_user_data_dir ();
                tmp_dir = get_runtime_dir ();
            }
            else if (mode == RuntimeMode.PRIVATE) {
                string? real_config = config != null && !Path.is_absolute (config)
                    ? Path.build_filename (Environment.get_current_dir (), config) : config;
                readonly_dir = real_config ?? Path.build_path (Path.DIR_SEPARATOR_S,
                    Environment.get_user_config_dir (), PACKAGE_NAME);
                cache_dir_for_reading = Path.build_path (Path.DIR_SEPARATOR_S,
                    Environment.get_user_cache_dir (), PACKAGE_NAME);
                user_data_dir_for_reading = Environment.get_user_data_dir ();
                tmp_dir = get_runtime_dir ();
            }
            else {
#if HAVE_WEBKIT2_3_91
// ZRL disable multiple process
#if 0
                /* Allow WebKit to spawn more than one rendering process */
                if (!("wk2:no-multi-render-process" in (Environment.get_variable ("MIDORI_DEBUG") ?? ""))) {
                    WebKit.WebContext.get_default ().set_process_model (WebKit.ProcessModel.MULTIPLE_SECONDARY_PROCESSES);
                }
#endif
#endif
                string? real_config = config != null && !Path.is_absolute (config)
                    ? Path.build_filename (Environment.get_current_dir (), config) : config;
                config_dir = real_config ?? Path.build_path (Path.DIR_SEPARATOR_S,
                    Environment.get_user_config_dir (), PACKAGE_NAME);
                cache_dir = Path.build_path (Path.DIR_SEPARATOR_S,
                    Environment.get_user_cache_dir (), PACKAGE_NAME);
                user_data_dir = Environment.get_user_data_dir ();
                tmp_dir = get_runtime_dir ();
            }
#if HAVE_WEBKIT2
            if (cache_dir != null) {
                /* Cache and extension dir MUST be set no later than here to work */
                /*WebKit.WebContext.get_default ().set_web_extensions_directory (
                    Path.build_path (Path.DIR_SEPARATOR_S, cache_dir, "wk2ext"));*/
                WebKit.WebContext.get_default ().set_disk_cache_directory (
                    Path.build_path (Path.DIR_SEPARATOR_S, cache_dir, "web"));
            }

            if (config_dir != null) {
                var cookie_manager = WebKit.WebContext.get_default ().get_cookie_manager ();
                cookie_manager.set_persistent_storage (Path.build_filename (config_dir, "cookies.db"),
                    WebKit.CookiePersistentStorage.SQLITE);
            }
#endif
            if (user_data_dir != null) {
                string folder = Path.build_filename (user_data_dir, "webkit", "icondatabase");
#if HAVE_WEBKIT2
                WebKit.WebContext.get_default ().set_favicon_database_directory (folder);
#else
                WebKit.get_favicon_database ().set_path (folder);
#endif
            }
            else
            {
#if HAVE_WEBKIT2
                /* with wk2 set_favicon_database_directory can only be called once and actually
                initializes and enables the favicon database, so we do not call it in this case */
#else
                /* wk1 documentation claims that the favicon database is not enabled unless
                a call to favicon_database.set_path is made, but in fact it must be explicitly
                disabled by setting to null (verified as of webkitgtk 2.3.1) */
                WebKit.get_favicon_database ().set_path (null);
#endif
            }

                stdout.printf ("ZRL config: %s\ncache: %s\nuser_data: %s\ntmp: %s\n",
                               config_dir, cache_dir, user_data_dir, tmp_dir);

            if (strcmp (Environment.get_variable ("MIDORI_DEBUG"), "paths") == 0) {
                stdout.printf ("config: %s\ncache: %s\nuser_data: %s\ntmp: %s\n",
                               config_dir, cache_dir, user_data_dir, tmp_dir);
            }
        }

        public static void mkdir_with_parents (string path, int mode = 0700) {
            /* Use g_access instead of g_file_test for better performance */
            if (Posix.access (path, Posix.F_OK) == 0)
                return;
            int i = path.index_of_char (Path.DIR_SEPARATOR, 0);
            do {
                string fn = path.substring (i, -1);
                if (Posix.access (fn, Posix.F_OK) != 0) {
                    if (DirUtils.create (fn, mode) == -1) {
                        /* Slow fallback; if this fails we fail */
                        DirUtils.create_with_parents (path, mode);
                        return;
                    }
                }
                else if (!FileUtils.test (fn, FileTest.IS_SYMLINK))
                    return; /* Failed */

                i = path.index_of_char (Path.DIR_SEPARATOR, i);
            }
            while (i != -1);
        }

        public static void remove_path (string path) {
            try {
                var dir = Dir.open (path, 0);
                string? name;
                while (true) {
                    name = dir.read_name ();
                    if (name == null)
                        break;
                    remove_path (Path.build_filename (path, name));
                }
            }
            catch (Error error) {
                FileUtils.remove (path);
            }
        }

        public static unowned string get_config_dir_for_writing () {
            assert (config_dir != null);
            mkdir_with_parents (config_dir);
            return config_dir;
        }

        public static string get_extension_config_dir (string extension) {
            assert (config_dir != null);
            string folder;
            if ("." in extension)
                folder = Path.build_filename (config_dir, "extensions", extension);
            else
                folder = Path.build_filename (config_dir, "extensions",
                    MODULE_PREFIX + extension + "." + GLib.Module.SUFFIX);
            mkdir_with_parents (folder);

            //stdout.printf ("ZRL extension_config_dir = %s\n", folder);
            return folder;
        }

        public static string get_extension_preset_filename (string extension, string filename) {
            assert (exec_path != null);
            string preset_filename = extension;
            if (extension.has_prefix (MODULE_PREFIX))
                preset_filename = extension.split (MODULE_PREFIX)[1];
            if (extension.has_suffix (MODULE_SUFFIX))
                preset_filename = preset_filename.split (MODULE_SUFFIX)[0];
            //return get_preset_filename (Path.build_filename ("extensions", preset_filename), filename);
            string tmp1 = get_preset_filename (Path.build_filename ("extensions", preset_filename), filename);
            //stdout.printf ("ZRL extension_preset_filename = %s, preset_filename = %s\n", tmp1, preset_filename);
            return tmp1;
        }

        /* returns the path to a user configuration file to which it is permitted to write.
        this is also necessary for files whose state is synchronized to disk by a manager,
        e.g. cookies. */
        public static string get_config_filename_for_writing (string filename) {
            assert (mode != RuntimeMode.UNDEFINED);
            assert (config_dir != null);
            mkdir_with_parents (config_dir);
            //return Path.build_path (Path.DIR_SEPARATOR_S, config_dir, filename);
            string tmp1 = Path.build_path (Path.DIR_SEPARATOR_S, config_dir, filename);
            //stdout.printf ("ZRL config_filename_for_writing = %s, filename = %s\n", tmp1, filename);
            return tmp1;
        }

        public static unowned string get_cache_dir () {
            assert (cache_dir != null);
            return cache_dir;
        }

        public static unowned string get_user_data_dir () {
            assert (user_data_dir != null);
            return user_data_dir;
        }

        public static unowned string get_user_data_dir_for_reading () {
            assert (user_data_dir_for_reading != null || user_data_dir != null);
            if (user_data_dir != null)
                return user_data_dir;
            return user_data_dir_for_reading;
        }

        public static unowned string get_cache_dir_for_reading () {
            assert (cache_dir_for_reading != null || cache_dir != null);
            if (cache_dir != null)
                return cache_dir;
            return cache_dir_for_reading;
        }

        public static unowned string get_tmp_dir () {
            assert (tmp_dir != null);
            return tmp_dir;
        }

        public static string make_tmp_dir (string tmpl) {
            assert (tmp_dir != null);
            try {
                return DirUtils.make_tmp (tmpl);
            }
            catch (Error error) {
                GLib.error (error.message);
            }
        }

        public static void init_exec_path (string[] new_command_line) {
            assert (command_line == null);
            command_line = new_command_line;
            #if HAVE_WIN32
            exec_path = Environment.get_variable ("MIDORI_EXEC_PATH") ??
                win32_get_package_installation_directory_of_module ();
            #else
            string? executable;
            try {
                if (!Path.is_absolute (command_line[0])) {
                    string program = Environment.find_program_in_path (command_line[0]);
                    if (FileUtils.test (program, FileTest.IS_SYMLINK))
                        executable = FileUtils.read_link (program);
                    else
                        executable = program;
                }
                else
                    executable = FileUtils.read_link (command_line[0]);
            }
            catch (Error error) {
                executable = command_line[0];
            }

            exec_path = File.new_for_path (executable).get_parent ().get_parent ().get_path ();
            //stdout.printf ("ZRL exec_path = %s \n", exec_path);
            #endif
            if (strcmp (Environment.get_variable ("MIDORI_DEBUG"), "paths") == 0) {
                stdout.printf ("command_line: %s\nexec_path: %s\nres: %s\nlib: %s\n",
                               get_command_line_str (true), exec_path,
                               get_res_filename ("about.css"), get_lib_path (PACKAGE_NAME));
            }
        }

        public static unowned string[] get_command_line () {
            assert (command_line != null);
            return command_line;
        }

        public static string get_command_line_str (bool for_display) {
            assert (command_line != null);
            if (for_display)
                return string.joinv (" ", command_line).replace (Environment.get_home_dir (), "~");
            return string.joinv (" ", command_line).replace ("--debug", "").replace ("-g", "")
                .replace ("--diagnostic-dialog", "").replace ("-d", "");
        }

        public static string get_lib_path (string package) {
            assert (command_line != null);
            string path = Path.build_filename (exec_path, "lib", package);
            //stdout.printf ("ZRL get_lib_path1 = %s, package = %s\n", path, package);
            if (Posix.access (path, Posix.F_OK) == 0)
                return path;

            if (package == PACKAGE_NAME) {
                /* Fallback to build folder */
                path = Path.build_filename ((File.new_for_path (exec_path).get_path ()), "extensions");
                //stdout.printf ("ZRL get_lib_path2 = %s \n", path);
                if (Posix.access (path, Posix.F_OK) == 0)
                    return path;
            }

// ZRL add extra path for lib search.
#if 0
            return Path.build_filename (LIBDIR, PACKAGE_NAME);
#else
            path = Path.build_filename (LIBDIR, PACKAGE_NAME);
            //stdout.printf ("ZRL get_lib_path3 = %s \n", path);
            if (Posix.access (path, Posix.F_OK) == 0)
                return path;

            //only search LIBDIR.
            path = Path.build_filename (LIBDIR, "");
            //stdout.printf ("ZRL get_lib_path4 = %s \n", path);
            return path;
#endif
        }

        public static string get_res_filename (string filename) {
            assert (command_line != null);
            assert (filename != "");
            #if HAVE_WIN32
            return Path.build_filename (exec_path, "share", PACKAGE_NAME, "res", filename);
            #else
            string path = Path.build_filename (exec_path, "share", PACKAGE_NAME, "res", filename);
            //stdout.printf ("ZRL get_res_filename1 = %s, filename = %s\n", path, filename);
            if (Posix.access (path, Posix.F_OK) == 0)
                return path;

//ZRL add extra path.
#if 0
            return build_folder ("data", null, filename) ??
              Path.build_filename (MDATADIR, PACKAGE_NAME, "res", filename);
#else
            path =  build_folder ("data", null, filename) ??
              Path.build_filename (MDATADIR, PACKAGE_NAME, "res", filename);
            //stdout.printf ("ZRL get_res_filename2 = %s, filename = %s\n", path, filename);
            if (Posix.access (path, Posix.F_OK) == 0)
                return path;

            path = Path.build_filename (MDATADIR, "", "res", filename);
            //stdout.printf ("ZRL get_res_filename3 = %s, filename = %s\n", path, filename);
            return path;
#endif
            #endif
        }

        /* ZRL 获取资源路径接口 2 Path to search: exec_path/share/package_name/res & MDATADIR/PACKAGE_NAME/res */
        public static string get_res_dir () {
            assert (command_line != null);

            string path = Path.build_filename (exec_path, "share", PACKAGE_NAME, "res", null);
            if (Posix.access (path, Posix.F_OK) == 0)
                return path;

            path = Path.build_filename (MDATADIR, PACKAGE_NAME, "res", null);
            return path;
        }

        #if !HAVE_WIN32
        string? build_folder (string folder, string? middle, string filename) {
            /* Fallback to build folder */
            File? parent = File.new_for_path (exec_path);
            while (parent != null) {
                var data = parent.get_child (folder);
                if (middle != null)
                    data = data.get_child (middle);
                var child = data.get_child (filename);
                if (child.query_exists ())
                    return child.get_path ();
                parent = parent.get_parent ();
            }
            return null;
        }
        #endif

        /* returns the path to a file containing read-only data installed with the application
        if @res is true, looks in the midori resource folder specifically */
        public static string get_data_filename (string filename, bool res) {
            assert (command_line != null);
            string res1 = res ? PACKAGE_NAME : "";
            string res2 = res ? "res" : "";

            #if HAVE_WIN32
            return Path.build_filename (exec_path, "share", res1, res2, filename);
            #else
            string path = Path.build_filename (get_user_data_dir_for_reading (), res1, res2, filename);
            //stdout.printf ("ZRL get_data_filename1 = %s, filename = %s\n", path, filename);
            if (Posix.access (path, Posix.F_OK) == 0)
                return path;

            foreach (string data_dir in Environment.get_system_data_dirs ()) {
                path = Path.build_filename (data_dir, res1, res2, filename);
                //stdout.printf ("ZRL get_data_filename2 = %s, filename = %s\n", path, filename);
                if (Posix.access (path, Posix.F_OK) == 0)
                    return path;
            }

// ZRL
#if 0
            return Path.build_filename (MDATADIR, res1, res2, filename);
#else
            string tmp1 = Path.build_filename (MDATADIR, res1, res2, filename);
            stdout.printf ("ZRL get_data_filename3 = %s, filename = %s\n", tmp1, filename);
            return tmp1;
#endif
            #endif
        }

        /* returns the path to a file containing system default configuration */
        public static string get_preset_filename (string? folder, string filename) {
            assert (exec_path != null);

            foreach (string config_dir in Environment.get_system_config_dirs ()) {
                string path = Path.build_filename (config_dir, PACKAGE_NAME, folder ?? "", filename);
                //stdout.printf ("ZRL get_preset_filename1 = %s, filename = %s\n", path, filename);
                if (Posix.access (path, Posix.F_OK) == 0)
                    return path;
            }

// ZRL
#if 0
            return build_folder ("config", folder, filename) ??
              Path.build_filename (SYSCONFDIR, "xdg", PACKAGE_NAME, folder ?? "", filename);
#else
            string path = build_folder ("config", folder, filename) ??
              Path.build_filename (SYSCONFDIR, PACKAGE_NAME, folder ?? "", filename);
            //stdout.printf ("ZRL get_preset_filename2 = %s, filename = %s\n", path, filename);
            return path;
#endif
        }

        public static void clear_icons () {
            assert (cache_dir != null);
            assert (user_data_dir != null);
#if HAVE_WEBKIT2
            WebKit.WebContext.get_default ().get_favicon_database ().clear ();
#else
            WebKit.get_favicon_database ().clear ();
#endif
            /* FIXME: Exclude search engine icons */
            remove_path (Path.build_filename (user_data_dir, "webkit", "icondatabase"));
        }

        /**
         * Looks up a pixbuf for the given @uri. If @widget is given a generic
         * file icon is used in case there's no icon.
         *
         * Deprecated: 0.5.8: Use Midori.URI.Icon or Midori.URI.get_icon instead.
         **/
        public static Gdk.Pixbuf? get_icon (string? uri, Gtk.Widget? widget) {
            if (!Midori.URI.is_resource (uri))
                return null;
            int icon_width = 16, icon_height = 16;
            if (widget != null)
                Gtk.icon_size_lookup_for_settings (widget.get_settings (),
                    Gtk.IconSize.MENU, out icon_width, out icon_height);
            else
                icon_width = icon_height = 0 /* maximum size */;

            //add by zgh
            Gdk.Pixbuf? pixbuf = WebKit.WebContext.get_default ().get_favicon_database ()
                .try_get_favicon_pixbuf (uri, icon_width, icon_height);
            if (pixbuf != null)
                return pixbuf;
            //end add by zgh

            if (widget != null)
                return widget.render_icon (Gtk.STOCK_FILE, Gtk.IconSize.MENU, null);
            return null;
        }
    }
}
