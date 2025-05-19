CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
FN = functions/
GFLAGS = --coverage
TESTS = tests/
SYSTEM := $(shell uname -s)

ifeq ($(SYSTEM), Linux)
	OPEN_CMD = xdg-open
	ADD_LIB = -lcheck -lm -lsubunit
else ifeq ($(SYSTEM), Darwin)
	OPEN_CMD = open
	ADD_LIB = -lcheck
	BREW := ${HOME}/homebrew
else
	$(error Usupported system: $(SYSTEM))
endif

all: s21_decimal.a test gcov_report

s21_decimal.a: $(FN)*.c s21_decimal.h
	$(CC) $(CFLAGS) -c $(FN)*.c
	ar rc s21_decimal.a *.o
	rm -rf *.o

clean:
	rm -rf *.a *.o test *.gcno *.gcda *.info report/

test: s21_decimal.a
	$(CC) $(CFLAGS) $(GFLAGS) $(TESTS)*.c s21_decimal.a ${ADD_LIB} -o test

gcov_report: test
	./test
	lcov -t "s21_decimal" -o s21_decimal.info -c -d .
	genhtml -o report s21_decimal.info
	$(OPEN_CMD) report/index.html

install:
	if [ $(OS) = "LINUX" ]; then \
		sudo apt-get install -y lcov; \
		sudo apt-get install -y check; \
		sudo apt-get install -y libcapture-tiny-perl; \
		sudo apt-get install -y libdatetime-perl; \
	else \
		brew install lcov; \
	fi

rebuild: clean all
