# Sparse Matrix

>Project's specifications are taken from the 2020 C++ exam of the course "Programming C++" and can be found in the file assignment.pdf in italian languages.

A sparse matrix is a matrix in which only the elements explicitly inserted are physically stored, all other elements have the default value.
I choose to save in the matrix also elements equal to the default value if they are explicitly inserted! 
I use a ordered linked-list to represent the sparse matrix efficiently. Each new element is inserted in order according to its coordinates.

## SparseMatrix.h

Is a template class that implement the sparse matrix.
T: type of the values stored in the matrix.

**Variable type implemented**

value_type: value of type T, is the value of a single cell of the matrix.

sm_size: coordinate value for the matrix.

**Class member**

Struct element: implemented an element of the matrix. Each element contains the value (value_type) and const coordinates (i,j).


**Public method**

```c++
SparseMatrix(const sm_size r,const sm_size c, const value_type &dv): Initialize a sparse matrix with r number of rows, c number of columns and dv default value

SparseMatrix& operator=(const SparseMatrix &other): redefinition of operator=

SparseMatrix(const SparseMatrix &other): copy constructor

template <typename Q>
SparseMatrix(const SparseMatrix<Q> &other): copy constructor with other sparse matrix of type Q. Leave the conversion Q->T to the compiler
  
add(const sm_size ii, const sm_size jj, const value_type& value): add an element at (ii, jj) coordinates with the specifed value.
    
const value_type& operator()(const sm_size ii,const sm_size jj) const: *redefinition of operator(). Return constant value of the element at (ii,jj) coordinates.
    
sm_size getNumRows() const: return the number of rows.
    
sm_size getNumCols() const: return the number of columns.
    
sm_size getNumElement() const: return the number of element explicit inserted.
    
sm_size getDefaultValue() const: return the default value.

```

**Iterator**

I have implemented forward iterator for reading (const_iterator) and writing (iterator).

```c++
iterator begin(): return the iterator at the start of the matrix

iterator end(): return the iterator at the end of the matrix

const_iterator begin() const: *return the const_iterator at the start of the matrix

const_iterator end() const: return the const_iterator at the end of the matrix
```
## Main.cpp

Contains examples of class use. I used this file as a test file for the class.
