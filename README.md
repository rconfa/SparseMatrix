# Sparse Matrix

Project's specifications are taken from the 2020 C++ exam of the course "Programming C++" and can be found in the file assignment.pdf in italian languages.

A sparse matrix is a matrix in which only the elements explicitly inserted are physically stored, all other elements have the default value.
I choose to save in the matrix also elements equal to the default value if they are explicitly inserted! 
I use a ordered linked-list to represent the sparse matrix efficiently. Each new element is inserted in order according to its coordinates.

## SparseMatrix.h

Is a template class that implement the sparse matrix.
T: type of the values stored in the matrix.

### Variable type implemented

value_type: value of type T, is the value of a single cell of the matrix.
sm_size: coordinate value for the matrix.

### Public method

TODO 

## Main.cpp

Contains examples of class use. I used this file as a test file for the class.
