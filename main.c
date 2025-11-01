//number_of_philosophers time_to_die time_to_eat time_to_sleep
//[number_of_times_each_philosopher_must_eat]
// number_of_philosophers: The number of philosophers and also the number
// of forks.
// ◦ time_to_die (in milliseconds): If a philosopher has not started eating within
// time_to_die milliseconds since the start of their last meal or the start of the
// simulation, they die.
// ◦ time_to_eat (in milliseconds): The time it takes for a philosopher to eat.
// During that time, they will need to hold two forks.
// ◦ time_to_sleep (in milliseconds): The time a philosopher will spend sleeping.
// ◦ number_of_times_each_philosopher_must_eat (optional argument): If all
// philosophers have eaten at least number_of_times_each_philosopher_must_eat
// times, the simulation stops. If not specified, the simulation stops when a
// philosopher dies.
// • Each philosopher has a number ranging from 1 to number_of_philosophers.
// • Philosopher number 1 sits next to philosopher number number_of_philosophers.
// Any other philosopher, numbered N, sits between philosopher N - 1 and philosopher
// N + 1.
// ./philo 5 200 5 5 [10]

// Any state change of a philosopher must be formatted as follows:
// ◦ timestamp_in_ms X has taken a fork
// ◦ timestamp_in_ms X is eating
// ◦ timestamp_in_ms X is sleeping
// ◦ timestamp_in_ms X is thinking
// ◦ timestamp_in_ms X died

// memset, printf, malloc, free, write,
// usleep, gettimeofday, pthread_create,
// pthread_detach, pthread_join, pthread_mutex_init,
// pthread_mutex_destroy, pthread_mutex_lock,
// pthread_mutex_unlock

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>

//**********************TYPES**********************************/
typedef struct s_philstate
{
    int id;
    long long last_meal_ms;
    int meals_eaten;
    int left;
    int right;
    pthread_t thread;
}   t_philstate;

typedef struct s_data
{
    int philos;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int number_of_times;
    long long start_time;
    t_philstate *ph_states;
    pthread_mutex_t *f_mutexes;
    pthread_mutex_t print_mutex;
}   t_data;

//*********************FUNCTION DECLARATIONS*******************/
int phil_atoi(char *c);
int ft_isdigit(char c);
void clean(t_data data);
void run_philos(t_data *data);
void init_data(int argc, char *argv[], t_data *data);
int is_valid_input(int argc, char *argv[]);
long long   gettime_ms(void);
long long   diff_time(t_data *data);
void run_one_ph(t_data *data);
void init_philstates(t_data *data);
void *runthread(void *arg);
//*************************************************************** */
int phil_atoi(char *c)
{
    long num;
    int i;

    i = 0;
    num = 0;
    while (c[i])
    {
        num *= 10;
        num += c[i] - '0';
        if (num > INT_MAX)
            return(-1);
        i++;
    }
    return ((int)num);
}

int ft_isdigit(char c)
{
    if (c < '0' || c > '9')
        return (0);
    return (1);
}

void clean(t_data data)
{
    //TODO: 
    //join/detach
    //free(data);
    //destroy mutexes
}
void *runthread(void *arg)
{
    t_philstate *philstate = (t_philstate *)arg;
    //TODO: write routine
    //check with monitor?
    return (NULL);
}

void run_one_ph(t_data *data)
{
    //TODO
}

void init_mutexes_and_threads(t_data *data)
{
    int i;

    i = 0;
    while (i < data->philos)
    {
        pthread_mutex_init(&data->f_mutexes[i], NULL);
        i++;
    }
    pthread_mutex_init(&data->print_mutex, NULL);
    i = 0;
    while (i < data->philos)
    {
        pthread_create(&data->ph_states[i].thread, NULL, runthread, &data->ph_states[i]);
        i++;
    }
}

void run_philos(t_data *data)
{
    data->start_time = gettime_ms();
    init_philstates(data);
    if (data->philos == 1)
    {
        run_one_ph(data);
        return ;
    }
    init_mutexes_and_threads(data);
    //even go first
    // initialize mutexes/arrays, 
    // spawn threads, start_time, 
    // launch monitor, 
    // join/detach
}

long long gettime_ms(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return ((long long)tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

long long   diff_time(t_data *data)
{
    long long   now;

    now = gettime_ms();
    return (now - data->start_time);
}

void init_philstates(t_data *data)
{
    int i;

    i = 0;
    while (i < data->philos)
    {
        data->ph_states[i].id = i + 1;
        data->ph_states[i].last_meal_ms = data->start_time;
        data->ph_states[i].meals_eaten = 0;
        data->ph_states[i].left = i;
        data->ph_states[i].right = (i + 1) % data->philos;
        i++;
    }
}

void init_data(int argc, char *argv[], t_data *data)
{
    data->philos = phil_atoi(argv[1]);
    data->time_to_die = phil_atoi(argv[2]);
    data->time_to_eat = phil_atoi(argv[3]);
    data->time_to_sleep = phil_atoi(argv[4]);
    if (argc == 6)
        data->number_of_times = phil_atoi(argv[5]);
    else
        data->number_of_times = -1;
    data->ph_states = malloc(sizeof(t_philstate) * data->philos);
    data->f_mutexes = malloc(sizeof(pthread_mutex_t) * data->philos);
}

int is_valid_input(int argc, char *argv[])
{
    int i;
    int j;

    if (argc < 5 || argc > 6)
        return (printf("wrong amount of args\n"), 0);
    i = 1;
    while (i < argc)
    {
        j = 0;
        while (argv[i][j])
        {
            if (!ft_isdigit(argv[i][j]))
                return (printf("args should be ints\n"), 0);
            j++;
        }
        if (phil_atoi(argv[i]) < 1)
            return (printf("args should be ints and > 0\n"), 0);
        i++;
    }
    return (1);
}

int main (int argc, char *argv[])
{
    t_data data;
    if (!is_valid_input(argc, argv))
        return (1);
    init_data(argc, argv, &data);
    run_philos(&data);
    clean(data);
    return (0);
}
