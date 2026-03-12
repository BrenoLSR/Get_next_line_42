<div align="center">

# 📖 Get Next Line

### *Reading files, one line at a time*

<img src="https://img.shields.io/badge/42-SP-000000?style=for-the-badge&logo=42&logoColor=white"/>
<img src="https://img.shields.io/badge/Language-C-00599C?style=for-the-badge&logo=c&logoColor=white"/>
<img src="https://img.shields.io/badge/Buffered-Reading-FF6B6B?style=for-the-badge"/>
<img src="https://img.shields.io/badge/100%25-C-blue?style=for-the-badge"/>
<img src="https://img.shields.io/badge/Status-Completed-brightgreen?style=for-the-badge"/>

</div>

---

## 📖 Table of Contents

- [About](#-about)
- [Function Prototype](#-function-prototype)
- [Project Structure](#-project-structure)
- [How It Works](#-how-it-works)
- [The Static Variable](#-the-static-variable)
- [BUFFER_SIZE](#-buffer_size)
- [Return Value](#-return-value)
- [Function Reference](#-function-reference)
- [Edge Cases](#-edge-cases)
- [Usage](#-usage)
- [Examples](#-examples)
- [Compilation](#-compilation)
- [Author](#-author)

---

## 📚 About

**Get Next Line (GNL)** is a project from the **42 School** curriculum that challenges you to implement a function capable of reading a single line from a file descriptor with each call — no matter how large the file is or how small the read buffer is.

The project introduces two critical concepts in C programming: **buffered reading** with a configurable `BUFFER_SIZE`, and **static variables** to persist state between successive calls to the same function. After completing GNL, the function becomes a staple utility reused in all future 42 projects that require file or stdin reading.

---

## 🔣 Function Prototype

```c
char    *get_next_line(int fd);
```

| Parameter | Type | Description |
|---|---|---|
| `fd` | `int` | File descriptor to read from (file, stdin, pipe, etc.) |

| Return Value | Condition |
|---|---|
| `char *` | A newly allocated string containing the next line (including `\n` if present) |
| `NULL` | End of file reached, read error, or invalid `fd` |

---

## 🗂️ Project Structure

```
Get_next_line_42/
│
├── get_next_line.h         # Header — prototype, BUFFER_SIZE define, includes
├── get_next_line.c         # Core — main GNL logic using static variable
└── get_next_line_utils.c   # Helpers — string utilities used by GNL
```

> The project is intentionally minimal: just **3 files**. These are designed to be dropped directly into any other 42 project that needs line-by-line reading.

---

## ⚙️ How It Works

`get_next_line` uses a **static variable** (`stash`) to preserve leftover data between calls. Each invocation follows this flow:

```
get_next_line(fd)
│
├── 1. READ from fd into a buffer of size BUFFER_SIZE
│       └── append buffer content to the static stash
│
├── 2. SEARCH the stash for a newline '\n'
│       ├── found → extract everything up to and including '\n' as the line
│       │             update stash to hold the remainder after '\n'
│       │             return the extracted line
│       └── not found → keep reading (loop back to step 1)
│
├── 3. END OF FILE reached (read returns 0)
│       ├── stash has remaining content → return it as the last line
│       └── stash is empty → return NULL
│
└── 4. ERROR (read returns -1 or fd invalid) → free stash, return NULL
```

### Visual Diagram

```
FILE CONTENT:          "Hello\nWorld\nFoo"
                        ─────────────────

1st call → returns:    "Hello\n"
                        stash = "World\nFoo"

2nd call → returns:    "World\n"
                        stash = "Foo"

3rd call → returns:    "Foo"
                        stash = NULL

4th call → returns:    NULL   (EOF)
```

---

## 🔒 The Static Variable

The key to GNL's design is a **static variable** that persists its value between function calls:

```c
char    *get_next_line(int fd)
{
    static char *stash;
    // ...
}
```

Unlike a regular local variable (which is destroyed when the function returns), a static variable **retains its value** across calls. This allows GNL to remember what was left unread in the buffer from the previous call and continue from exactly where it left off.

| Property | Local Variable | Static Variable |
|---|---|---|
| Lifetime | Until function returns | Until program ends |
| Initialized | Every call | Only once (first call) |
| Scope | Inside the function | Inside the function |
| Use in GNL | ❌ Can't retain state | ✅ Persists leftover data |

---

## 📐 BUFFER_SIZE

`BUFFER_SIZE` controls how many bytes are read from the file descriptor in each `read()` call. It is defined in `get_next_line.h` and can be overridden at compile time:

```c
#ifndef BUFFER_SIZE
# define BUFFER_SIZE 42
#endif
```

### Effect of BUFFER_SIZE on performance

| BUFFER_SIZE | Behaviour |
|---|---|
| Very small (e.g. `1`) | Many `read()` calls — correct but slow |
| Moderate (e.g. `42`, `1024`) | Good balance of correctness and speed |
| Very large (e.g. `100000`) | Few `read()` calls — faster for large files |

> The function must work correctly for **any positive value** of `BUFFER_SIZE`. This is tested during evaluation.

Override at compile time:

```bash
gcc -D BUFFER_SIZE=64 main.c get_next_line.c get_next_line_utils.c
```

---

## 🔢 Return Value

| Situation | Returns |
|---|---|
| Line successfully read (with `\n`) | `"Hello\n"` — allocated string |
| Last line of file (no trailing `\n`) | `"last line"` — allocated string without `\n` |
| End of file, nothing left to read | `NULL` |
| Read error | `NULL` |
| Invalid file descriptor (`fd < 0`) | `NULL` |
| `BUFFER_SIZE <= 0` | `NULL` |

> ⚠️ The caller is responsible for **freeing** the returned string.

---

## 📋 Function Reference

### Core (`get_next_line.c`)

| Function | Description |
|---|---|
| `get_next_line(int fd)` | Main function. Reads the next line from `fd` using a persistent static stash. Returns the line as a newly allocated string, or `NULL` on EOF or error. |

### Utilities (`get_next_line_utils.c`)

| Function | Description |
|---|---|
| `ft_strlen` | Returns the length of a string. Used to calculate sizes for allocation. |
| `ft_strchr` | Searches for a character (typically `\n`) within the stash to detect line endings. |
| `ft_strjoin` | Concatenates two strings into a new allocation. Used to append buffer chunks to the stash. |
| `ft_substr` | Extracts a substring from the stash — used to isolate the line to return and the remainder. |
| `ft_strdup` | Duplicates a string. Used to copy data safely into new allocations. |

> These utilities are implemented from scratch inside `get_next_line_utils.c`, without relying on `libft` or any external string library — keeping GNL fully self-contained and portable.

---

## ⚠️ Edge Cases

| Case | Expected Behaviour |
|---|---|
| Empty file | First call returns `NULL` |
| File with only `\n` | Returns `"\n"`, second call returns `NULL` |
| File with no trailing newline | Last line is returned without `\n`, next call returns `NULL` |
| Very large file | Works correctly regardless of file size |
| `BUFFER_SIZE = 1` | Works correctly, one byte read at a time |
| Binary files / null bytes | Behaviour is undefined — designed for text files |
| `fd = 0` (stdin) | Reads from standard input line by line |
| Invalid fd (e.g. `-1`) | Returns `NULL` immediately |
| Multiple file descriptors | Each `fd` should maintain its own stash (bonus) |

---

## 🔗 Usage

Copy the three files into your project and include the header:

```c
#include "get_next_line.h"
```

Then compile alongside your project:

```bash
gcc main.c get_next_line.c get_next_line_utils.c -o my_program
```

Or with a custom buffer size:

```bash
gcc -D BUFFER_SIZE=128 main.c get_next_line.c get_next_line_utils.c -o my_program
```

---

## 💡 Examples

### Reading a file line by line

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int     fd;
    char    *line;

    fd = open("file.txt", O_RDONLY);
    if (fd < 0)
        return (1);
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}
```

**Input file (`file.txt`):**
```
Hello, 42!
Welcome to Get Next Line.
Last line without newline
```

**Output:**
```
Hello, 42!
Welcome to Get Next Line.
Last line without newline
```

### Reading from stdin

```c
char *line;

while ((line = get_next_line(0)) != NULL)
{
    printf("Read: %s", line);
    free(line);
}
```

---

## 🛠️ Compilation

Since GNL has no `Makefile` of its own (it is designed to be included in other projects), compile it directly alongside your source files:

```bash
# Default BUFFER_SIZE (defined in get_next_line.h)
gcc main.c get_next_line.c get_next_line_utils.c -o program

# Custom BUFFER_SIZE at compile time
gcc -D BUFFER_SIZE=1 main.c get_next_line.c get_next_line_utils.c -o program

# With memory check
valgrind --leak-check=full ./program
```

---

## 👤 Author

<div align="center">

**Breno LSR**
42 São Paulo

[![GitHub](https://img.shields.io/badge/GitHub-BrenoLSR-181717?style=for-the-badge&logo=github)](https://github.com/BrenoLSR)
[![42](https://img.shields.io/badge/42-São_Paulo-000000?style=for-the-badge&logo=42&logoColor=white)](https://www.42sp.org.br/)

</div>

---

<div align="center">

*"A journey of a thousand miles begins with a single line."*

</div>
