/* Created by ZRL. The entry to all built-in extensions
*/

private class ExtensionsManager : Midori.Extension {
    private About.Manager about_manager;
    private Apps.Manager apps_manager;
    private ExternalApplications.Manager open_with_manager;
    private Tabby.Manager tabby_manager;
    private DelayedLoad.Manager delayload_manager;
    private Flummi.Manager flummi_manager;
    //private Transfers.Manager download_manager;

    public void activated (Midori.App app) {
        about_manager.activated(app);
        apps_manager.activated(app);
        open_with_manager.activated(app);
        tabby_manager.activated(app);
        delayload_manager.activated(app);
        flummi_manager.activated(app);
        //download_manager.activated(app);
    }

    public void deactivated () {
        Midori.App app = this.get_app ();
        about_manager.deactivated(app);
        apps_manager.deactivated(app);
        open_with_manager.deactivated(app);
        tabby_manager.deactivated(app);
        delayload_manager.deactivated(app);
        //download_manager.deactivated(app);
    }

    internal ExtensionsManager () {
        GLib.Object (name: "Built-in Extension",
                     description: "All build-in extensions",
                     version: "0.1",
                     authors: "BrowserTeam");

        about_manager = new About.Manager();
        apps_manager = new Apps.Manager ();
        open_with_manager = new ExternalApplications.Manager ();
        tabby_manager = new Tabby.Manager ();
        delayload_manager = new DelayedLoad.Manager ();
        flummi_manager = new Flummi.Manager ();
        //download_manager = new Transfers.Manager();

        this.activate.connect (this.activated);
        this.deactivate.connect (this.deactivated);
    }
}

public Midori.Extension extension_init () {
    return new ExtensionsManager ();
}
