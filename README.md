This project implements a template class - a matrix. 
Memory management occurs in accordance with the rule of five. 
This class is also designed with strict exception safety guarantees. 
The concept of a two-tier container is used for this. 
For a matrix, operators for arithmetic operations are overloaded, and access to a matrix element is implemented through double square brackets. 
The project implements several methods for working with matrices, the main one being the calculation of the determinant. 
To test this method, random matrices of size 100 by 100 are generated; there are separate tests for integers and rational numbers. 
There are also tests for copying matrices and for exception situations.
