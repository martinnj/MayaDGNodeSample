Maya Dependency Graph Node Sample
======

This plugin will register a DG Node with Maya that is able to calculate sine
based in its input plug, and report in the output plug.
Node implements clean/dirty marking correctly.
Note that this sample do not include a plugin that uses the node or adds it
to the DG, this just registers a new node for use.

Warning: The ID for the node is 0x80000 and is NOT unique! registering it with
a Maya installation that have other custom nodes can create problems if they
share the ID!

Building
=====
The CMake file works for standard Windows installations, should you have non
standard paths or be workoing on Linux/OS X, please follow below steps:
1. In CMakeLists.txt change the Maya path to whereever you'r Maya is installed.
2. Change the extension from .mll to .so for linux, and to... Whatever
   OS X uses ;)

Remember that CMake does not compile the node, it creates a project/makefile
that can compile and link it.