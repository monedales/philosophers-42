# Norminette Checker Skill

## Purpose
Automatically check and fix C code according to 42 School's Norminette standards.

## When to Use
- Before committing code
- When user mentions "norm", "norminette", or "42 style"
- When creating or modifying .c or .h files
- During code review

## Key Rules

### File Structure
- **Header**: Must include standard 42 header with filename, author, dates
- **Guards**: Header files must have `#ifndef`, `#define`, `#endif` guards
- **Line Length**: Maximum 80 characters per line (including newline)
- **Function Length**: Maximum 25 lines per function
- **Lines per File**: Maximum 5 functions per file (excluding static helpers)

### Formatting
- **Indentation**: Tabs only (no spaces for indentation)
- **Braces**: Opening brace on same line for control structures
  ```c
  if (condition)
  {
      // code
  }
  ```
- **Function Braces**: Opening brace on new line
  ```c
  int function(void)
  {
      // code
  }
  ```
- **Spaces**: 
  - After keywords: `if (`, `while (`, `return `
  - Around operators: `a + b`, `i = 0`
  - No space before semicolon: `foo();` not `foo() ;`
- **Line Breaks**: 
  - Empty line between functions
  - No multiple empty lines

### Naming
- **Functions**: lowercase with underscores: `ft_strlen`, `init_data`
- **Variables**: lowercase with underscores: `last_meal_time`, `philo_count`
- **Defines**: UPPERCASE with underscores: `#define MAX_PHILOS 200`
- **Structs**: lowercase with `s_` prefix: `struct s_philo`, `typedef struct s_data`
- **Typedefs**: lowercase with `t_` prefix: `typedef struct s_philo t_philo`

### Declarations
- **Variables**: Declare at beginning of function/block
- **One per Line**: Each variable on separate line
  ```c
  int     i;
  int     count;
  char    *str;
  ```
- **Alignment**: Align variable names when declaring multiple variables
- **Initialization**: Allowed: `int i = 0;` on declaration line

### Functions
- **Max Parameters**: 4 parameters maximum
- **Max Lines**: 25 lines per function
- **Prototype**: All non-static functions must have prototype in header
- **Return**: Must return value or void
- **Empty Line**: Between function definition and first statement

### Forbidden
- `for` loops (must use `while`)
- Multiple assignments in one line: `a = b = 0;`
- Nested ternary operators
- `switch`, `case`, `goto`
- Variable-length arrays (VLA)
- Implicit type conversions
- Function calls in conditions without proper checks

## Common Issues and Fixes

### Issue: Line too long
```c
// BAD (>80 chars)
printf("Philosopher %d has taken a fork and is eating delicious spaghetti\n", id);

// GOOD
printf("Philosopher %d has taken a fork\n", id);
```

### Issue: Incorrect brace placement
```c
// BAD
if (condition) {
    code();
}

// GOOD
if (condition)
{
    code();
}
```

### Issue: Multiple declarations
```c
// BAD
int i, j, k;

// GOOD
int     i;
int     j;
int     k;
```

### Issue: Forbidden for loop
```c
// BAD
for (int i = 0; i < n; i++)
    do_something(i);

// GOOD
int     i;

i = 0;
while (i < n)
{
    do_something(i);
    i++;
}
```

## Checking Process
1. Run norminette: `norminette -R CheckForbiddenSourceHeader`
2. Check each error and warning
3. Fix systematically from top to bottom
4. Re-run until no errors

## Auto-Fix Patterns
When fixing code:
- Preserve logic and functionality
- Maintain readability
- Split long lines intelligently
- Break long functions into smaller helper functions
- Add static keyword to helper functions
- Ensure proper alignment and spacing
