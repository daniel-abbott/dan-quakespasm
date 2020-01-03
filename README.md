This is just a personal version of Quakespasm, mainly for learning and hacking around. Both versions should build with no problems using VS2017 (MSVC), haven't tried building with anything else or for other platforms yet so no promises.

If you want the real Quakespasm, head to http://quakespasm.sourceforge.net

So far, changes/new features include:
* New Graphics Options submenu, currently contains:
    * Texture filter setting (GL_NEAREST, etc)
    * Anisotropy slider (the slider is currently busted, so just prints current cvar on the right)
	* Particle rendering mode, can switch between None, Smooth, and Classic styles.
	* Interpolation switch, can enable/disable animation and movement interpolation (one setting)
		* Has 3 modes, the second uses an interpolation ignore list to skip animations like flames.

You'll probably see some questionable C, I'm still new to the language (and to Quake's way of doing things). Feel free to open issues for anything that just looks awful/dangerous.

Possible goals:
* Add menu(s) with all (most?) console-only graphics options.
* Expand on QuakeC (make new types possible, etc.)
* Re-implement software rendering.
* Add back in the fun classic exit messages.