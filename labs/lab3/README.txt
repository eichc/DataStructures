1. Commmand line to compile into executable:
g++ roads.cpp line.cpp point.cpp -o lab3.exe -Wall -Wextra -g

2. Added #include<algorithm> to roads.cpp so that std::sort() can be used

3. Removed #include "point.h" from line.cpp because it was already included in line.h

4. Executable successfully created

5. A segmentation fault occurs with each data set

6. Errors observed from compilation warnings:
	- the function load() returns a reference to a local variable, which means it is out of scope
	- logic error: comparing an unsigned int (which is always positive) to 0
	- math error: possibly used * instead of *= when computing gradient
	- when comparing two gradients in steeper_gradient(), if they are equal then nothing gets returned
	- when computing slope, "answer" is computed but then never used