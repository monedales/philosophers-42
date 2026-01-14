# Philosophers Project Guide Skill

## Purpose
Specific guidance for the Dining Philosophers (philo) project at 42 School.

## When to Use
- Working on philosophers project
- Debugging threading or synchronization issues
- Understanding the dining philosophers problem
- When user mentions "philosophers", "philo", "dining", "deadlock", or "race condition"

## Project Overview

The Dining Philosophers problem simulates philosophers at a round table who:
1. **Think** (do nothing)
2. **Eat** (need 2 forks, dies if waits too long)
3. **Sleep** (rest after eating)

**Goal**: Keep all philosophers alive without deadlocks or data races.

## Program Arguments

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_must_eat]
```

- `number_of_philosophers`: Number of philos and forks (1-200)
- `time_to_die`: Time in ms before a philo dies without eating
- `time_to_eat`: Time in ms a philo spends eating
- `time_to_sleep`: Time in ms a philo spends sleeping
- `[number_of_times_each_must_eat]`: Optional. Simulation stops when all philos eat this many times

## Core Data Structures

### Philosopher Structure
```c
typedef struct s_philo
{
    int                 id;              // Philosopher ID (1 to N)
    int                 meals_eaten;     // Count of meals
    long                last_meal_time;  // Timestamp of last meal (ms)
    pthread_t           thread;          // Philosopher's thread
    pthread_mutex_t     *left_fork;      // Left fork mutex
    pthread_mutex_t     *right_fork;     // Right fork mutex
    t_data              *data;           // Shared data reference
}   t_philo;
```

### Shared Data Structure
```c
typedef struct s_data
{
    int                 philo_count;         // Number of philosophers
    long                time_to_die;         // Time before death (ms)
    long                time_to_eat;         // Eating duration (ms)
    long                time_to_sleep;       // Sleeping duration (ms)
    int                 must_eat_count;      // Required meals (-1 if not set)
    long                start_time;          // Simulation start timestamp
    bool                simulation_ended;    // Flag to stop simulation
    pthread_mutex_t     print_mutex;         // Protect printf
    pthread_mutex_t     death_mutex;         // Protect death checks
    pthread_mutex_t     meal_mutex;          // Protect meal data
    pthread_mutex_t     *forks;              // Array of fork mutexes
    t_philo             *philos;             // Array of philosophers
}   t_data;
```

## Critical Sections to Protect

### 1. Printing
```c
pthread_mutex_lock(&data->print_mutex);
printf("%ld %d %s\n", timestamp, philo_id, message);
pthread_mutex_unlock(&data->print_mutex);
```

### 2. Last Meal Time (Read and Write)
```c
pthread_mutex_lock(&data->meal_mutex);
philo->last_meal_time = get_time();
pthread_mutex_unlock(&data->meal_mutex);
```

### 3. Death Check
```c
pthread_mutex_lock(&data->death_mutex);
if (data->simulation_ended)
{
    pthread_mutex_unlock(&data->death_mutex);
    return ;
}
pthread_mutex_unlock(&data->death_mutex);
```

## Philosopher Routine

```c
void    *philosopher_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    
    // Even philosophers start with small delay to avoid deadlock
    if (philo->id % 2 == 0)
        usleep(1000);
    
    while (!should_stop(philo->data))
    {
        think(philo);
        eat(philo);      // Takes both forks, eats, releases forks
        sleep_philo(philo);
    }
    return (NULL);
}
```

## Avoiding Deadlock

### Problem
If all philosophers grab their left fork simultaneously, no one can get their right fork → **deadlock**.

### Solutions

#### 1. Even/Odd Strategy
```c
// Even IDs: right fork first, then left
// Odd IDs: left fork first, then right
void    take_forks(t_philo *philo)
{
    if (philo->id % 2 == 0)
    {
        pthread_mutex_lock(philo->right_fork);
        print_status(philo, "has taken a fork");
        pthread_mutex_lock(philo->left_fork);
        print_status(philo, "has taken a fork");
    }
    else
    {
        pthread_mutex_lock(philo->left_fork);
        print_status(philo, "has taken a fork");
        pthread_mutex_lock(philo->right_fork);
        print_status(philo, "has taken a fork");
    }
}
```

#### 2. Delay Even Philosophers
```c
// In routine start
if (philo->id % 2 == 0)
    usleep(1000);  // Small delay prevents simultaneous fork grabbing
```

## Avoiding Data Races

### Use Mutexes for Shared Data
- `last_meal_time` → protected by `meal_mutex`
- `meals_eaten` → protected by `meal_mutex`
- `simulation_ended` → protected by `death_mutex`
- Any `printf` → protected by `print_mutex`

### Atomic Checks
```c
// BAD: Race condition
if (data->simulation_ended)
    return ;

// GOOD: Protected check
pthread_mutex_lock(&data->death_mutex);
bool ended = data->simulation_ended;
pthread_mutex_unlock(&data->death_mutex);
if (ended)
    return ;
