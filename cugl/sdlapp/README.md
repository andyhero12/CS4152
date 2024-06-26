# SDL App Creator
---
This directory is a copy of SDL with installation scripts specifically to 
create runnable applications (and not just SDL libraries). The applications 
created will be linked to a private copy of SDL, ensuring that the application 
uses the correct version of SDL at all times. This copy of SDL will be packaged 
with the application deliverable (either as a static library or a bundled 
shared library). While this is often derided as not the Unix way, neither Android nor iOS have package managers.

This directory is itself a Python script, whose purpose is to create the 
appropriate IDE project for the application. While CMake is still supported in 
this copy of SDL, mobile applications such as iOS and Anroid need further 
customization not supported by CMake. In addition, the XCode and Visual Studio 
projects generated by this script are significantly cleaner and easier to debug 
than those generated by CMake.

## Project Setup

To use that app creator, you should first have an app directory. This 
directory should contain a YAML configuration file. This file is used to 
specify the location of the following 

- Location of all source code files
- Location of any additional include files
- The game assets (textures, sounds, etc)
- The application icon

Except for the source code files (which must include a `main`), all of these are 
optional. The icon should be specified as a 1024x1024 PNG file.

## Setup Script

The python setup script assumes that the following packages are installed:

- [PyYAML](https://pyyaml.org)
- [Pillow](https://pillow.readthedocs.io/en/stable/)
- [shortuuid](https://pypi.org/project/shortuuid/)

The first two are for reading the configuration file and generating the icon 
assets, respectively. The script will fail if they are not present. The last 
package is for UUID generation for XCode projects. Without it, generated XCode 
projects will have weaker uniqueness guarantees on their UUIDs.

To create an SDL application, run this folder as a python program as follows

    python sdl2 <project>

where `project` is a directory containing `config.yml`. This is a YAML 
configuration file describing all of the application properties. See the provided 
demos for an explanation of the various settings. 

Most build options are specified in the config file. However, the script does 
support some command line arguments as well. They are as follows.

- `-b, --build `   The build directory
- `-c, --config`   The configuration file
- `-t, --target`   The active build target

The latter option can be used to specify a single build target. It can be one of 
`android`, `apple`, `macos`, `ios`, `windows` or `cmake`. The `cmake` option supports all of the platforms that SDL does, though we have only tested it against Linux and the platforms listed above, and we have made modifications to the default SDL CMake file.  The other options create an IDE project for the given platform.  For example, `android` creates an Android Studio project while `ios` or `macos` create an XCode project. The `windows` option creates a Visual Studio project. All projects are fully configured and ready to run.

We also have an experimental `flatpak` option. This is a project built on top of CMake for packaging x86 Linux builds for rapid deployment on the Steam Deck. 


## SDL Support

This version of the application creator is built from the following packages:

- SDL2, version 2.28.0
- SDL2-Image, version 2.6.3
- SDL2-TTF, version 2.20.1
- SDL2-ATK, version 2.1.0

For SDL2-Image we enable JPEG, PNG, TIFF and WEBP formats. These are the only formats we can build without error on all platforms. For SDL2-TTF, we enable both FreeType and Harfbuzz.

Other SDL2 extensions are not supported, as our course at Cornell prefers to use alternate packages for audio and networking.
