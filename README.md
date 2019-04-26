# sieveOfEratosthenes

The sieve of Eratosthenes is a simple algorithm to generate a list of prime numbers upto a given limit. This procedure is attributed to the Greek mathematician Eratoshthenes of Cyrene.

Dependencies
============
* [GNU Multi Precision Arithmetic Library](https://gmplib.org/)
* [cmake](https://cmake.org/) version 3.5 or greater

Installation
============
Typical installation command:
```
mkdir build
cd build
cmake ../
make
```

If GMP is installed in a non-default location, you should run cmake with:

```
cmake ../ -DGMP_ROOT_DIR=<GMP install location>
```
