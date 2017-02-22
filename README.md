# GraphMatching
A graph matching library which provides stable and popular matching algorithms for the HR problem.


## Dependencies:
clang compiler, cmake, ninja (if desired)


## Installation
$ mkdir build; cd build
$ cmake -G "Ninja" ../
$ ninja

You can replace Ninja with "Unix Makefiles" in the above command.
Then type make to build.

This should build an executable named graphmatching inside the build directory.


## Usage
The executable takes a set of parameters to compute the desired matching:
-s -- compute a stable matching
-p -- compute a maximum cardinality popular matching
-m -- compute a maximum cardinality matching
-h -- compute a matching in a given HRLQ instance

To provide an input graph, and the output matching filename:
-i -- /path/to/graphfile
-o -- /path/to/store/the/matching

Also, for the -s, -p, and -m parameters, you could specify the resident/hospital
proposing algorithm (by default it runs the resident proposing algorithm).
-A -- run the resident proposing algorithm
-B -- run the hospital proposing algorithm

For e.g., to compute a stable matching with the hospitals proposing (assuming inside the build directory), do
$ ./graphmatching -B -s -i ../tests/hrlq_m6.txt -o ../tests/hrlq_m6_stable.txt


## License
MIT
