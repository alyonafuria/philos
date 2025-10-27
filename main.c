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

typedef struct s_data
{
    int philos;
    int forks; //?
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int number_of_times; //set or null;
    //TODO:
    //fork mutexes array
    //print mutex
    //start_time
    //philo_state(id, last_meal, meals_eaten, thread, fork_indices)
}   t_data;

int phil_atoi(char *c)
{
    //handle overflow by using long/uint64_t
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

void run_philos()
{
    //special case for N = 1
    //even go first
    // initialize mutexes/arrays, 
    // spawn threads, start_time, 
    // launch monitor, 
    // join/detach
}

void init_data(int argc, char *argv[], t_data *data)
{
    //TODO:
    //philos >= 1
    //times > 0
    //optional meals > 0
    data->forks = phil_atoi(argv[1]);
    data->philos = data->forks;
    data->time_to_die = phil_atoi(argv[2]);
    data->time_to_eat = phil_atoi(argv[3]);
    data->time_to_sleep = phil_atoi(argv[4]);
    if (argc == 6)
        data->number_of_times = phil_atoi(argv[5]);
    else
        data->number_of_times = -1;
}

int is_valid_input(int argc, char *argv[])
{
    int i;
    int j;

    if (argc < 5 || argc > 6)
        return (0);
    i = 1;
    while (i < argc)
    {
        j = 0;
        while (argv[i][j])
        {
            if (!ft_isdigit(argv[i][j]))
                return (0);
            j++;
        }
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
    while (1)
        run_philos();
    clean(data);
    return (0);
}
