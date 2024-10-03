#include <iostream>
#include <pthread.h>
#include <cstdio>
#include <cstdlib>

using namespace std;
int sum; /* this data is shared by the thread(s) */
pthread_mutex_t sum_mutex; // Mutex for synchronizing access to sum

void *runner(void *param); /* the thread */

int main(int argc, char *argv[])
{
    pthread_t tid[10]; /* the thread identifier */
    pthread_attr_t attr; /* set of attributes for the thread */

    if (argc != 2) {
        fprintf(stderr,"usage: a.out <integer value>\n");
        return -1;
    }

    if (atoi(argv[1]) < 0) {
        fprintf(stderr,"Argument %d must be non-negative\n",atoi(argv[1]));
        return -1;
    }

    /* Initialize mutex */
    pthread_mutex_init(&sum_mutex, nullptr);

    /* get the default attributes */
    pthread_attr_init(&attr);

    /* create the thread */
    for (auto & i : tid) {
        pthread_create(&i,&attr,runner,argv[1]);
    }

    /* now wait for the thread to exit */
    for (auto & i : tid) {
        pthread_join(i,nullptr);
    }

    cout << "sum = " << sum << endl;

    /* Clean up mutex */
    pthread_mutex_destroy(&sum_mutex);
    return 0;
}

void *runner(void *param)
{
    int i, upper = atoi(reinterpret_cast<const char *>(param));
    int partial_sum = 0;

    if (upper > 0) {
        for (i = 1; i <= upper; i++)
            partial_sum += i;
    }

    /* Acquire lock before modifying sum */
    pthread_mutex_lock(&sum_mutex);
    sum += partial_sum; // Update the shared sum
    cout << "partial sum = " << partial_sum << ", sum = " << sum << endl;
    /* Release lock after modifying sum */
    pthread_mutex_unlock(&sum_mutex);

    pthread_exit(nullptr);
}
