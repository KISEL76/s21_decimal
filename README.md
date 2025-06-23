# s21_decimal

**An educational group project from School 21: a full reimplementation of the `decimal` data type and arithmetic logic in pure C.**

**The project involved designing a custom fixed-point decimal structure with bit-level control, followed by full implementation of arithmetic operations, comparison logic, rounding strategies, and type conversions.**

**Functions were organized in modules and rigorously tested using the `check` unit testing framework. The project also included tools for code coverage reporting.**

```bash
make            # build the decimal static library

make tests       # run all unit tests

make gcov_report # generate coverage report (HTML)

make clean       # clean all build artefacts

make rebuild     # rebuild the project from scratch
```