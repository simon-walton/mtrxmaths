Mtrxmaths is a simple utility for matrix arithmetic. It is similar to mtrxmath, but internally uses the Eigen library.

##### Build instructions
The project is vanilla CMake E.g. (in root of checked-out tree)
```
mkdir build
cd build
cmake ..
make
```

##### Matrix file-format
One row per line, components separated by commas or whitespace. Lines starting with '#' are ignored.

