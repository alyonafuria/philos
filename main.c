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
#include <unistd.h>

//**********************TYPES**********************************/
typedef struct s_data t_data;

typedef struct s_philstate
{
    int id;
    long long last_meal_ms;
    int meals_eaten;
    int left;
    int right;
    t_data *data;
    pthread_t thread;
}   t_philstate;

typedef struct s_data
{
    int philos;
    int stop;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int number_of_times;
    long long start_time;
    t_philstate *ph_states;
    pthread_t monitor;
    pthread_mutex_t *f_mutexes;
    pthread_mutex_t print_mutex;
    pthread_mutex_t state_mut;
}   t_data;

//*********************FUNCTION DECLARATIONS*******************/
int phil_atoi(char *c);
int ft_isdigit(char c);
// void cleanup(t_data data);
void do_think(t_philstate *philstate, t_data *data);
void take_forks(t_philstate *philstate, t_data *data);
void release_forks(t_philstate *philstate, t_data *data);
void do_eat(t_philstate *philstate, t_data *data);
int is_stop(t_data *data);
void *runthread(void *arg);
int hasdied(t_philstate *phil);
void *runmonitor(void *arg);
// void run_one_ph(t_data *data);
void init_mutexes(t_data *data);
void init_threads(t_data *data);
void run_philos(t_data *data);
void safe_printf(t_philstate *ph, t_data *data, char c);
int smart_sleep(t_data *data, long long target_sleep);
long long   gettime_ms(void);
long long   diff_time(t_data *data);
void init_philstates(t_data *data);
void init_data(int argc, char *argv[], t_data *data);
int is_valid_input(int argc, char *argv[]);
int main (int argc, char *argv[]);
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

// void clean(t_data data)
// {
//     //TODO: 
//     //join/detach
//     //free(data);
//     //destroy mutexes
// }

void do_think(t_philstate *philstate, t_data *data)
{
    safe_printf(philstate, data, 't');
    if (philstate->id % 2 == 0)
        smart_sleep(data, 50);
    else
        smart_sleep(data, 40);
}

void take_forks(t_philstate *philstate, t_data *data)
{
    if (philstate->id % 2 == 0)
    {
        pthread_mutex_lock(&data->f_mutexes[philstate->right]);
        safe_printf(philstate, data, 'f');
        pthread_mutex_lock(&data->f_mutexes[philstate->left]);
        safe_printf(philstate, data, 'f');
    }
    else
    {
        pthread_mutex_lock(&data->f_mutexes[philstate->left]);
        safe_printf(philstate, data, 'f');
        pthread_mutex_lock(&data->f_mutexes[philstate->right]);
        safe_printf(philstate, data, 'f');
    }
}

void release_forks(t_philstate *philstate, t_data *data)
{
     if (philstate->id % 2 == 0)
    {
        pthread_mutex_unlock(&data->f_mutexes[philstate->left]);
        pthread_mutex_unlock(&data->f_mutexes[philstate->right]);
    }
    else
    {
        pthread_mutex_unlock(&data->f_mutexes[philstate->right]);
        pthread_mutex_unlock(&data->f_mutexes[philstate->left]);
    }
}

void do_eat(t_philstate *philstate, t_data *data)
{
    pthread_mutex_lock(&data->state_mut);
    if (philstate->meals_eaten >= 0)
        philstate->meals_eaten++;
    philstate->last_meal_ms = gettime_ms();
    pthread_mutex_unlock(&data->state_mut);
    safe_printf(philstate, philstate->data, 'e');
    smart_sleep(data, data->time_to_eat);
}

int is_stop(t_data *data)
{
    int value;

    pthread_mutex_lock(&data->state_mut);
    value = data->stop;
    pthread_mutex_unlock(&data->state_mut);
    return (value);
}

