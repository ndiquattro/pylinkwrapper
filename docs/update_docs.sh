#!/usr/bin/env bash

# Make CSS work
touch ~/Code/psychopy_modules/pylinkwrapper/docs/_build/html/.nojekyll

# Automate pushing to gh-pages branch
cd ~/Code/psychopy_modules/pylinkwrapper/
git subtree push --prefix docs/_build/html/ origin gh-pages