```

## Monitor Thread

The monitor constantly checks if any philosopher died or if all have eaten enough.

```c
void    *monitor_routine(void *arg)
{
    t_data  *data = (t_data *)arg;
    
    while (1)
    {
        if (check_deaths(data))
            break ;
        if (check_all_ate(data))
            break ;
        usleep(1000);  // Check every 1ms
    }
    return (NULL);
}
```

### Death Detection
```c
bool    check_deaths(t_data *data)
{
    int     i;
    long    now;
    long    time_since_meal;
    
    i = 0;
    while (i < data->philo_count)
    {
        now = get_time();
        pthread_mutex_lock(&data->meal_mutex);
        time_since_meal = now - data->philos[i].last_meal_time;
        pthread_mutex_unlock(&data->meal_mutex);
        
        if (time_since_meal > data->time_to_die)
        {
            print_death(&data->philos[i]);
            set_simulation_ended(data);
            return (true);
        }
        i++;
    }
    return (false);
}
```

## Precise Timing

### Get Current Time (milliseconds)
```c
long    get_time(void)
{
    struct timeval  tv;
    
    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}
```

### Precise Sleep (better than usleep for small durations)
```c
void    ft_usleep(long duration_ms)
{
    long    start;
    long    now;
    
    start = get_time();
    while (1)
    {
        now = get_time();
        if ((now - start) >= duration_ms)
            break ;
        usleep(500);  // Sleep in small chunks
    }
}
```

## Common Issues and Solutions

### Issue: Philosophers die immediately
**Cause**: `last_meal_time` not initialized to start time.
**Fix**: Set `last_meal_time = start_time` during initialization.

### Issue: Deadlock (program hangs)
**Cause**: Circular wait on mutexes.
**Fix**: Use even/odd fork ordering or delay even philosophers.

### Issue: Data races (undefined behavior)
**Cause**: Accessing shared data without mutex protection.
**Fix**: Protect all reads/writes to shared variables with appropriate mutexes.

### Issue: Delayed death messages
**Cause**: Monitor checks too infrequently or printing after simulation ends.
**Fix**: Check every 1ms and immediately print death (before stopping).

### Issue: Program doesn't stop after all philos eat enough
**Cause**: Not checking `must_eat_count` or not setting `simulation_ended`.
**Fix**: Monitor checks if all philos reached `must_eat_count`.

### Issue: One philosopher case
**Cause**: Only one fork available, philosopher can't eat.
**Fix**: Special handling - philosopher grabs fork, waits, dies.

## Testing Strategy

### Basic Tests
```bash
./philo 5 800 200 200        # Should not die
./philo 5 800 200 200 7      # Stops after 7 meals each
./philo 4 410 200 200        # Should not die (tight timing)
./philo 4 310 200 100        # Should die (impossible timing)
./philo 1 400 200 200        # One philo (should die)
```

### Stress Tests
```bash
./philo 200 800 200 200      # Many philosophers
./philo 2 410 200 200        # Two philosophers edge case
```

### Check with Tools
```bash
# Check for data races
cc -fsanitize=thread -g src/*.c -pthread -o philo
./philo 4 410 200 200

# Check for memory leaks
valgrind --leak-check=full ./philo 4 410 200 200
```

## Output Format

```
[timestamp_ms] [philosopher_id] [action]
```

Actions:
- `has taken a fork`
- `is eating`
- `is sleeping`
- `is thinking`
- `died`

Example:
```
0 1 has taken a fork
0 1 has taken a fork
0 1 is eating
200 1 is sleeping
400 1 is thinking
```

## Key Implementation Tips

1. **Initialize everything**: All mutexes, all philos, start times
2. **Protect shared data**: Use mutexes for any shared variable
3. **Stop gracefully**: Check `simulation_ended` before every action
4. **Avoid usleep for long durations**: Use precise ft_usleep with checks
5. **One philosopher edge case**: Handle separately (can't eat with 1 fork)
6. **Monitor frequency**: Check deaths every ~1ms for accuracy
7. **No prints after death**: Stop immediately after printing death message

## File Organization

```
src/
├── philosophers.c   # main(), argument validation
├── inits.c          # Initialize data, mutexes, philos
├── routine.c        # Philosopher routine, thread creation
├── actions.c        # eat, sleep, think functions
├── monitor.c        # Monitor thread, death/completion checks
├── utils.c          # get_time, ft_usleep, ft_atoi
├── parsing.c        # Argument parsing
└── cleanup.c        # Destroy mutexes, free memory
```

## References

Consult the guides in the `guides/` folder:
- `EXPLANATION.md` - Detailed project explanation
- `CONCURRENCY_CONCEPTS.md` - Threading and synchronization concepts
- `CHEAT_SHEET.md` - Quick reference for common patterns
- `STUDY_PLAN.md` - Learning roadmap for the project
