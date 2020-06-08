Overview

This is a C program which simulates a given logic circuit in order to determine whether it is stable, and if so to output a logic table for the circuit. A CNF statement will also be output to describe the circuit output if appropriate.

Input

The input to this program should be a simple text file. e.g. a NAND latch would be represented by:

a IN 
b IN 
out NAND a c 
c NAND b out

The output value must always be called 'out'. Supported operators are: IN, NOT, AND, OR, NOR, NAND, XOR, EQ. IN is used to show an input value, EQ is used as double implication, all other operators are as expected.
