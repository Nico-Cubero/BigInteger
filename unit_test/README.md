## How to build and run Unit Tests

This section explains steps to compile and run the Unit Tests for the Integer class.

Following instructions does apply for CMake in terminal mode on Linux-based system.
However, if you use another tool ensure following sources and dependencies are compiled and linked:

* googletest sources.
* `integer/integer.cpp` source.
* `unit_test` folder cpp source files over the `main_test.cpp` file.

Building instructions with CMake:

1. Go to `unit_test` folder:

```
cd unit_test
```

2. Make `build` folder and navigate into:

```
mkdir build
cd build
```

3. Build from `CMakeLists.txt`:

```
cmake ..
```

(optional) Flag `-DCMAKE_BUILD_TYPE=Debug` enables compilation in Debug mode which enables all the precondition and postcondition checks:

```
cmake -DCMAKE_BUILD_TYPE=Debug ..
```

4. Build `main_test` executable:

```
make
```

5. Finally, run `main_test`.

```
./main_test
```
