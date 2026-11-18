# Crossword Generator
## What it does
This program generates a word grid of a given length and width for a crossword while trying to maximize intersections between words.  
## Usage
### C version
To use run:

crossword_generator.exe \<filepath> \<width> \<height>

where:

- \<filepath> is the path of a text file containing **words separated by a coma, with no spaces, and all in lowercase**.

- \<width> is the maximum width of your grid

- \<height> is the maximum height of your grid

width and height both default to 15 and can be ommited, but filepath is mandatory.

### Python version
**The python version is only a prototype. It is much slower and is not getting updated.**<br>
To use run: python generate.py <br>
Height, width and the word list are all set in the file itself, so to change those edit them in the file.