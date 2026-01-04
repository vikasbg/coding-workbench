# Makefile


## Makefile syntax
```
targets: [dependencies]; [command-0]
<tab>command-1
<tab>command-2
...
<tab>command-N
```
- In this syntax definition, square brackets ([ and ]) denote optional portions of a rule and <tab> represents a tab (ctrl-i) character.

- Almost everything in a rule is optional; the only required aspect of a rule is the targets portion and its colon (:) character.

- Use of the first command, command-0 and its preceding semicolon (;), is an optional form that’s generally not used, but is perfectly legitimate make syntax if you have a single command to execute.

- In general, targets are objects that need to be built, and dependencies are objects that provide source material for targets. Thus, targets are said to depend upon the dependencies.

## What is Makefile
- It is a set of rules.
- What is rules: Target, Dependancies & Commands.

## Makefile Default Behaviour
- The Default/Primary Target:
    - By default (running just make), the program executes the first target defined in the Makefile (often called the "Default/Primary Target").
- Specific Arguments:
    - When you run make [target_name], you are explicitly setting the entry point.
    - `make clean` usually executes only one thing because clean typically has no prerequisites. However, if clean did depend on something else, that other target would run too.
- Conclusion:
    - `make` executes one dependency tree.
    - Explanation: When you request a target (like all or app), make looks at what that target needs. It works backward.
        - If Target A needs Target B, and Target B needs Target C...
        - `make` will execute C, then B, and finally A.
        - You asked for 1 target, but `make` executed 3.
        ```Makefile
        # Target 1 (The Default)
        final_app: main.o utils.o
            gcc main.o utils.o -o final_app

        # Target 2
        main.o: main.c
            gcc -c main.c

        # Target 3
        utils.o: utils.c
            gcc -c utils.c

        # Target 4
        clean:
            rm *.o final_app
        ```

## Makefile Default Rule
- When for the target the dependancies are not found, in that case the defualt rules are used to generate those dependancies.
- For example, see below `Makefile`.
```Makefile
main: main.o add.o pr.o
	gcc main.o add.o pr.o -o main
	@echo "Binary generated"

# main.o: main.c
# 	gcc -I./ -c main.c -o main.o
# 	@echo "main.o generated."

# add.o: add.c
# 	gcc -I./ -c add.c -o add.o
# 	@echo "add.o generated."

# pr.o: pr.c
# 	gcc -I./ -c pr.c -o pr.o
# 	@echo "pr.o generated."

clean:
	rm -rf *.o main
	@echo "Binaries cleaned"
```
- When we executes this `Makefile`, see what we got.
```
$ make
cc    -c -o main.o main.c
cc    -c -o add.o add.c
cc    -c -o pr.o pr.c
gcc main.o add.o pr.o -o main
Binary generated
```
- So, how this has happened, this is because of GNU Make's Implicit Rule.

## GNU Make's Implicit Rule.
- This is very powerful (and sometimes confusing) feature of GNU Make called Implicit Rules.
- Even though you commented out your specific instructions, Make "guessed" what to do based on standard file extensions.
- The "Magic" of Implicit Rules:
    - Make has a built-in database of standard rules. It knows common patterns like:
        - "If I need a `.o` file..."
        - "...and I see a matching .c file in the directory..."
        - "...I will execute a default command to compile it."
    - Even though the `main.c` has included a custom header file i.e. `header.h`.
        - For this we did not got any error, because it found it in the current directoty since we included it as `#include "header.h"`.
        - If we would have included it like `#include <header.h>`, then definitely we would have got below error:
        ```
        $ make
        cc    -c -o main.o main.c
        main.c:4:10: fatal error: header.h: No such file or directory
            4 | #include <header.h>
            |          ^~~~~~~~~~
        compilation terminated.
        make: *** [<builtin>: main.o] Error 1
        ```
        - Refer `multipleFile` examples.

- Check below `Makefile`
```Makefile
# The makefile consists of targets and the rules for achieving the
# targets. The syntax of declaring the targets and rules

# <target-name>: <dependencies>
# <rules for creating the target>

# target=program (creating the application)
program: main.o pr.o add.o
        gcc main.o pr.o add.o -o program
        @echo "Application Created Successfully"

# target= main.o (Generate relocatable from main.c)
main.o: main.c
        gcc -I./ -c -g -O2 -o main.o main.c
        @echo "Relocatable main.o generated"

# target= pr.o (Generate relocatable from pr.c)
pr.o: pr.c
        gcc -c -g -O2 -o pr.o pr.c
        @echo "Relocatable pr.o generated"

# target= add.o (Generate relocatable from add.c)
add.o: add.c
        gcc -c -g -O2 -o add.o add.c
        @echo "Relocatable add.o generated"
```
- In above `Makefile if you see certain flags are used.
- These flags are not used in Implicit Rules.
- So, If we want to such flags then we must have our own custom rules.