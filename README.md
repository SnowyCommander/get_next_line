*This project has been created as part of the 42 curriculum by jiylee.*

# Description
- ## For?
	Helping me understand an important concept in C programming: static variables.
- ## Overview?
	This activity is about programming a function that returns a line read from a file descriptor. Its version is 14.2 including the Bonus part.
- ## What?
	- ### Mandatory
		A function that returns a line read from a file descriptor, reading one line at a time with each successive call.
	- ### Bonus
		An enhanced version that can handle multiple file descriptors simultaneously, allowing interleaved reading from different sources without losing track of each descriptor's reading position. This is implemented using only a single static variable.


# Instructions
## Installation
```bash
	git clone <repository-url> working_directory
	cd working_directory
```
## Compilation

Since this project has no Makefile, compile directly with `cc` and the `-D` flag to define BUFFER_SIZE.

**Mandatory:**
```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 main.c get_next_line.c get_next_line_utils.c -o program
```

**Bonus:**
```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 main.c get_next_line_bonus.c get_next_line_utils_bonus.c -o program
```

You can change the value of `BUFFER_SIZE` to any positive integer to test different read sizes.
Type `./program` to execute.


## Usage
```c
	#include "get_next_line.h"
	#include <fcntl.h>

	int main(void)
	{
		int		fd;
		char	*line;

		fd = open("example.txt", O_RDONLY);
		while ((line = get_next_line(fd)) != NULL)
		{
			printf("%s", line);
			free(line);
		}
		close(fd);
		return (0);
	}
```
	Compile with:
```bash
	cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 main.c get_next_line.c get_next_line_utils.c -o program
```
	Execute the program with:
```bash
	./program
```

# Resources
- ## References
	- [get_next_line subject PDF](https://cdn.intra.42.fr/pdf/pdf/195513/en.subject.pdf)
- ## How AI was used
	- ### What?
		- I used only Opus 4.6 of Claude.
	- ### Which tasks?
		- Getting hints of how logics should flow when I got stuck.
		- Getting detailed knowledge of everything like knowing full-name of each title.
		- Getting evaluation of my original source-code and refactored one, to choose which one should survive.
		- Getting to know why this code must be like this, like each intention of some codes, which cases I should use which code, as the right code in the right place.
		- Writing this README.md, then I edited it.

# Algorithm

## Why a Static Variable?
The `read` system call does not care about newlines. When we call `read(fd, buf, BUFFER_SIZE)`, it simply reads up to `BUFFER_SIZE` bytes regardless of where `\n` appears. This means a single `read` call may contain more data than one line, or a line may span across multiple `read` calls. We need somewhere to store the leftover data between function calls. A local variable would be destroyed when the function returns, so a **static variable** is the natural choice, as it retains its value across multiple invocations of the same function.

## Core Reading Logic
1. `get_next_line` is called with a file descriptor `fd`
2. It reads from `fd` into a temporary buffer of `BUFFER_SIZE` bytes using the `read` system call
3. The read content is appended to the static variable (the "backup") that persists across calls
4. Reading repeats until a newline `\n` is found in the backup or `read` returns 0 (EOF)

**Why loop until `\n` is found?**
A single `read` call with a small `BUFFER_SIZE` may not capture a full line. For example, if `BUFFER_SIZE` is 4 and the first line is `"Hello\n"`, it takes two reads (`"Hell"` then `"o\n"`) to reach the newline. Conversely, if `BUFFER_SIZE` is large, one read may capture multiple lines at once, which is why the leftover must be saved.

## Line Extraction
1. Once the backup contains a newline, it is split at the first `\n`
2. Everything up to and including `\n` is returned as the current line
3. Everything after `\n` remains in the backup for the next call
4. If there is no newline and EOF is reached, the remaining backup is returned as the last line

**Why include `\n` in the returned line?**
The subject requires it. This also allows the caller to distinguish between a mid-file line (ends with `\n`) and the final line of a file that has no trailing newline (no `\n` at the end).

## Bonus: Multiple File Descriptors
1. Instead of a single `static char *backup`, a `static char *backup[1024]` array is used
2. Each index corresponds to a file descriptor, so `backup[fd]` stores the leftover data for that specific `fd`
3. This allows interleaved calls like `get_next_line(3)`, `get_next_line(4)`, `get_next_line(3)` without losing any reading position

**Why an array indexed by `fd`?**
File descriptors are non-negative integers assigned sequentially by the OS, making them perfect natural indices for an array. Compared to alternatives like a linked list of `(fd, backup)` pairs, an array provides O(1) direct access with no search overhead. {O(1) access to an array is possible because of the combination of three factors: random access characteristics of RAM + contiguous memory layout + address calculation (addition).} The trade-off is that it allocates space for `1024` pointers regardless of how many are used, but since each element is just a pointer (8 bytes), the total memory cost is negligible. In reality, even if only three file types are used, 1,024 pointers are allocated. However, 8KB is a negligible size by modern computer standards. That's why the the text used the term "negligible". On the other hand, a linked list saves memory because it only creates nodes for the three file types actually used, but each search takes O(n) time.

| | Array (`backup[1024]`) | Linked List |
|---|---|---|
| Lookup speed | O(1) — direct access | O(n) — traversal required |
| Memory usage | ~8KB fixed (mostly empty) | Only as much as the number of active fds |
| Implementation complexity | Simple | Requires node creation, deletion, and search logic |
| Conclusion | Fast ✅ slight memory waste | Memory-efficient ✅ slower |