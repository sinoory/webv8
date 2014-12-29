/*

*/

namespace DelayedLoad {
#if 0
    private class Manager : Midori.Extension {
#else
    public class Manager {
#endif
        private void tab_changed (Midori.View? old_view, Midori.View? new_view) {
            if (new_view != null) {
                unowned Katze.Item item = new_view.get_proxy_item ();

                int64 delay = item.get_meta_integer ("delay");
                if (delay == Midori.Delay.PENDING_UNDELAY && new_view.progress < 1.0) {
                    new_view.reload (true);
                }
            }
        }

        private void browser_added (Midori.Browser browser) {
            browser.switch_tab.connect_after (this.tab_changed);
        }

        private void browser_removed (Midori.Browser browser) {
            browser.switch_tab.disconnect (this.tab_changed);
        }

        public void activated (Midori.App app) {
            foreach (Midori.Browser browser in app.get_browsers ()) {
                browser_added (browser);
            }
            app.add_browser.connect (browser_added);
        }

#if 0
        public void deactivated () {
            Midori.App app = get_app ();
#else
        public void deactivated (Midori.App app) {
#endif
            foreach (Midori.Browser browser in app.get_browsers ()) {
                browser_removed (browser);
            }
            app.add_browser.disconnect (browser_added);
        }

#if 0
        internal Manager () {
            GLib.Object (name: _("Delayed load"),
                         description: _("Delay page load until you actually use the tab."),
                         version: "0.2",
                         authors: "André Stösel <andre@stoesel.de>");

            activate.connect (this.activated);
            deactivate.connect (this.deactivated);
        }
#endif
    }
}

#if 0
public Midori.Extension extension_init () {
    return new DelayedLoad.Manager ();
}
#endif

