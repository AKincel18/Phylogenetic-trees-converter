# Phylogenetic-trees-converter
## Description
The aim of the project is to write a program in C++ using the Boost Spirit library, which converts phylogenetic trees from the newick to phyloxml format in both directions.
## User manual
Program can be started by downloading the file **app** and run the g++ command:

`./app -i input_file -o output_file`

It is not required to specify which side the conversion will take place, because the first attempt is to convert: newick -> phyloxml, and after an unsuccessful attempt: phyloxml -> newick. The application is protected against errors, which results in displaying an appropriate message.

Messages:
* Error! Could not open input file: file_name.
* Error! Wrong number of arguments.
* Error! Failed to parse input file.
* Error! Failed to generate output file.

## Examples
Examples of the program's work are presented in the [*Examples*](https://github.com/AKincel18/Phylogenetic-trees-converter/tree/main/Examples) folder.