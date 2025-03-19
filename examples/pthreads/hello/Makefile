# target: prerequisites
# command to build target

include ../../../common/Makefile

FLAG += -pthread

hello: src/hello.c
	cc -g -Wall -Wextra src/hello.c -o hello -pthread

.PHONY: lint
lint:
	cpplint src/hello.c

.PHONY: memcheck
memcheck: hello
	valgrind --tool=memcheck --leak-check=full ./src/hello

.PHONY: helgrind
helgrind: hello
	valgrind --tool=helgrind ./src/hello

.PHONY: asan # invalid access & memory leaks
asan: hello.c
	cc -g -Wall -Wextra -fsanitize=address src/hello.c -o hello -pthread

.PHONY: msan # uninitialized memory
msan: hello.c
	clang -g -Wall -Wextra -fsanitize=memory src/hello.c -o hello -pthread

.PHONY: tsan # thread sanitizer
tsan: hello.c
	cc -g -Wall -Wextra -fsanitize=thread src/hello.c -o hello -pthread

.PHONY: ubsan # undefined behavior
ubsan: hello.c
	cc -g -Wall -Wextra -fsanitize=undefined src/hello.c -o hello -pthread

.PHONY: clean
clean:
	rm -f src/hello