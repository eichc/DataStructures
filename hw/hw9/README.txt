HOMEWORK 9: MINIBLAST


NAME:  Cam Eich


COLLABORATORS AND OTHER RESOURCES:

Submitty discussion forum, Lecture Notes, C++ reference databases



ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  10


HASH FUNCTION DESCRIPTION

I created my own hash function, rather than taking one from our notes or online. My hash function uses the
ascii value of each letter in the string and multiplies it by the letter's position in the string, then adds
that to the total. This way, strings that are anagrams of each other are less likely to end up with the same
hash value.



HASH TABLE IMPLEMENTATION

My hash table uses a vector at the top level. The elements of the vector are of a custom class called
TableEntry. TableEntry is basically a very simple pair, with two member variables: a string key and a vector
of ints. The kmer is the key, and the vector contains each location of the kmer in the genome. I use linear
probing to deal with collisions. In order to re-size the hashtable, I create a new table with double the
previous size, and copy all of the old elements into the new table (while also re-hashing them).



ANALYSIS OF PERFORMANCE OF YOUR ALGORITHM:

L = length of the genome sequence
q - query length
p - number of different locations where key is found
k - key size (k-mer size)

How much memory will the hash table data structure require (order notation for memory use)?

Since it's very difficult to know the exact size of the overlying vector in the hash table, I won't include
the entries in the vector which are empty in the order notation. Therefore, the hash table takes up O(L-k)
space, because the total number of strings, or kmers, added to the hash table is L-k+1, and the +1 isn't
included in big O.



What is the order notation for performance (running time) of each of
the commands? What about for the hash table's insert, find, and resize?

Commands: O(p*q), because the for loop runs p times. Inside the for loop, the while loop that compares the 
query to the genome runs q times. Find is called outside the foor loop which is O(k+n), but k+n will always be 
smaller than q so it doesn't appear in the final big O.

Insert: O(k+n), because hash() is constant in relation to the size of the hash table but depends on the key 
size, so it takes O(k) time. That gets added to n because the linear probing takes O(n) time, where n 
represents the number of kmers that have the same hash value. Ideally n will be negligible, but this is not 
always the case. The rest of insert is constant, as long as the vector doesn't need to be resized.

Find: O(k+n), because it is implemented nearly identically to insert.

Resize: O(s), where s is the size of the hash table, because creating the new vector takes O(2s), and the rest
of the function will not be greater than O(2s). O(2s) simplifies to O(s).



EXTRA CREDIT
Add a new command to implement the database using one of the other
 data structures that we have covered so far in the course: 
vectors, lists, arrays etc. 

Compare the performance your alternative method to the homework method
by making a table of run times for each of the genomes and query 
sets provided with the homework  and compare the times to build
the index and the times to process the queries. 
Document any new commands you have added in your README file.



MISC. COMMENTS TO GRADER:  
(optional, please be concise!)


