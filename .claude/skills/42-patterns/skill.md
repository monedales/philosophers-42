# 42 Project Patterns Skill

## Purpose
Apply common patterns, structures, and best practices for 42 School projects.

## When to Use
- Starting a new 42 project
- Structuring code files
- Creating Makefiles
- Setting up project architecture
- When user asks about "42 patterns", "project structure", or "42 best practices"

## Standard Project Structure

```
project_name/
├── Makefile
├── README.md
├── subject/              # Project subject (PDF)
├── include/
│   └── project_name.h    # Main header file
├── src/                  # Source files
│   ├── main.c
│   ├── init.c
│   ├── utils.c
│   └── ...
├── obj/                  # Object files (gitignored)
└── bonus/               # Bonus part (if applicable)
    ├── src_bonus/
    └── include_bonus/
```

## Standard Header Template

```c
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filename.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: username <username@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: YYYY/MM/DD HH:MM:SS by username         #+#    #+#             */
/*   Updated: YYYY/MM/DD HH:MM:SS by username        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
```

## Standard Makefile Pattern

```makefile
# Program name
NAME = program_name

# Colors for output
GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m

# Directories
SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -I $(INCLUDE_DIR)
RM = rm -rf

# Source files
SRC = main.c \
      init.c \
      utils.c

OBJS = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

# Main target
all: $(NAME)

# Compile program
$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)✓ $(NAME) compiled successfully!$(RESET)"

# Compile object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(GREEN)✓ Compiled: $<$(RESET)"

# Clean object files
clean:
	@$(RM) $(OBJ_DIR)
	@echo "$(RED)✗ Object files removed$(RESET)"

# Clean everything
fclean: clean
	@$(RM) $(NAME)
	@echo "$(RED)✗ $(NAME) removed$(RESET)"

# Rebuild
re: fclean all

.PHONY: all clean fclean re
```

## Error Handling Pattern

### Error Enum
```c
typedef enum e_error
{
    ERR_ARGS = 1,
    ERR_MALLOC,
    ERR_INVALID_INPUT,
    ERR_SYSCALL
}   t_error;
```

### Error Functions
```c
// Exit with error message
void    error_exit(char *msg, int code)
{
    write(2, "Error: ", 7);
    write(2, msg, ft_strlen(msg));
    write(2, "\n", 1);
    exit(code);
}

// Return error code
int error_return(char *msg, int code)
{
    write(2, "Error: ", 7);
    write(2, msg, ft_strlen(msg));
    write(2, "\n", 1);
    return (code);
}
```

## Initialization Pattern

```c
// Always initialize variables
int     init_data(t_data *data)
{
    data->value = 0;
    data->ptr = NULL;
    data->flag = false;
    
    // Allocate memory with check
    data->array = malloc(sizeof(int) * size);
    if (!data->array)
        return (error_return("malloc failed", ERR_MALLOC));
    
    return (0);
}
```

## Cleanup Pattern

```c
void    cleanup_data(t_data *data)
{
    if (data->array)
        free(data->array);
    if (data->ptr)
        free(data->ptr);
    // Set to NULL after freeing
    data->array = NULL;
    data->ptr = NULL;
}
```

## Parsing Pattern

```c
// Validate arguments first
int validate_args(int ac, char **av)
{
    int i;
    
    if (ac < MIN_ARGS || ac > MAX_ARGS)
        return (error_return("invalid argument count", ERR_ARGS));
    
    i = 1;
    while (i < ac)
    {
        if (!is_valid_number(av[i]))
            return (error_return("invalid number format", ERR_ARGS));
        i++;
    }
    return (0);
}

// Then parse
int parse_args(t_data *data, int ac, char **av)
{
    data->arg1 = ft_atoi(av[1]);
    data->arg2 = ft_atoi(av[2]);
    // ... more parsing
    return (0);
}
```

## Threading Pattern (for concurrent projects)

```c
// Create threads
pthread_t   thread;

if (pthread_create(&thread, NULL, routine, data))
    return (error_return("thread creation failed", ERR_THREAD));

// Join threads
if (pthread_join(thread, NULL))
    return (error_return("thread join failed", ERR_THREAD));

// Mutex initialization
pthread_mutex_t mutex;

if (pthread_mutex_init(&mutex, NULL))
    return (error_return("mutex init failed", ERR_MUTEX));

// Always destroy mutexes
pthread_mutex_destroy(&mutex);
```

## Main Function Pattern

```c
int main(int ac, char **av)
{
    t_data  data;
    
    // 1. Validate input
    if (validate_args(ac, av))
        return (ERR_ARGS);
    
    // 2. Parse arguments
    if (parse_args(&data, ac, av))
        return (ERR_ARGS);
    
    // 3. Initialize
    if (init_data(&data))
        return (ERR_INIT);
    
    // 4. Execute main logic
    run_program(&data);
    
    // 5. Cleanup
    cleanup_data(&data);
    
    return (0);
}
```

## .gitignore Pattern

```
# Object files
*.o
obj/
*.a

# Executables
program_name
*.out

# System files
.DS_Store
.vscode/
*.swp
*.swo
*~

# Test files
test
test.c
a.out
```

## Best Practices

### Memory Management
- Always free what you malloc
- Check malloc return values
- Set pointers to NULL after freeing
- Use valgrind to check leaks: `valgrind --leak-check=full ./program`

### Code Organization
- One function = one task
- Keep functions under 25 lines
- Group related functions in same file
- Use static for helper functions
- Clear, descriptive function names

### Header Files
- Include guards on all headers
- Only function prototypes, no implementations
- Group related declarations
- Include necessary system headers

### Testing
- Test edge cases (0, negative, INT_MAX, etc.)
- Test with various input combinations
- Use fsanitize: `cc -fsanitize=address -g`
- Check for race conditions (threading projects)

### Documentation
- Clear README with compilation and usage
- Comment complex logic
- Describe non-obvious algorithms
- Keep guides folder with explanations
