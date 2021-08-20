# Border-Security-System
A GEOJSON parsing C++ program that helps calculate border perimeters and cable lengths measured between the main controller in the capital and the closest border, using geometrical data stored in GEOJSON files for capitals and countries.


In an imaginary world, every country wants to deploy a border security system (BSS). Main controller of BSS will be deployed in capital city of the country. A sensing cable will go straight to nearest border. The sensing cable will then installed along border surrounding country and come back to capital. One file contains borders of countries as a polygon in geojson format (countries.geojson) and one contains the coordinates of the capital cities of countries (capitals.geojson).
The program then calculates border of countries and the length of the sensing cable in JSON format.

The application titled "bss" is written in C++ using MS Visual Studio and run the terminal in Linux OS. Third party libraries "lib_geojson" and "gason" are used and edited as per need, as well as Makefile based in Linux which helps in running and executing the program by helping in establishing a working dependency chain. Command arguments -c and -t are used with iso_a3 codes, parsed through the command terminal to the program to run with all files, libraries and third party tools stored in the same directory.

### Compilation & Running of Program
With the help of the "gason" libraries, GEOJSON file contents are parsed. Some modifications are made to the header and source files for "lib_geojson" for our specific project which help realize the file contents as JSON objects and makes it easy to access their elements. Functions implemented in C++ are called accordinly for distance calculation, perimeter calculation and sensing cable length calculation. 
Makefile works as the main compiler and is put to use by running "make" in the terminal after loading the source directory. Then the appropriate command is given but it is important to note that "./bss" would be used instead of "bss", and " | jq" can be added after the command to format and indent the output to make it look nicer.
This Linux based execution can also be run on Windows using various emulators and virtual machines e.g. VMWare. The steps to run a sample of the program would be:
  1. Open the terminal in Linux
  2. Load the source directory with the libraries, .geojson files and source code files
  3. Run "make" in the terminal to compile the project
  4. Write the appropriate command and obtain the results.
An example is:
The source directory is opened in the terminal, "make" is run to compile the project then "./bss -c AZE -t border" is entered to the command terminal to find the border length of Azerbaijan or "./bss -c BGR -t cable" is sent to the command terminal to find the sensing cable length for Bulgaria or just "./bss" is sent to the command terminal to find the border lengths of all countries. The outputs are obtained in JSON formats as follows respectively: 

Border length for Azerbaijan:
[
  {
    "name": "Azerbaijan",
    "iso_a3": "AZE",
    "border": 2013.0
  }
]


Cable length for Bulgaria:
[
  {
    "name": "Bulgaria",
    "iso_a3": "BGR",
    "cable": 2050.6
  }
]


All border lengths:
[
  {
    "name": "Aruba",
    "iso_a3": "ABW",
    "cable": 500.2
  },
  {
  "name": "Afghanistan",
  "iso_a3": "AFG",
  "cable": 5529.0
  },{
  "name": "Angola",
  "iso_a3": "AGO",
  "cable": 5198.0
  },
  ...
]
