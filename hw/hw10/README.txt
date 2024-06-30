HOMEWORK 9: DISTANCE FIELDS & FAST MARCHING METHOD


NAME: Cam Eich


COLLABORATORS AND OTHER RESOURCES:

Submitty discussion forum, Lecture Notes, C++ reference databases



ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  10



NAIVE ALGORITHM:

Order Notation: O(w^2 * h^2)

Timing Experiment Data: (using "user" time)
small_10x10: 0.004s
squiggle_30x30: 0.008s
lines_100x100: 0.777s
data_structures_300x300: 1m 5.791s

Discussion: The actual running time seems to match my prediction. As the image size increases, the running 
time appears to increase exponentially. If the running time was linear, then the largest file I tested should 
have taken less than a second, not over one minute.



IMPROVED ALGORITHM:

Order Notation: O(w*h*p)

Timing Experiment Data: (using "user" time)
small_10x10: 0.005s
squiggle_30x30: 0.000s
lines_100x100: 0.051s
data_structures_300x300: 6.840s

Discussion: The actual running time matches my prediction fairly well. The larger data sets are significantly 
faster for this method than the naive method, because for large images p will almost always be much smaller 
than w*h. The smallest data sets weren't always faster, but this is likely in part due to my computer's 
limitations, not the actual algorithm.



FAST MARCHING METHOD (with a map):

Order Notation: O(w*h*log(n)), where n represents the number of elements in the priority queue at any given 
time. Every pixel is looped over (aside from the black pixels), and inside of the loop it takes log(n) to add
a pixel to the queue or update its position.

Timing Experiment Data: (using "user" time)
small_10x10: 0.003s
squiggle_30x30: 0.002s
lines_100x100: 0.043s
data_structures_300x300: 0.493s

Discussion: For the larger images, this method is significantly faster than the previous two methods. For the
small images there isn't much of a difference, mostly because the time is already so short that it can't be
optimized much further. This aligns with my prediction because even though the time complexities of this
method and the improved method may look similar, log(n) will almost always be smaller than p.



DISTANCE FIELD VISUALIZATIONS FOR EXTRA CREDIT:




FAST MARCHING METHOD (with a hash table) FOR EXTRA CREDIT:

Order Notation:

Timing Experiment Data:

Discussion:



MISC. COMMENTS TO GRADER:  
Optional, please be concise!






