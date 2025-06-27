# curry
A single-header terminal library in C99.

## About
curry is a single-header, dependency-free library for creating terminal applications in C99. It provides a simple API for handling input, output, and terminal control.

## Features
- Written in C99
- No dependencies
- Easy to use: simply clone `curry.h` into your project.
- Supports basic terminal operations like printing text, reading input, and clearing the screen


## Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/dn-blip/curry.git

2. Drop it into your project:
   - Copy `curry.h` to your project's directory.
   - Include it in your source files with `#include "curry.h"`.

## Usage
Compile like normal C code. Note that you need to link against `<termios.h>` on Linux/MacOS/BSD, and kernel32.lib on Windows. 