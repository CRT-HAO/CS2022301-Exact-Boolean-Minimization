# CS2022301-Exact-Boolean-Minimization

> 資訊工程系 二年級 乙班 張皓鈞 B11030202



## Requirements

- GNU GCC/Clang with C++14 supported
- CMake >= 3.18
- Make or Ninja build tool

> Microsoft Visual C++ may be supported, but not tested



## Build

Use CMake to build the program

```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```



## Usage

```shell
Usage: ./exact_boolean_minimization <input_pla> <output_pla> [--debug]
```

### Example

```shell
./exact_boolean_minimization ../examples/set03/case1.pla ./output/set03/case1.pla
Total number of terms: 3
Total number of literals: 6
```

### Output



## Test platform

![neofetch](./assets/neofetch.png)

### macOS

```
Darwin ip139-166.wlan.ntust.edu.tw 22.6.0 Darwin Kernel Version 22.6.0: Wed Jul  5 22:22:52 PDT 2023; root:xnu-8796.141.3~6/RELEASE_ARM64_T8103 arm64
```

### Apple Clang

```
Apple clang version 15.0.0 (clang-1500.0.40.1)
Target: arm64-apple-darwin22.6.0
Thread model: posix
InstalledDir: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin
```

### CMake

```
cmake version 3.26.4
```



## Test Cases

### Case 1

[./examples/set03/case1.pla](./examples/set03/case1.pla)

#### Input PLA

```pla
.i 4
.o 1
.ilb a b c d
.ob f
.p 7
001- 1
0111 1
10-1 1
1101 1
0001 -
1010 -
1111 -
.e
```

#### Output PLA

```pla
.i 4
.o 1
.ilb a b c d 
.ob f 
.p 3
-01- 1
--11 1
1--1 1
.e
```

#### Best solutions

- $b'c + cd + ad$



### Case 2

[./examples/set03/case2.pla](./examples/set03/case2.pla)

#### Input PLA

```pla
.i 5
.o 1
.ilb a b c d e
.ob f
.p 8
0110- 1
-1000 1
0-00- -
01-01 1
0-101 1
10-10 1
1-1-0 1
010-- 1
.e
```

#### Output PLA

```pla
.i 5
.o 1
.ilb a b c d e 
.ob f 
.p 5
0--01 1
010-- 1
-1-00 1
10-10 1
1-1-0 1
.e
```

#### Best solutions

- $a'd'e + a'bc' + bd'e' + ab'de' + ace'$



#### Case 3

[./examples/set03/case3.pla](./examples/set03/case3.pla)

#### Input PLA

```pla
.i 6
.o 1
.ilb a b c d e g
.ob f
.p 8
00--00 1
1-11-- -
-11-01 1
0-1110 1
11-011 1
011101 1
-11--0 1
11-0-1 1
.e
```

#### Output PLA

```pla
.i 6
.o 1
.ilb a b c d e g 
.ob f 
.p 5
00--00 1
--11-0 1
-11-0- 1
-11--0 1
11-0-1 1
.e
```

#### Best solutions

- $a'b'e'g' + cdg' + bce' + bcg' + abd'g$

