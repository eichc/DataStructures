HOMEWORK 7: SUPER SMASH BROS. FRAMES


NAME:  Cam Eich


COLLABORATORS AND OTHER RESOURCES:

Submitty discussion forum, Lecture Notes, C++ reference databases



ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  6



ORDER NOTATION ANALYSIS:
Consider the following variables and give the complexity for both running
time AND space along with a short justification for each of the queries below.
For the commands, do not count the space occupied by the database in the space
complexity analysis.

f: The total number of fighters
m: The number of different moves
t: The highest number of moves that have the same starting frame for a particular move

reading the database/populating your initial data structure: Time is O(f*m*(f log f)) because the while loop
runs f*m times, and the if statement that contains insert runs f times. Space is O(f) because a map is created
with f keys. **I am assuming a map takes O(keys) space.**

-q (specific move): Time is O(log f + log m) because the [] operator on the fighters map takes log f, and
get_move takes log m because it uses find(). Space is O(1) because the only thing that gets created is a
single iterator.

-q (all): Time is O(m*(log f + log m)) because it calls -q for a specific move m times. Space is O(m) because
it creates a map with m keys.

-f: Time is O(f log f) because it calls insert for every fighter. Space is O(f) because it creates a set with
every fighter.

-s: Time is O(f log f) and space is O(f) because -s and -f call the same function.

-d: Time is O(f*(log m + t log f) because the fighters map is looped over and inside of that get_move is
called every time, while insert is called t times. Space is O(t) because a set is created that contains t
fighters.


MISC. COMMENTS TO GRADER:  
(optional, please be concise!)
