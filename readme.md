# get_next_line

A C function that reads a line from a file descriptor, developed as part of the 42 school curriculum. get_next_line is a function that reads a line from a file descriptor, one line at a time. The function returns the line that was read, including the newline character `\n` if present, or `NULL` if there is nothing else to read or if an error occurred. This project helps understand buffered reading, dynamic memory management, and handling multiple file descriptors simultaneously.

## Features

Reads from multiple file descriptors simultaneously (bonus version), dynamic buffer size through `BUFFER_SIZE` macro, memory efficient with proper allocation and deallocation, handles standard input, files, and redirection, fully compliant with 42 Norminette.

## Function Prototype

```c
char *get_next_line(int fd);
