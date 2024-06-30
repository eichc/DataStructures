HOMEWORK 6: INVERSE WORD SEARCH


NAME:  Cam Eich


COLLABORATORS AND OTHER RESOURCES: 

Submitty discussion forum, Lecture Notes, C++ reference databases



ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  15


ALGORITHM ANALYSIS:
What's the order notation of your algorithm?
The worst case time complexity of my algorithm is O(w^2 * h^2 * r * f). This is because 
find_boards runs w*h*r times, which must be multiplied by the complexity of find_forbidden because 
find_forbidden is called by find_boards. The complexity of find_forbidden is O(f*w*h + l) because 
it calls find_word f times. The complexity of find_word is O(w*h + l). Other functions such as 
read_words or print_board don't affect the complexity of the algorithm because they are 
insignificant compared to find_boards.


TEST CASE SUMMARY:
How did your program perform on the different test cases?  Summarize
the running times.  (It's ok if it didn't finish the harder examples.)
What new test cases did you create and how did it perform on those
tests?
Test #		Success?									Running Time (all solutions)
Test 1 		Yes 										0.019
Test 2 		Yes 										0.015
Test 3 		Yes 										0.049
Test 4 		Yes 										0.025
Test 5 		Yes 										0.107
Test 6 		Mostly - 216 solutions found instead of 208 0.214
Test 7 		No 											N/A
Test 8 		Yes 										13.44
Custom 1 	Yes											0.013
Custom 2 	Yes											0.015
Custom 3 	Yes 										0.041

My first 2 test check edge cases, and my third test is a random example that is different from the given test cases.


MISC. COMMENTS TO GRADER:  
Optional, please be concise!


