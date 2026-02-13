#!/bin/bash
set -e # Exit with nonzero exit code if anything fails

# Build sphinx
cd Docs/sphinx_documentation
echo "Build the Sphinx documentation for Amrex."
make PYTHON="python3" latexpdf
mv build/latex/amrex.pdf source/
make clean
make SPHINXOPTS='-v -W --keep-going' PYTHON="python3" html
cd ../../

# copy sphinx to target location
cd build
cp -rp ../Docs/sphinx_documentation/build/html/* docs_html/
