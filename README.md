# hdtree
`Windows` application that allows the creation of a tree containing categories as first-level items and materials contained by these categories.
This repository provides fully-working project files for building the code with [RAD Studio 11](https://www.embarcadero.com/products/cbuilder) and [Visual Studio 2022](https://visualstudio.microsoft.com/vs/community/) using the latest [ISO C++ Standard](https://www.google.com/search?q=iso+C%2B%2B) available.

Instructions:
- Install [Visual Studio 2022](https://visualstudio.microsoft.com/vs/community/) with the C++ packages and/or [RAD Studio 11](https://www.embarcadero.com/products/cbuilder). I've written [a doc for installing Visual Studio in case you need it](https://github.com/asm128/hdtree/blob/master/VS.md)
- Open [hdtree.sln](./hdtree.sln) and/or [hdtreeappGroup.groupproj](./hdtreeappGroup.groupproj), respectively.
- Build everythig and run.
- The resulting files can be found in the folder that contains this repository, inside a folder called [Win32.Debug](../Win32.Debug/), [Win32.Embarcadero.Debug](../Win32.Embarcadero.Debug/), [Win32.Release](../Win32.Release/), [Win32.Embarcadero.Release](../Win32.Embarcadero.Release/),  [x64.Debug](../x64.Debug/), [x64.Embarcadero.Debug](../x64.Embarcadero.Debug/), [x64.Release](../x64.Release/) or [x64.Embarcadero.Release](../x64.Embarcadero.Release/), depending on the IDE and configuration chosen for building the project. In there you fill find the `hdtreeapp.exe` file that you can execute to run the program.
- You still need to copy the .bmp files manually to the folder where the .exe is located, except when running the program from Visual Studio.

Notes:
- The categories display a custom icon depending on whether they are empty or are expanded or collapsed. The materials just have a single icon.
- The application provides a button for clearing the tree.
- The application provides buttons for adding categories and materials. The buttons enable a text input field which creates the category or material on pressing the `enter` key.
- The contents of the window adjust their size and position if the window is resized.

Architecture overview:
- The [hdtree folder](https://github.com/asm128/hdtree/tree/master/hdtree) contains a static library that defines the business logic. It contains code that should build for every platform.
- The [hdtreeapp folder](https://github.com/asm128/hdtree/tree/master/hdtreeapp) contains the application code that defines the graphical user interface. It provides a system-specific interface to the business logic provided by htree. 
- The [hdtreeapp_win32.cpp file](https://github.com/asm128/hdtree/blob/master/hdtreeapp/hdtreeapp_win32.cpp) is the only platform-specific code file from this repository. Everything else should build smoothly for other platforms.

