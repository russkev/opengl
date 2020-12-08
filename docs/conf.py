# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
# import os
# import sys
# sys.path.insert(0, os.path.abspath('.'))
from sphinx.builders.html import StandaloneHTMLBuilder
import subprocess, os


read_the_docs_build = os.environ.get('READTHEDOCS', None) == 'True'
if read_the_docs_build:
    subprocess.call('doxygen Doxyfile.in', shell=True)

# subprocess.call('doxygen Doxyfile.in', shell=True)

# -- Git information -----------------------------------------------------
def is_git_installed():
    return command_is_valid('git --version')

def is_git_repository():
    return command_is_valid('git rev-parse --is-inside-work-tree')

def get_git_version():
    command = ''
    if is_git_installed() and is_git_repository():
        version = command_output('git describe --tags --always --abbrev=0')
        if version:
            command = version.decode("utf-8") 
    return command

def command_output(command):
    command_output = None
    try:
        command_output = subprocess.check_output(command.split())
    except OSError:
        pass
    return command_output

def command_is_valid(command):
    try:
        command_return = subprocess.check_call( 
            # Ensure space seperated arguments are separate elements of an array 
            # (required for subprocess calls)
            command.split(),
            # Ensure error output doesn't get printed to terminal                 
            stdout = open(os.devnull, 'wb'))    

        return command_return == 0
    except (subprocess.CalledProcessError, OSError):
        pass

    return False

# -- Project information -----------------------------------------------------

version = get_git_version()

project = 'GL Engine'
copyright = '2020, Kevin Russell'
author = 'Kevin Russell'
release = version


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    'sphinx.ext.autodoc',
    'sphinx.ext.intersphinx',
    'sphinx.ext.autosectionlabel',
    'sphinx.ext.todo',
    'sphinx.ext.coverage',
    'sphinx.ext.mathjax',
    'sphinx.ext.ifconfig',
    'sphinx.ext.viewcode',
    'sphinx_sitemap',
    'sphinx.ext.inheritance_diagram',
    'sphinx_markdown_tables',
    'breathe',
    'recommonmark'
]

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store', '#include']

highlight_language = 'c++'

source_suffix = {
    '.rst': 'restructuredtext',
    '.md': 'markdown',
}

surpress_warnings = ['toc.circular']

# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = 'sphinx_rtd_theme'
html_theme_options = {
    'canonical_url': '',
    'analytics_id': '',
    'display_version': True,
    'prev_next_buttons_location': 'bottom',
    'style_external_links': False,
    
    'logo_only': False,

    # Toc options
    'collapse_navigation': True,
    'sticky_navigation': True,
    'navigation_depth': 4,
    'includehidden': False,
    'titles_only': False
}
# html_logo = ''
# github_url = ''
# html_baseurl = ''

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']


# -- Breathe configuration -------------------------------------------------

breathe_projects = {
	"GL Engine": "_build/xml/"
}
breathe_default_project = "GL Engine"
breathe_default_members = ('members', 'undoc-members')