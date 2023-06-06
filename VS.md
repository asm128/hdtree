# How to set up a new C++ project in `Visual Studio 2022`

1. Download and execute the [Visual Studio 2022 Installer](https://visualstudio.microsoft.com/vs/community/) from the Microsoft website. Then select Visual Studio Community 2022 and click on the `Install` button. If you already installed Visual Studio 2022, you can move on to `step 4`.

2. You will be asked to select the packages you want to install. This tutorial is for C++ but you can select other packages if you like. The basic C++ package you should install is the one called `Desktop development with C++` (which is not necessarily for desktop only as I use it to develop for other platforms too).

- 2.1 I recommend to also go to the `Individual components` tab and select the `Git extension` or `Git for Windows` that comes with it (until VS2019 you had both).

3. When opening VS `for the first time`, a menu will let you choose `Visual C++` as your `default setting`. *

4. A `folder` will be needed where your development solutions will be placed. I called mine just `solutions`.

5. You can then create a new solution and project as follows:

- 5.1 Open Visual Studio 2022 and click on `Create a new project`:

- 5.2 Select `Windows Desktop Wizard` as project template, then click on `Next`. 

- 5.3 Then type the Project name (such as `MyFirstProject`) and the name of the folder in the `Location` box, then click on `Create`. 

- 5.4 In the window that pops-up next, select `Console Application` as the `Application type` and check the `Empty Project` checkbox.

- 5.5 Click on `OK` and the project and project files will be created automatically as shown below.

6. After setting up the project, you will be able to add files to it from the samples you find here or browsing the web. If you want to `test` some code you can write a `hello world example` as follows:

- 6.1 Right click on the `project name` in the `Solution Explorer` in `Visual Studio` (at the right or left of the edit area) and in the pop-up menu select `Add` -> `New item`

- 6.2 Select `Visual C++` -> `C++ file (.cpp)` and type the name of the file, such as `main.cpp` at the name box. Then Click on `Add` to create the file.

- 6.3 Type or copy the following code into the `main.cpp` file:

```
#include <stdio.h>

int main()
{
    printf("Hello world!\n");
    return 0;
}
```

- 6.4 Press `F7` (or `ctrl`+`shift`+`b` if you didn't set up Visual C++ settings) to build the executable and `F5` to execute in debug mode. 

### If you didn't set up Visual C++ settings during install you can do it later by going to menu `Tools` -> `Import and export settings` -> `Reset all settings` and the option will appear again. The screenshot shown is from that window because I had no fresh install of Visual Studio to see the window appear during the first run. 