# hdtree
WinAPI application that allows the creation of a tree containing categories as first-level items and materials contained by these categories.
This repository provides fully-working project files for building the code with [RAD Studio 11](https://www.embarcadero.com/products/cbuilder) and [Visual Studio 2022](https://visualstudio.microsoft.com/vs/community/) using the latest [ISO C++ Standard](https://www.google.com/search?q=iso+C%2B%2B) available.

Instructions:
- Install [Visual Studio 2022](https://visualstudio.microsoft.com/vs/community/) with the C++ packages and/or [RAD Studio 11](https://www.embarcadero.com/products/cbuilder).
- Open [hdtree.sln](./hdtree.sln) and/or [hdtreeappGroup.groupproj](./hdtreeappGroup.groupproj), respectively.
- Build everythig and run.
- You still need to copy the .bmp files manually to the folder where the .exe is located, except when running the program from Visual Studio.

Notes:
- The categories display a custom icon depending on whether they are empty or are expanded or collapsed. The materials just have a single icon.
- The application provides a button for clearing the tree.
- The application provides buttons for adding categories and materials. The buttons enable a text input field which creates the category or material on pressing the `enter` key.
- The contents of the window adjust their size and position if the window is resized.

Architecture overview:
- The [hdtree folder](https://github.com/asm128/hdtree/tree/master/hdtree) contains a static library that defines the business logic. It contains code that should build for every platform.
- The [hdtreeapp folder](https://github.com/asm128/hdtree/tree/master/hdtreeapp) contains the application code that defines the graphical user interface. It provides a system-specific interface to the business logic provided by htree. 
- The [hdtreeapp.cpp file](https://github.com/asm128/hdtree/blob/master/hdtreeapp/hdtreeapp.cpp) is the only platform-specific code file from this repository. Everything else should build smoothly for other platforms.

