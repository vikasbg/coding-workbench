# Makefile : Symbols, Variable & Functions

## The "Magic" Symbols (* vs %)

These two symbols often confuse beginners. The key difference is **when** they are calculated.

### 1. The Wildcard `*` (Filesystem Matcher)
* **What it is:** A standard shell wildcard.
* **What it does:** It looks at your **Hard Drive**. It searches for files that explicitly exist right now.
* **When to use:** When you need to create a list of source files without typing them manually.

| Usage | Example | Explanation |
| :--- | :--- | :--- |
| **In Variables** | `SRC = *.c` | **WRONG.** This is just the text string `*.c`. Make doesn't look at the disk yet. |
| **In Functions** | `SRC = $(wildcard *.c)` | **CORRECT.** Make scans the folder and replaces this with `main.c add.c`. |
| **In Recipes** | `rm *.o` | **CORRECT.** The shell (bash) understands `*` and deletes the files. |

### 2. The Stem `%` (Pattern Matcher)
* **What it is:** A text placeholder (variable) for rules.
* **What it does:** It performs **Text Substitution** inside Make's brain. It does not look at the hard drive. It basically says: *"Whatever text is here, put the exact same text over there."*
* **When to use:** In targets, prerequisites, or the `patsubst` function.

| Usage | Example | Explanation |
| :--- | :--- | :--- |
| **In Rules** | `%.o : %.c` | "To make `[NAME].o`, you need `[NAME].c`." |
| **In Functions** | `$(patsubst %.c, %.o, ...)` | "Find strings ending in `.c` and change the ending to `.o`." |

---

## Automatic Variables

In GNU Make, the symbols like `$@` and `$<` are called **Automatic Variables**.
* They are the most powerful "shorthand" tools you have.
* They are calculated **automatically** by Make for every rule, saving you from typing filenames twice.

Here is the complete list of these variables and how to use them.

### The "Big Three" (Most Common)
These are the ones you will use 99% of the time in C/C++ Makefiles.

1.  **`$@` (The Target Name)**
    * **Meaning:** The full name of the **Target** (the file being built).
    * **Mnemonic:** The `@` looks like a **Target** (bullseye).
    * **Usage:** Use this when specifying the output file (e.g., `-o output`).
    ```makefile
    main.o: main.c
        # Becomes: gcc -c main.c -o main.o
        $(CC) -c main.c -o $@
    ```

2.  **`$<` (The First Prerequisite)**
    * **Meaning:** The name of the **First** dependency only.
    * **Mnemonic:** The `<` points to the **start** of the dependency list.
    * **Usage:** Use this for compilation rules. You usually only want to compile the `.c` file (the first one), even if header files are listed as dependencies.
    ```makefile
    # We depend on main.c AND defs.h
    main.o: main.c defs.h
        # Becomes: gcc -c main.c -o main.o
        # Note: It ignored defs.h because $< takes only the first item!
        $(CC) -c $< -o $@
    ```

3.  **`$^` (All Prerequisites)**
    * **Meaning:** The names of **All** dependencies, separated by spaces.
    * **Note:** It automatically removes duplicate names.
    * **Mnemonic:** The `^` means "All of the above".
    * **Usage:** Use this for **Linking**. You need *every* `.o` file to build the final binary.
    ```makefile
    app: main.o utils.o gui.o
        # Becomes: gcc main.o utils.o gui.o -o app
        $(CC) $^ -o $@
    ```

### The "Advanced" Variables (Less Common)
You might not need these immediately, but they are good to know.

1.  **`$?` (The "Newer" Prerequisites)**
    * **Meaning:** The names of all dependencies that are **newer** than the target.
    * **Usage:** Useful for "update" commands. If you are updating a library archive (`.a`), you only want to add the files that actually changed, not re-add everything.

