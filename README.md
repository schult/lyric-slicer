# Lyric Slicer

This is a tool for generating simple text-only slides in the PNG image format.
I created it to simplify the inclusion of custom lyrics and scripture readings
in [iSingWorship](https://www.isingworship.com/).

## Usage

 - Text entered into the "Lyrics" field will be placed on slides.
 - Use blank lines to separate slides.
 - Use the "Export" button or menu item to create PNG files of the slides.
 - The "Title" field is used to generate exported file names. If left empty,
   the first line of the lyrics will be used.
 - "Slide group size" affects slide numbers in the exported file names.

## Building

This project depends on [Qt 6](https://www.qt.io/download-open-source).

    mkdir build
    cd build
    qmake ..
    make

## Licenses

Lyric Slicer is released under the [MIT License](LICENSE.txt).

[Fluent UI System Icons](https://github.com/microsoft/fluentui-system-icons)
are used under the [MIT License](icons/LICENSE.txt).

[Linux Biolinum](https://sourceforge.net/projects/linuxlibertine/) is used
under the [SIL OFL Version 1.1](font/LICENSE.txt).

[Qt](https://www.qt.io/download-open-source) is used under the
[LGPL Version 3](qt/LICENSE.txt).
