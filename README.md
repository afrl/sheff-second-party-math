# SHEFF Second-Party Math (`sheff-second-party-math`)

`sheff-second-party-math` is a second-party module in the **Scalable Hierarchical Enrichment Finite-element Framework (SHEFF)** that compiles and wraps the C++ Mathematical Expression Toolkit Library (**ExprTk**) into a precompiled static library for fast runtime evaluation of user-defined mathematical expressions.

## Capabilities

- **String-to-Function Mathematical Expression Parsing (`second_party_math/ExprtkWrappers.h`)**: Wraps ExprTk to parse and evaluate symbolic mathematical expressions involving spatial coordinates, state variables, parameters, and standard algebraic/trigonometric functions at runtime.
- **Precompiled Static Library Packaging**: Encapsulates heavy ExprTk template instantiations behind clean C++ wrapper interfaces (`SecondPartyMath::second_party_math`), substantially reducing compile times for downstream modules such as [`sheff-constitutive`](https://github.com/afrl/sheff-constitutive) (functional material properties and position-dependent crack growth/direction criteria) and [`spawc-parser`](https://github.com/afrl/spawc-parser).

## Project Structure

- `include/second_party_math/ExprtkWrappers.h`: Public C++ wrapper interface for expression compilation and evaluation.
- `source/`: Precompiled wrapper implementations and bundled ExprTk header (`exprtk.hpp`).
- `unit_tests/`: Unit tests verifying expression parsing, variable binding, and evaluation.
- **CMake Package / Target**: `SecondPartyMath` / `SecondPartyMath::second_party_math` (static library).

## Dependencies

- **Build System**: [`sheff-build-tools`](https://github.com/afrl/sheff-build-tools) (`SHEFF_TOOLS`), CMake 3.17+, C++17/C++20 compiler.
- **SHEFF / External Dependencies**: None (self-contained static library).

## Building

When building as part of a top-level SHEFF application (such as [SPAWC](https://github.com/afrl/spawc)), this module is configured and built automatically via the top-level `make.py`.

To configure, build, and run unit tests standalone (with `SHEFF_TOOLS` set to [`sheff-build-tools`](https://github.com/afrl/sheff-build-tools)):

```bash
python3 make.py --config --build --make-tests --run-tests
```

## Public Release Information

Distribution Statement A. Approved for public release: distribution is unlimited. Case #: AFRL-2026-4513
