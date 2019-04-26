# Encoding Bayesian Network to Weighted Model Counting

## COMPILATION

A simple Makefile is provided for compilation, which can be
compiled by simply running the "make" command.


## CLI usage
```
./bayes2wmc [-h] <input> <evid> [<type>]
Example: ./bayes2wmc input.uai input.uai.evid 1
```
There are two types of encoding (0 and 1)

Result will be store in "cnf.txt" and "weight.txt"
