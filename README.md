# KART - Kernel compilation At RunTime
[![Build Status](https://travis-ci.org/noma/kart.svg?branch=master)](https://travis-ci.org/noma/kart)
[![Licence](https://img.shields.io/badge/license-BSL-blue.svg?style=flat)](LICENSE_1_0.txt)

NOTE: 
- This code is **work in progress**, the API might change.
- This code is **research** and was developed in the context of scientific high 
  performance computing, see below for **citing** information.
- **It should be useable, and is maintained.**
- **Any feedack is appreciated.**

This library provides an interface to compile and dynamically link-in code at
runtime. It was originally designed as a drop-in replacement for OpenCL's 
runtime compilation feature. Runtime compilation can facilitate compiler 
optimisation by using runtime-data as compile-time constants in performance
critical code sections. Think of all data the somehow determines memory access 
patterns (index computations), loop-counters, etc.

KART provides an C++, C and Fortran API for calling any command-line compiler
to generate code at runtime. Behind the scenes, a shared library is generated
whose functions can be used as callable kernels.

Having an easy-to-use runtime-compilation API allows for a whole bunch of 
interesting techniques beyond improved compiler optimisation, e.g.
- auto-tuning at runtime
- implementing benchmarks
- runtime code generation
- mixing different compilers
- ...

Please study the examples to quickly learn the basic concepts.

## Building

**Dependencies:**
- Boost: program_options, filesystem, system, iostreams
    - If you need to build boost (especially for the MIC), see this 
      [script](https://github.com/noma/ham#building-and-installing-boost).
    - Make sure Boost is built with the same (or an ABI compatible) compiler 
      and version as KART.
- Boost.Process which is an inofficial Boost library, which is shipped inside
  the thirdparty folder
	- NOTE: There is an different, offical Boost.Process available now from 
	  Boost v1.64 onwards to which KART the code will eventually be migrated

```
mkdir build.release
cd build.release
cmake -DCMAKE_BUILD_TYPE=Release -DKART_BUILD_EXAMPLES=TRUE ..
make -j
cd ..
```

| CMake Option             | Default | Description                   |
|--------------------------|---------|-------------------------------|
| `KART_BUILD_EXAMPLES`    | OFF     | Build example applications.   |
| `KART_ENABLE_FORTRAN`    | OFF     | Enable to build the examples. |
| `KART_ENABLE_MIC_BUILD`  | OFF     | Enable FORTRAN support.       |

## Running the Examples

NOTE: Build with CMake option `KART_BUILD_EXAMPLES=TRUE`.

C++ Example:
```bash
export KART_DEFAULT_TOOLSET_CONFIG=../config/gnu.kart
./example
```

C Example:
```bash
export KART_DEFAULT_TOOLSET_CONFIG=../config/gnu_c.kart
./examplec
```

Fortran Example:
```bash
export KART_DEFAULT_TOOLSET_CONFIG=../config/gnu_fortran.kart
./examplef
```

## Compiling Applications

- Include a KART header:
    - C++ header: `include/kart/kart.hpp`
        ```c++
        #include "kart/kart.hpp" // C++ API
        ```
    - C header: `include/kart/kart.h`
        ```c
        #include "kart/kart.h" // C API (also Fortran)
        ```
    - Fortran 90 Module: `fortran/kart.F90`
        ```fortran
        use kart
        ```
- (Statically) link your application with libkart (C++) or libkartc (C/Fortran)
- See examples 
    - `src/example.cpp`
    - `src/example.c`
    - `src/example.F90`

## KART Toolset Configuration Files

Toolsets contain paths to compiler and linker executables, was well as 
options. They often are paltform-specific and reside in files that are used to
instantiate `toolset` API objects.

- See the `*.kart` files in `config/`.
- Executable paths for linker/compiler must be absolute
    - use e.g. `which gcc` to find out
- In principle, any language/compiler/linker/options configuration can be used, 
  independent of the application.
- There is one constraint: the result must be useable with the dlopen()/dlsym()
  POSIX functions.

## Runtime Configuration

The following environment variables are available:

| Variable                      | Description |
|-------------------------------|-------------|
| `KART_DEFAULT_TOOLSET_CONFIG` | Path to a KART configuration-file that will be used for a defaul-constructed `toolset` object. |
| `KART_TMP_PATH`               | Path to a directory where temporary files will be stored. |
| `KART_KEEP_TMP_FILES`         | Set to some value to prevent deletion of temporary on `program` destruction. |
| `KART_LOG_LEVEL`              | Set to one of (increasing amount of output): `none`, `error`, `warning`, or `info` to control the amount of log messages generated during execution. |


**Examples:**
```bash
export KART_DEFAULT_CONFIG=`pwd`/../config/gnu.kart
```

```bash
export KART_TMP_PATH=/tmp
```

```bash
export KART_KEEP_TMP_FILES=1
unset KART_KEEP_TMP_FILES
```

```bash
export KART_LOG_LEVEL=none
export KART_LOG_LEVEL=error
export KART_LOG_LEVEL=warning
export KART_LOG_LEVEL=info
```

## Publications and Citing 

The idea for KART was first presented at the 
[IXPUG Annual Meeting 2015](https://www.ixpug.org/events/ixpug-annual-meeting-2015) 
at Lawrence Berkeley Lab, California, US:
Presentation: "Runtime Kernel Compilation for efficient vectorisation", Matthias Noack, 2015-09-30

A technical report was published at 2016-10-31: 
Matthias Noack, Florian Wende, Georg Zitzlsberger, Michael Klemm, Thomas Steinke
[KART – A Runtime Compilation Library for Improving HPC Application Performance](https://doi.org/10.1007/978-3-319-67630-2_29).

An invited talk about KART was given at 
[DHPCC++17](http://sycl.tech/distributed-heterogeneous-programming-in-c-cpp-dhpccpp17.html), 
Toronto, Canada, 2017-05-16.

A peer reviewed paper was presented at the 
[IXPUG Workshop "Experiences on Intel Knights Landing at the One Year Mark" at ISC'17](https://www.ixpug.org/events/workshop-isc17)
where it received a best-paper award.

If you want to cite KART, please use the 
[DOI](https://doi.org/10.1007/978-3-319-67630-2_29) 
from the latest paper above, where you can download citation information in
different formats. Here is the bibtex code (last updated 2017-12-11):  

```latex
@Inbook{Noack2017,
author="Noack, Matthias
and Wende, Florian
and Zitzlsberger, Georg
and Klemm, Michael
and Steinke, Thomas",
editor="Kunkel, Julian M.
and Yokota, Rio
and Taufer, Michela
and Shalf, John",
title="KART -- A Runtime Compilation Library for Improving HPC Application Performance",
bookTitle="High Performance Computing: ISC High Performance 2017 International Workshops, DRBSD, ExaComm, HCPM, HPC-IODC, IWOPH, IXPUG, P^3MA, VHPC, Visualization at Scale, WOPSSS, Frankfurt, Germany, June 18-22, 2017, Revised Selected Papers",
year="2017",
publisher="Springer International Publishing",
address="Cham",
pages="389--403",
isbn="978-3-319-67630-2",
doi="10.1007/978-3-319-67630-2_29",
url="https://doi.org/10.1007/978-3-319-67630-2_29"
}
```

## Acknowledgements

This work was partially supported by Intel Corporation within the "Research
Center for Many-core High-Performance Computing" (Intel PCC) at ZIB. We thank
the "The North-German Supercomputing Alliance - HLRN" for providing us access
to the HLRN-III production system 'Konrad' and the Cray TDS system with Intel
KNL nodes.
