Installation
============

Requirements
------------

  - Windows
  - Visual Studio 2019
  - Graphics card that supports at least OpenGL 4.4

Recommended Components
----------------------

  - `NVidia NSight Visual Studio Edition <https://developer.nvidia.com/nsight-visual-studio-edition)>`_ and `Nvidia NSight Graphics <https://developer.nvidia.com/nsight-graphics>`_

    - Great for debugging everything that gets sent to the graphics card. 
    - If you don't have a Nvidia graphics card, you can also use `RenderDoc <https://renderdoc.org/>`_
  - `GLSL language integration <https://marketplace.visualstudio.com/items?itemName=DanielScherzer.GLSL>`_

    - Makes GLSL shader writing a whole lot easier

Getting Started
---------------

To get going, simply clone the repository and open ``src/gl_engine.sln``.

Ensure that the ``gl_demo`` is the startup project (it should be **bold** in the ``Solution Explorer``).
If not, right click on it inside the `Solution Explorer` and click `Set as Startup Project`.

If you have Visual Studio set up to automatically download NuGet packages, press `F5`
to build and run the solution. The required packages should automatically download. 

If they don't, click ``Tools`` -> ``NuGet Package Manager`` -> ``Manage NuGet Packages for Solution...`` 
You should get a warning saying that some packages are missing. Click ``Restore``.