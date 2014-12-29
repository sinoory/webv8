/*

*/

namespace Flummi {
#if 0
    private class Manager : Midori.Extension {
#else
    public class Manager {
        private Midori.App thisApp;
#endif
        private bool bounce () {
            try {
                Midori.App app = thisApp;//this.get_app ();
                Midori.Browser? browser = app.browser;

                if (browser == null || browser.tab == null) {
                    return true;
                }


                Midori.Database database = new Midori.Database ("flummi.db");
                unowned Sqlite.Database db = database.db;

                string sqlcmd = "SELECT id, once, command FROM tasks ORDER BY id;";

                Sqlite.Statement stmt;
                if (db.prepare_v2 (sqlcmd, -1, out stmt, null) != Sqlite.OK) {
                    GLib.critical ("Failed to select from database: %s", db.errmsg ());
                    return false;
                }

                int result = stmt.step ();
                if (!(result == Sqlite.DONE || result == Sqlite.ROW)) {
                    GLib.critical ("Failed to select from database: %s", db.errmsg ());
                    return false;
                }

                Sqlite.Statement del_stmt;
                sqlcmd = "DELETE FROM `tasks` WHERE id = :task_id;";
                if (db.prepare_v2 (sqlcmd, -1, out del_stmt, null) != Sqlite.OK) {
                    GLib.critical ("Failed to update database: %s", db.errmsg ());
                    return false;
                }

                while (result == Sqlite.ROW) {
                    int64 id = stmt.column_int64 (0);
                    int64 once = stmt.column_int64 (1);
                    string command = stmt.column_text (2);

                    string[] commands = { command };

                    if (!app.send_command (commands)) {
                        GLib.critical ("Command failed: %s", command);
                        return false;
                    }

                    if (once > 0) {
                        del_stmt.bind_int64 (del_stmt.bind_parameter_index (":task_id"), id);
                        if (del_stmt.step () != Sqlite.DONE) {
                            GLib.critical ("Failed to delete record %lf.\nError: %s", id, db.errmsg ());
                            return false;
                        }
                    }

                    result = stmt.step ();
                }
            } catch (Midori.DatabaseError schema_error) {
                GLib.error (schema_error.message);
            }

            return false;
        }

        public void activated (Midori.App app) {
            this.thisApp = app;
            GLib.Idle.add (this.bounce);
        }

#if 0
        internal Manager () {
            GLib.Object (name: _("Flummi"),
                         description: _("This extension provides a task queue for update jobs or recurring events."),
                         version: "0.1",
                         authors: "André Stösel <andre@stoesel.de>");

            this.activate.connect (this.activated);
        }
#endif
    }
}

#if 0
public Midori.Extension extension_init () {
    return new Flummi.Manager ();
}
#endif
