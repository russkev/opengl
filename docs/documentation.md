This project uses [qt style](https://doc.qt.io/qt-5/qtwritingstyle-cpp.html) documentation.
# Documentation

## Steps to manually make GL Engine documentation
 - To start fresh, first delete the `_build` directory
 - Load Msys and navigate to the `docs` folder

 - run `doxygen Doxyfile.in`
	- Whis will generate the doxygen documentation
 - run `make html`
	- This will make the Sphinx documantation 

## Tutorial
Making documentation for a C++ project that works on `readthedocs.org` is a little bit complicated. It requires Doxygen, Breathe and Sphinx to work together. I'm going to outline the steps that worked for me here.

- Install Doxygen
  - I found the easiest way was to install it with NuGet package manager and then add the directory where it was installed to the Windows `Path` environment variables.
- Install [MinGW and MSYS](https://genome.sph.umich.edu/wiki/Installing_MinGW_%26_MSYS_on_Windows). This will give you access to the `make` command.
- From here, you can follow [this tutorial](https://medium.com/practical-coding/c-documentation-with-doxygen-cmake-sphinx-breathe-for-those-of-use-who-are-totally-lost-part-2-21f4fb1abd9f)
	- You only need part 2 and you don't need anything `CMake` related.
- Add `subprocess.call('doxygen Doxyfile.in', shell=True)` near the top of the `conf.py` file to ensure that Doxygen and Sphinx both run when `make html` is called.
- Make a [requirements.txt](https://pip.pypa.io/en/stable/user_guide/) file for your pip packages and put it in your `docs` folder.
- Make a `readthedocs.yml` file and put it in your root directory. This will tell `readthedocs.org` what to install in order to display your documentation.
- Use the following settings
```yaml
# Required
version: 2

# Build documentation in the docs/ directory with Sphinx
sphinx:
  builder: html
  configuration: docs/conf.py


python:
    version: 3.7
    install:
      - requirements: docs/requirements.txt
    system_packages: true
```
  - Go to the [Read the Docs website](https://readthedocs.org/) and set up an account.
	- If everything goes well, your documentation should build and display succesfully.

