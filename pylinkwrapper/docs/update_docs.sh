#!/usr/bin/env bash

# Make CSS work
touch ~/Code/psychopy_modules/pylinkwrapper/docs/_build/html/.nojekyll

# Automate pushing to gh-pages branch
cd ~/Code/psychopy_modules/pylinkwrapper/
git add docs/_build/html/.nojekyll
git commit -m "stop jekyll processing of docs"
git subtree push --prefix docs/_build/html/ origin gh-pages
