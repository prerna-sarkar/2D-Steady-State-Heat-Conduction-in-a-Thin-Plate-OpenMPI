# 2D-Steady-State-Heat-Conduction-in-a-Thin-Plate-OpenMPI

An OpenMPI program to determine the steady state heat distribution in a thin metal plate using synchronous iteration on a Distributed Systems.

Consider a thin plate is perfectly insulated on the top and bottom having known temperatures along all its edges. The objective is to find the steady state temperature distribution inside the plate. We can find the temperature distribution by dividing the area into a fine mesh of points, hi,j. The temperature at an inside point can be taken to be the average of the temperatures of the four neighboring points.

The problem setup is as shown in the picture:

image

The program takes the following command line arguments, identified by their command line flags

the number of interior points (e.g. -n 100)
the number of iterations (e.g. -I 2000)
The final calculated temperature values after the iterations are outputted to a csv file.
