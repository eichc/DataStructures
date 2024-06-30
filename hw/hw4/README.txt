HOMEWORK 4: TOOL RENTAL LISTS


NAME:  Cam Eich


COLLABORATORS AND OTHER RESOURCES:

Submitty discussion forum, Lecture Notes, C++ reference databases


ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  12



ORDER NOTATION:
Use these variables to analyze each phase of computation.  Briefly
justify your answer for each part.


i = # of different inventory items
c = # of different customers
p = # of pending customers
q = max quantity of a particular tool owned by the store
r = max # of tools rented or requested by a customer at one time
p = total # of pending tool rentals
e = # of rental or return events


Load inventory file: O(i^2) because the while loop runs i times, and the inner for loop also
runs i times.

Rental event: O(e*(i+c)) because the while loop of read_customers runs e times. Inside the
while loop, the inventory list loop is O(i), add_customer is O(c), and the worst case of
rent_tool or add_to_waitlist is O(i+c). Those three events simplify to O(i+c).

Return event - no pending customer(s): O(e * (i + (c*r))) because the while loop of
read_customers runs e times. Inside the while loop, there are two inventory list loops which
simplify to O(i), and return_tool is O(c*r) if r is the number of different tools the customer
has rented.

Return event - with pending customer(s): O(e * ((c*r) + p*(i+c))) because it is the same as the
above scenario, but it also loops over the waitlist which is O(p). Inside of the waitlist loop,
rent_tool i O(i+c) and remove_from_pending is O(c).

Entire program: Printing the tools is O(i*c*r), where r is the number of tools rented/requested
by each customer. Printing the customers is O(c*r). When these are added to the other events,
the result is O(i*c*r) + (i^2) + (e*((c*r) + p*(i+c)))



MISC. COMMENTS TO GRADER:
(optional, please be concise!)

