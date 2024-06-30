HOMEWORK 3: MATRIX CLASS


NAME:  Cam Eich


COLLABORATORS AND OTHER RESOURCES:

Submitty discussion forum, Lecture Notes, C++ reference databases (cplusplus.com,
cppreference.com)


ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  11



ORDER NOTATION:
For each of the functions below, write the order notation O().
Write each answer in terms of m = the number of rows and n = the
number of columns.  You should assume that calling new [] or delete []
on an array will take time proportional to the number of elements in
the array.

get O(1)

set O(1)

num_rows O(1)

get_column O(m^2)

operator<< O(m*n)

quarter O(m*n)

operator== O(m*n)

operator!= O(m*n)

swap_rows O(1)

rref (provided in matrix_main.cpp) O(m^2*n)



TESTING & DEBUGGING STRATEGY: 

While I was writing the Matrix class, I used a separate main file for testing. Whenever I wrote
a function or a section of code that made a significant change to the matrix, I used both
Valgrind and std::cout to test it. I used Valgrind to check for memory bugs, and std::cout to
check the code's actual functionality. I tested corner cases by creating new tests that were
specific to whichever function I was testing. After I finished writing the Matrix class, I
wrote tests in matrix_main.cpp for cases that weren't covered in the provided tests. I used
assert rather than cout for most of these to avoid a lengthy output.


EXTRA CREDIT: 
I implemented resize and included test cases in matrix_main. The order notation is O(m*n), 
where m and n are the new number of rows and columns.



MISC. COMMENTS TO GRADER:  
(optional, please be concise!)