void *runthread(void *arg)
{
    t_philstate *philstate;
    t_data *data;

    philstate = (t_philstate *)arg;
    data = philstate->data;
    while (!is_stop(data))
    {
        do_think(philstate, data);
        if (is_stop(data))
            return (NULL);
        take_forks(philstate, data);
        if (is_stop(data))
            return (NULL);
        do_eat(philstate, data);
        if (is_stop(data))
            return (NULL);
        release_forks(philstate, data);
        if (is_stop(data))
            return (NULL);
        safe_printf(philstate, data, 's');
        smart_sleep(data, data->time_to_sleep);
    }
    return (NULL);
}

int hasdied(t_philstate *phil)
{
    long long last_meal_cpy;
    int death;

    death = 0;
    pthread_mutex_lock(&phil->data->state_mut);
    last_meal_cpy = phil->last_meal_ms;
    if (gettime_ms() - last_meal_cpy > phil->data->time_to_die)
        death = 1;
    pthread_mutex_unlock(&phil->data->state_mut);
    return (death);
}

void *runmonitor(void *arg)
{
    t_data *data;
    t_philstate *philos;
    int i;

    data = (t_data *)arg;
    philos = data->ph_states;
    while (!is_stop(data))
    {
        i = 0;
        while (i < data->philos)
        {
            // || allfed(&philos[i])
            if (hasdied(&philos[i]))
            {
                pthread_mutex_lock(&data->state_mut);
                data->stop = 1;
                pthread_mutex_unlock(&data->state_mut);
                safe_printf(&philos[i], data, 'd');
                return (NULL);
            }
            i++; 
        }
        smart_sleep(data, 50);
    }
    return (NULL);
}

// void run_one_ph(t_data *data)
// {
//     //TODO
// }

void init_mutexes(t_data *data)
{
    int i;

    i = 0;
    while (i < data->philos)
    {
        pthread_mutex_init(&data->f_mutexes[i], NULL);
        i++;
    }
    pthread_mutex_init(&data->print_mutex, NULL);
    pthread_mutex_init(&data->state_mut, NULL);
}

void init_threads(t_data *data)
{
    int i;

    i = 0;
    while (i < data->philos)
    {
        pthread_create(&data->ph_states[i].thread, NULL, runthread, &data->ph_states[i]);
        pthread_detach(data->ph_states[i].thread);
        i++;
    }
    pthread_create(&data->monitor, NULL, runmonitor, data);
    //pthread_detach(data->monitor);
}

void run_philos(t_data *data)
{
    data->start_time = gettime_ms();
    init_philstates(data);
    if (data->philos == 1)
    {
        //run_one_ph(data);
        return ;
    }
    init_mutexes(data);
    init_threads(data);
}

void safe_printf(t_philstate *ph, t_data *data, char c)
{
    pthread_mutex_lock(&data->print_mutex);
    if (c == 'd')
        printf("%lld %d died\n", diff_time(data), ph->id);
    if (c == 'f')
        printf("%lld %d has taken a fork\n", diff_time(data), ph->id);
    if (c == 'e')
        printf("%lld %d is eating\n", diff_time(data), ph->id);
    if (c == 's')
        printf("%lld %d is sleeping\n", diff_time(data), ph->id);
    if (c == 't')
        printf("%lld %d is thinking\n", diff_time(data), ph->id);
    pthread_mutex_unlock(&data->print_mutex);
}

int smart_sleep(t_data *data, long long target_sleep)
{
    long long start_sleep;
    int success;

    start_sleep = gettime_ms();
    success = 1;
    while(gettime_ms() - start_sleep < target_sleep && data->stop == 0)
        usleep(500);
    if (is_stop(data))
        success = 0;
    return (success);
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
        data->ph_states[i].data = data;
        i++;
    }
}

void init_data(int argc, char *argv[], t_data *data)
{
    data->philos = phil_atoi(argv[1]);
    data->stop = 0;
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
    if (data.philos == 1)
        return (0);
    pthread_join(data.monitor, NULL);
    //cleanup(data);
    return (0);
}
