Traveling Salesman Solver
===================================================
  
This repo contains a TSP solver made in C starting from a .txt file containing a list of cities.     
My intention is to expand this project with multiple algorithms to solve the traveling salesman problem and add a visualization of the resulting path to have a somewhat comprehensive aggregation of solving methods.  
I'll try my best to make every method indipendent from the others so that it would be easy to experiment with many different ones to see the various results.
  
Be aware that this is a personal project and doesn't have priority over others, this means that it may remain incomplete for a long time or it may never be fully completed at all.

I am by no means and expert in both C and the TSP so if you find any problems and/or want to propose a solution/different ways to do stuff feel free to get in touch with me and/or open an issue here on GitHub

### Algorithms and Features implemented so far:

- **nearest neighbour**
- **two opt**
- **simulated annealing**

How to compile
----
Move to the program directory and run ***cmake CmakeLists.txt***, then run ***make*** and an executable called ***tsp*** should be created in the same directory.  

Alternatively move to the program directory and write ***gcc main.c algorithms.c printer.c -o tsp*** and the executable should be created
 
 How to run
----
To run the program move to the executable's directory and type ./tsp, this program can take 3 arguments, arguments in brackets are optional

- **-f filepath**
- **[-r number]**
- **[-s number]** 

-f is the relative path from the executable to the file containing the list of cities, it should work with the absolute path as well but i haven't tested it   
-r is the number of repetitions to run while searching for the best path  
-s is the seed to use  

if both -r and -s are used together the first run will use the seed provided while subsequent ones will use the system time as seed.

How to create the cities file
----
To use all the algorithms it's necessary to have a file containing as a first line the known optimal solution (or 0 if the solution it's not known) followed by a list of cities.  
Every city should be on a different line and each line should be structured as follows:  

```c
city_number x_coordinate y_coordinate
```
The city number is just used to identify which lines contain cities. I.E. if the first city in the list has number 80 it will still be treated as city 1, if the second one in hte list has number 90 it will be treated as city 2 and so on.  
The city order doesn't matter anyway since the starting city is chosen randomly but i guess it's useful to point it out just in case.  
The coordinates can be both integers and float numbers but the distance between two cities will be rounded to the nearest integer.  
This means that the total tour length is always going to be an integer.  
  
An example list of cities is given in the file ***ch130.tsp***

