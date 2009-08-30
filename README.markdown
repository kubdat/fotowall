Fotowall
========

Fotowall is a creative tool that allows you to layout your photos or pictures
in a personal way. You can add pictures, then resize, move, change colors, text,
shadows, etc..
Check out the [home page](http://code.google.com/p/fotowall) and the
[blog](https://enricoros.wordpress.com/tag/fotowall) too.

*Focus is on simplicity, express your creativity! ;-)*

ChangeLog
---------

Version 0.8 (unreleased):

* Image Cropping (by Arnaud Tanguy)
* Shaped text (bezier curve)
* Properties in the top bar
* Window transparency (needs compositing)
* OpenGL support (where available)
* Fixed bugs
* ___minor changes:
* picture opacity (by Arnaud Tanguy)
* add ok button for finalizing the text and just close the tab with X (req. by Rossana)
* network image loading from http (req. by Rossana)
* pictures on background: keep ratio in some way (req. by Rossana)
* Video -> Webcam (req. by Rossana)
* FotoWall -> Fotowall (req. by Rossana)
* restore the common behavior for rotating (left click)
* fixes from Georges Dubus (intro overflow and translation) and Andreas Brech (bad accelereators, scale with all corners)
* animations with 4.6: top bar, item show/hide, properties show/hide
* builds with 4.4 (without many features), 4.5 (without animations), 4.6-unreleased (gfx errors on windows)
* Polish translation? ask Marek Walczak, see 4ba1502
* fix: same folder when you open the picture (req. by Rossana)
* fix: when loading a .fotowall file from the command line some items are aligned to the left
* fix: resizing works only with the bottom right corner. fix the others.

Version 0.7.1 (2009-07-23):

* Flickr content download
* Qt 4.6 GFX cuteness and network speed (4.6 highly recommended)
* Rubberband selection of items
* Prettified properties dialogs
* Better load/save (even of network objects)
* Load additional frames + 1 new frame (by Arnaud Tanguy)
* German translation (by Martin Zimmermann)
* Check for updates
* Fixed many bugs

Version 0.6 (2009-04-29):

* PosteRazor Integration
* Live Video (Linux only, requires a WebCam)
* Export Wizard (Wallpaper, Image, PosteRazor, Print)
* XML file format (will ensure compatibility from now on)
* Multi-items selection (to move, delete, set properties, ...)
* All-in-one Scale+Rotate button (different with LMB/RMB and Shift/Ctrl)
* Perspective Transform
* Fixed Size mode
* Sepia Effect
* Scale Text like Images

Version 0.5 (2009-04-11):

* CD/DVD Cover composition and printing (by Arnaud Tanguy)
* Rich Text (with extended editing)
* Glow effect (by Arnaud Tanguy)
* Properties configurable for all objects [reqby Ilan d'Inca]
* Usability fixes [reqby Ilan d'Inca]
* Save to all the supported image formats (by Arnaud Tanguy)
* Properties close button [reqby Ilan d'Inca]
* Improved stacking (fixes + relative to neighbourhoods) [reqby Ilan d'Inca]
* Add the 'No effect' effect removal option (by Arnaud Tanguy)
* French Introduction translation (by Arnaud Tanguy)
* Translation of the Introduction text (by Arnaud Tanguy)
* Fall back to QTextDocument where QtWebkit is not available

Version 0.4 (2009-03-29):

* Windows executable on [github](http://github.com/enricoros/Fotowall/downloads)
* Image Effects (by Arnaud Tanguy)
* French Translation (by Arnaud Tanguy)
* German translation [Martin]
* Apply properties to All

Version 0.3.1 (2009-03-13):

* builds with Qt 4.4
* Brazilian translation [Marcio Moraes]
* Configuration Panel: right click on an image and edit properties
* More Frame types (default, heart, Plasma backgrounds) [David Eccher]
* More Image Formats (bmp, tiff, gif, ...) [Superstoned]
* Reflections, and really cool ones
* Mirror the photo by clicking on an axis [Alberto Scarpa]
* DoubleClick on a picture -> set it as background, and vice-versa [Alberto Scarpa]
* More pretty (plasma icons) and usable (behavior) frame buttons
* Stacking order [Superstoned]
* Export a single picture as-you-see-it on disk
* Fixed-ratio resize: hold CTRL while resizing
* Visual Help [Alberto Scarpa]
* Drag&Drop: better behavior and can change a picture inside an existing frame too
* Pictures stay on-screen


Sharing Ideas
-------------

I hope to expand the program as soon as I can, since there is many room for
improvement and the code is kept clean and small.

- You can join the development.. the whole stuff is done with Qt 4.5 or higher
  using the GraphicsView framework.
- And if you have any Idea or Suggestion, just drop me a line, on the project
  homepage or by mail at  enrico.ros@gmail.com ;-)

### Current Wishlist

* mosaic effect: from 1 picture to a grid of misaligned pics
* svg drawings [balloons, signs, etc..]
* masked pic
* both new and old resize&co
* webcam: on win32
* webcam: multiple webcams
* add the undo/redo support
* update the video tutorial (still at 0.2!)
* add the "color changing stuff" to the picture themselves (req. by Contrast, Superstoned)
* add google images, like flickr ones (req. by Rossana)
* win: photo dragging from the 'add picture' filedialog (seems harder that it should be. works on linux) (req. by Rossana)
* auto-placement [force field, shape, collage, etc..]
  * ap: perform rectangular image composition based on an input picture (color, pattern)
  * ap: perform image distribution based on shapes or probability sampling (on a sprayed grayscale canvas)
* Google-suggest completion (already in, but misplaced when embedded into qgraphicsview)
* integrate kde-apps knowledgebase (already in, but doesn't work great)
* regional links to web resources
 - italian: http://www.dylanblog.com/2009/05/27/creare-dei-wallpaper-personalizzati-con-fotowall-061/
 - french: ubuntu-fr ?
* relicences
 - 3rdparty/enricomath.h: Could you re-release it with GPL v2+ like the rest of your source?
 - 3rdparty/richtextedit/richtexteditor*: redo 'cause non free
 - ./GlowEffectWidget.cpp: GPLv2 with all rights reserved, a no-go
 - videocapture is partially BSD and LGPLv2+ - not mentioned + no complete copy of lgplv2 available (add the file into that folder)
 - posterazor GPLv3+ need license file
 - remove qm? note that building on windows requires qm to be on the packages!


Building
--------

This Fotowall package provides source code; to make an executable out of the
sources you have to know a little about programming or ask one of your friends
to do it ;-) Or just grab an 'executable package' from the project homepage.

In Linux/X11, Mac, or Windows, install the latest Qt development packages that
you will find on http://www.trolltech.com:

- on linux this is tested with Qt 4.4/4.5/4.6-git and GCC 4.0+
- on windows this is tested witn Qt 4.5.2+ and MSVC Express 2005 and 2008
- on mac it is tested with Qt 4.5+

Just open a command prompt (win), shell (linux, mac) and go the sources
location and issue the followings commands:

- qmake  (or qmake-qt4 if your system is weird)
- make   (or nmake, if you're on windows)

The executable will be built and you can start enjoying Fotowall! ;-)


Old News
--------

**2009-03-02** Fotowall is now on GitHub. Development will continue here. Changes are coming soon!

**2008-09-03** Peter Upfold put together a great ScreenCast of the application!!

See http://fosswire.com/2008/09/03/Fotowall-make-wallpaper-collages-from-your-photos/ fot the article and click on the Video link to see the screencast.

**2008-08-01** Thanks to everybody that is sending me patches!

I really appreciate that, even if I don't have time right now to check and apply them.. but I'm willing to give everybody the SVN access, so drop me a note if you want to contribute!

**2008-04-14** Updated this page including the Wishlist

**2008-04-06** Fotowall 0.21 released

Changes in this release are: relicensing and a new program icon

**2008-04-06** Imported the project into Google Code
