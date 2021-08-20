# Border-Security-System
A GEOJSON parsing C++ program that helps calculate border perimeters and cable lengths measured between the main controller in the capital and the closest border, using geometrical data stored in GEOJSON files for capitals and countries.


In an imaginary world, every country wants to deploy a border security system (BSS). Main controller of BSS will be deployed in capital city of the country. A sensing cable will go straight to nearest border. The sensing cable will then installed along border surrounding country and come back to capital. One file contains borders of countries as a polygon in geojson format (countries.geojson) and one contains the coordinates of the capital cities of countries (capitals.geojson).
The program then calculates border of countries and the length of the sensing cable in JSON format.

The application titled "bss" is written in C++ using MS Visual Studio Editor and run in main console in Linux OS. Third party libraries titled geojson.hh and gason.h are used and edited as per need, as well as Makefile based in Linux which helps in running and executing program builds. Command arguments -c and -t are used with iso_a3 codes, parsed through the command terminal to the program to run with all files, libraries and third party tools stored in the same directory.
An example is:
"bss -c AZE -t border" is entered to the command terminal to find the border length of Azerbaijan, "bss -c BGR -t cable" is sent to the command terminal to find the sensing cable length for Bulgaria and "bss" is sent to the command terminal to find the border lengths of all countries
