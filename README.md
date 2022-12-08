# 2D-Steady-State-Heat-Conduction-in-a-Thin-Plate-OpenMPI

An OpenMPI program to determine the steady state heat distribution in a thin metal plate using synchronous iteration on a Distributed System. The program evenly distributes the processing of the calculations across all the processors available.

Consider a thin plate is perfectly insulated on the top and bottom having known temperatures along all its edges. The objective is to find the steady state temperature distribution inside the plate. We can find the temperature distribution by dividing the area into a fine mesh of points, hi,j. The temperature at an inside point can be taken to be the average of the temperatures of the four neighboring points.

The problem setup is as shown in the picture:

<img width="230" alt="image" src="https://user-images.githubusercontent.com/40262089/206548813-73e39b67-76a6-483d-944b-69283adff24e.png">

The program takes the following command line arguments, identified by their command line flags

1. the number of interior points (e.g. -n 100)
2. the number of iterations (e.g. -I 2000)

Note: the number of processors is given by "-np" (For example if we want to distribute our calculations among 4 nodes with 10 processors per node, the argument will be "-np 40")

The final calculated temperature values after the iterations are outputted to a csv file.
