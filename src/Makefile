cc = gcc -g -Wall -Werror -Wextra -std=c11
GCOVFLAGS = -fprofile-arcs -ftest-coverage

OS = $(shell uname)
ifeq ($(OS), Linux)
	CHECK_FLAGS = -lpthread -lcheck -pthread -lrt -lm -lsubunit
else
	CHECK_FLAGS = -lcheck
endif

ifeq ($(OS),Linux)
	OPEN_CMD = xdg-open
endif
ifeq ($(OS),Darwin)
	OPEN_CMD = open
endif

all: install tests gcov_report

install: uninstall
	@mkdir build
	@mkdir build/Viewer_v1
	@chmod -R 777 build
	qmake-qt5 Qt/viewer.pro -o build/Viewer_v1/Makefile && make -C build/Viewer_v1	
	# qmake Qt/viewer.pro -o build/Viewer_v1/Makefile && make -C build/Viewer_v1	
	rm -rf build/Viewer_v1/*.o

uninstall:
	@rm -rf build;

dist:
	mkdir dist
	cp -r Qt dist
	cp -r Cfiles dist
	cp -r Makefile dist
	tar -czvf Viewer_v1.0_dist.tar.gz dist/
	rm -rf dist

dvi:
	$(OPEN_CMD) dvi.html

test:
	$(cc) Cfiles/affine_transformations.c Cfiles/parser.c Cfiles/tests.c Cfiles/*.h $(CHECK_FLAGS) -o s21_test
	./s21_test

gcov_report:
	$(cc) $(GCOVFLAGS) Cfiles/affine_transformations.c Cfiles/parser.c Cfiles/tests.c Cfiles/*.h  $(CHECK_FLAGS) -o s21_test
	./s21_test
	lcov -t "test" -o test.info -c -d .
	genhtml -o report/ test.info
	rm -rf *.o *.gcno *.gcda 
	$(OPEN_CMD) ./report/index.html

clang:
	clang-format -style=Google -n Cfiles/*.c Cfiles/*.h Qt/*.cpp Qt/*h

leaks: test
	CK_FORK=no leaks --atExit -- ./s21_test

brew:
	cd ~
	curl -fsSL https://rawgit.com/kube/42homebrew/master/install.sh | zsh

lcov:
ifeq ($(OS),Linux)
	sudo apt install lcov
endif
ifeq ($(OS),Darwin)
	brew install lcov
endif

valgrind:
	$(cc) Cfiles/affine_transformations.c Cfiles/parser.c Cfiles/tests.c Cfiles/*.h $(CHECK_FLAGS) -o s21_test
	valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./s21_test

clean:
	rm -rf *.o *.gcno *.gcda *.a *.info report .clang-format s21_test *.out
