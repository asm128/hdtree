# hdtree
WinAPI application that allows the creation of a tree containing categories as first-level items and materials contained by these categories.
This repository provides fully-working project files for building the code with RAD Studio and Visual Studio 2022 using the latest C++ standard available.

Instructions:
- Install Visual Studio 2022 and/or RAD Studio 11.
- Open hdtree.sln and/or hdtreeappGroup.groupproj, respectively.
- Build everythig and run.
- You still need to copy the .bmp files manually to the folder where the .exe is located, except when running the program from Visual Studio.

Notes:
- The categories display a custom icon depending on whether they are empty or are expanded or collapsed. The materials just have a single icon.
- The application provides a button for clearing the tree.
- The application provides buttons for adding categories and materials. The button enables a text input field which creates the category or material on pressing the `enter` key.
- The contents of the window adjust their size and position if the window is resized.


