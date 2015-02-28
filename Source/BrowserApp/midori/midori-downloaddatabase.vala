/*
 Copyright (C) 2013 Christian Dywan <christian@twotoats.de>

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 See the file COPYING for the full license text.
*/

namespace Midori {
    public class DownloadDatabase : Midori.Database {
        public DownloadDatabase (GLib.Object? app) throws DatabaseError {
            Object (path: "download.db");
            init ();
        }
        
        public bool clear () throws DatabaseError {
            unowned string sqlcmd = """
                DELETE FROM download WHERE create_time IN (SELECT create_time FROM download ORDER BY create_time DESC LIMIT 100,-1);
                """;
            var statement = prepare (sqlcmd);
            return statement.exec ();
        }
    }
}