2.  **`$*` (The "Stem")**
    * **Meaning:** The part of the filename that matched the `%` wildcard.
    * **Example:** If your rule is `%.o: %.c` and you are building `build/main.o`, the stem `$*` is `build/main`.
    * **Usage:** Rarely used in modern Makefiles (usually `patsubst` is preferred), but can be useful for constructing unrelated filenames (like generating a log file).

---

## Makefile Functions Reference

### A. File Management Functions
Use these when you need to manipulate file paths or find files on the disk.

1.  **`$(wildcard pattern)`**
    * **Usage:** `SRC = $(wildcard src/*.c)`
    * **What it does:** Scans the disk and returns a space-separated list of existing files matching the pattern.
    * **When to use:** To automatically detect source files so you don't have to list them manually.

2.  **`$(dir names...)`**
    * **Usage:** `FOLDER = $(dir src/main.o)` $\rightarrow$ returns `src/`
    * **What it does:** Extracts the directory part of a path (leaves the trailing slash).
    * **When to use:** Inside a rule to create a directory before compiling (e.g., `mkdir -p $(dir $@)`).

3.  **`$(notdir names...)`**
    * **Usage:** `NAME = $(notdir src/main.c)` $\rightarrow$ returns `main.c`
    * **What it does:** Removes the directory path, leaving only the filename.
    * **When to use:** When you have a list of paths (like `src/foo.c`) but you want to compile them all into a flat folder (like `build/foo.o`) without preserving the structure.

4.  **`$(abspath names...)`**
    * **Usage:** `FULL = $(abspath main.c)` $\rightarrow$ returns `/home/user/project/main.c`
    * **What it does:** Converts relative paths into full absolute system paths.
    * **When to use:** When passing file paths to external tools that might get confused by relative paths (like `../`).

### B. String Manipulation Functions
Use these to transform text variables (like changing file extensions).

1.  **`$(patsubst pattern,replacement,text)`**
    * **Usage:** `OBJ = $(patsubst %.c, %.o, $(SRC))`
    * **What it does:** Finds words in `text` that match `pattern` and replaces them with `replacement`. The `%` acts as the placeholder.
    * **When to use:** To generate your list of object files from your list of source files. This is the **most common** function in Makefiles.

2.  **`$(subst from,to,text)`**
    * **Usage:** `PATH = $(subst /,\ ,$(VAR))` (Replaces slashes with spaces)
    * **What it does:** Simple find-and-replace. Does **not** use `%` wildcards.
    * **When to use:** For simple text cleanups where pattern matching isn't needed.

3.  **`$(strip string)`**
    * **Usage:** `CLEAN = $(strip $(VAR))`
    * **What it does:** Removes leading/trailing spaces and replaces multiple internal spaces with a single space.
    * **When to use:** To clean up variables, especially when comparing strings in `ifeq` conditions, where extra spaces cause errors.

4.  **`$(filter pattern...,text)`**
    * **Usage:** `ONLY_C = $(filter %.c, $(FILES))`
    * **What it does:** Returns only the words in `text` that match the `pattern`.
    * **When to use:** If you have a mixed list of files (headers and source) and want to extract only the source files.

### C. System & Control Functions
Use these to talk to the shell or debug your Makefile.

1.  **`$(shell command)`**
    * **Usage:** `DATE = $(shell date)`
    * **What it does:** Runs a command in the Linux terminal and returns the output text.
    * **When to use:** To get system info (current time, git commit hash, username) into a variable.

2.  **`$(error text...)`**
    * **Usage:** `$(error GCC not found!)`
    * **What it does:** Stops Make immediately and prints the error message.
    * **When to use:** Inside `ifeq` checks to prevent the build from running if a requirement (like a specific compiler version) is missing.

3.  **`$(info text...)`**
    * **Usage:** `$(info Source files are: $(SRC))`
    * **What it does:** Prints text to the console but keeps running (like `printf`).
    * **When to use:** For debugging. If your Makefile isn't working, use this to print out variables and see what they actually contain.