This is just a personal version of Quakespasm, mainly for learning and hacking around. SDL2 version should build with no problems with VS2017, haven't tried building with anything else or for other platforms yet so no promises.

If you want the real Quakespasm, head to http://quakespasm.sourceforge.net

So far, changes/new features include:
* New GL Options submenu, currently contains:
    * Texture filter setting
    * Anisotropy slider (the slider is currently busted, so just prints current cvar on the right)

You'll probably see some questionable C, I'm still new to the language (and to Quake's way of doing things). Feel free to open issues for anything that just looks awful/dangerous.

Possible goals:
* Add menu(s) with all (most?) console-only graphics options.
* Expand on QuakeC (make new types possible, etc.)
* Re-implement software rendering.
* Add back in the fun classic exit messages.