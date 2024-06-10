CXX = g++
CXXFLAGS = -std=c++17 -Wextra -Wall -Werror
CGFLAGS = --coverage
TESTS_DIR = tests
TESTS_EXEC_FILE = test
REPORT_DIR = report
TARGET = SmartCalc

TFLAGS = -lgtest
MEMORY_CHECKER = leaks -atExit --
ifeq ($(shell uname), Linux)
	MEMORY_CHECKER = valgrind -s --leak-check=full
endif

all: build test

build:
	mkdir build
	cd build && qmake ../$(TARGET).pro && make

install: build
	@test -d ~/$(TARGET) || mkdir -p ~/$(TARGET)
	cp ./build/$(TARGET) ~/$(TARGET)

uninstall:
	rm -rf ~/$(TARGET)
	rm -rf ./build

dvi:
	doxygen Doxyfile
	open ./html/index.html
	texi2pdf -q -c SmartCalc_v2.0_manual.tex -o SmartCalc_v2.0_manual.pdf

test: clean
	$(CXX) $(CXXFLAGS) -c $(TESTS_DIR)/*.cc $(TFLAGS) $(CGFLAGS)
	$(CXX) $(CXXFLAGS) -c lexeme.cc mainmodel.cc $(TFLAGS) $(CGFLAGS)
	$(CXX) $(CXXFLAGS) *.o -o $(TESTS_DIR)/$(TESTS_EXEC_FILE) $(TFLAGS) $(CGFLAGS)
	./$(TESTS_DIR)/$(TESTS_EXEC_FILE)

gcov_report: test
	lcov -f -d . -q -c -o $(REPORT_DIR)/report.info --no-external
	genhtml -o $(REPORT_DIR) $(REPORT_DIR)/report.info
	open ./$(REPORT_DIR)/index.html


memory_leaks_check: test
	CK_FORK=no $(MEMORY_CHECKER) ./$(TESTS_DIR)/$(TESTS_EXEC_FILE)

style_fix:
	clang-format --style=Google -i *.cc *.h
	clang-format --style=Google -i $(TESTS_DIR)/*.cc

style_check:
	clang-format --style=Google -n *.cc *.h
	clang-format --style=Google -n $(TESTS_DIR)/*.cc

cppcheck:
	cppcheck --std=c++17 --language=c++ --enable=all \
	mainmodel.* bonusmodel.* controller.* lexeme.* calcoptions.* main.cc view.* tests/tests.*

cpplint:
	cpplint --recursive --filter=-legal/copyright,-build/include_subdir,-build/header_guard \
	mainmodel.* bonusmodel.* controller.* lexeme.* calcoptions.* main.cc view.* tests/tests.*

clean:
	rm -rf build*
	rm -rf report/*
	rm -rf *.pdf
	rm -rf *.gz
	rm -rf tests/test
	rm -rf html latex
	rm -rf *.o
	rm -rf *.gcda *.gcno


