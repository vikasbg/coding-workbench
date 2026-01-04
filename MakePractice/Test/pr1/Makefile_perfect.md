This is often the **safest** and most professional way to manage medium-sized projects.

Instead of scanning the whole hard drive (which might pick up garbage files) or listing every single file (which is tedious), you can simply **list the directories** you want to build, and tell Make to "grab everything inside them."

We use two specific Make commands for this: `wildcard` (to find files) and `foreach` (to loop through your list).

### The "Folder List" Makefile

Here is the clean, maintenance-free solution.

```makefile
CC = gcc
TARGET = my_app

# -----------------------------------------------------------
# 1. USER CONFIGURATION
#    Just list the specific folders you want to compile here.
# -----------------------------------------------------------
DIRS = src/drivers src/wifi src/display

# -----------------------------------------------------------
# 2. AUTOMATION
#    The Logic: "For each folder in DIRS, find all *.c files"
# -----------------------------------------------------------
FILES  = $(foreach dir, $(DIRS), $(wildcard $(dir)/*.c))

# Optional: Add main.c if it is in the root separately
FILES += main.c

# -----------------------------------------------------------
# 3. BUILD STEPS (Standard)
#    Convert the list of Source files (.c) to Object files (.o)
# -----------------------------------------------------------
OBJS = $(FILES:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@

# Pattern Rule to compile files in any folder
%.o: %.c
	$(CC) -Wall -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

```

### How the "Magic" Line works

`FILES = $(foreach dir, $(DIRS), $(wildcard $(dir)/*.c))`

This line acts like a programming loop:

1. **`$(DIRS)`**: It looks at your list: `src/drivers src/wifi ...`
2. **`foreach dir`**: It picks the first folder (`src/drivers`) and assigns it to the variable `dir`.
3. **`wildcard $(dir)/*.c`**: It executes `wildcard src/drivers/*.c`. This finds `src/drivers/uart.c`, `src/drivers/gpio.c`, etc.
4. **Repeat**: It moves to the next folder (`src/wifi`) and does it again.
5. **Result**: It stitches all these results into one long list of source files.

### Why is this the best approach?

* **Controlled:** It only builds the folders you allow. (If you have a `src/experimental` folder you don't want to build yet, just don't list it in `DIRS`).
* **Easy to read:** You can see at a glance exactly which components are part of your project.
* **Automatic:** You can add 50 files to `src/wifi`, and you don't need to touch the Makefile. Just run `make`.