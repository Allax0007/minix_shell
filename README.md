# minix_shell

Thisis a simple command-line interface program developed in C for Minix systems. It allows users to execute commands, manage processes, handle input/output redirection, support piping, and enable background execution.

## Compilation Instructions

1. Ensure you have a C compiler installed on your Minix system.
2. Download the source code files for the shell program.
3. Open a terminal and navigate to the directory containing the source code files.
4. Run the following command to compile the source code:
`gcc -o minix_shell minix_shell.c` or `clang -o minix_shell minix_shell.c` 
> Replace `minix_shell` with your desired output executable name if needed.

## Running the Shell

1. After successful compilation, execute the compiled shell program using the following command:
`./minix_shell`
2. The shell will display a prompt in the format "user@minix3 /current_directory %" indicating it is ready to accept commands.
3. Enter commands at the prompt and press Enter to execute them.
4. To exit the shell, type "exit" and press Enter, or use Ctrl+C to terminate the shell.

> If no command is entered or 'cd' command is used without parameters, the shell may encounter a segmentation fault.

## Features

- Basic command execution
- Process management
- Input/output redirection
- Piping
- Background execution




