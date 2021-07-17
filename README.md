# NEPL

New experimental programming language, dynamically typed.

The interpreter is written in C++.

## Installation

### Build from source

Clone this repo (`git clone https://github.com/SashaVonSup/nepl.git`) and execute:
```sh
cd nepl
cd interpreter
mkdir cmake-build-debug
cd cmake-build-debug
cmake ..
make
```
Then use `./nepl` to run the built binary.

## Usage

Use `-s` or `--source` key to specify source code filepath:
```sh
./nepl -s ../../samples/numbers.nepl
```
or
```sh
./nepl --source ../../samples/persons.nepl
```

Use `-h` or `--help` key to call the list of all available keys.

## Dependencies

- gmp
- boost (program options)

## Documentation

Source code is documented with Doxygen. Build the docs after cloning the repo:
```sh
doxygen
```
Built docs will appear in `docs` directory.
