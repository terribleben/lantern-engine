Lantern
=======

Lantern is a piece of boilerplate code that serves as a decent starting point for making 2D games. It is designed to be fairly portable, although at the moment this project builds for iOS.

Stuff it does:
- It takes care of setting up OpenGL ES.
- It makes it easy to load and unload textures.
- It contains utilities for drawing primitives and textures.
- It can draw text using fixed-width texture fonts.
- It has some basic geometry utilities.
- It understands basic kinematics.
- It allows you to design different views which can take control of the graphics loop and hand it off to one another.
- It makes it easy to create particles and forget about them.

About its implementation:
It's written in C++ and OpenGL ES. Right now it is packaged inside an XCode project designed to build for iOS, but the border between C++ and platform-dependent code is kept quite clear, so theoretically it would not be much of a pain to build this for other platforms.

A few parts of it are platform-specific:
- It can handle the "scale" property of a graphics context (in order to support retina display). It works fine if you ignore this.
- The filesystem stuff (e.g. loading textures) is platform dependent.