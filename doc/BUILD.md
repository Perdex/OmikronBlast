# Building

Building our software is a simple process.

1. Goto either TFClient or TFServer folder.
2. Run `qmake`.
3. Open `Makefile` with a text editor, go to line that starts `GXXFLAGS` and add flag `-std=gnu++11` to end of line
4. Run `make`.

Now you have finished building either client or 
server depending on which folder you chose. To build
the other one, repeat the process, but choose the other folder.
