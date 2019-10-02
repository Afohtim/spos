// C program to demonstrate use of fork() and pipe()
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<sys/wait.h>
#include<poll.h>
#include<time.h>
#include<stdbool.h>

//#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)

int x;

int overtime = 5; //in sec



int f_func_or(int x)
{
    sleep(8);
    return x%2;
}

int g_func_or(int x)
{
    sleep(9);
    return (x+1)%2;
}

int main()
{
    printf("Input x:\n");
	scanf("%d", &x);

	int fd_f[2];
	int fd_g[2];

	char fixed_str[] = "forgeeks.org";
	char input_str[100];
	pid_t f_id = -1, g_id = -1;

	if (pipe(fd_f)==-1)
	{
		fprintf(stderr, "Pipe f Failed" );
		return 1;
	}
	if (pipe(fd_g)==-1)
	{
		fprintf(stderr, "Pipe g Failed" );
		return 1;
	}

	//scanf("%s", input_str);
	f_id = fork();

	if (f_id < 0)
	{
		fprintf(stderr, "fork f Failed" );
		return 1;
	}

	if(f_id > 0)
    {
        g_id = fork();

        if(g_id < 0)
        {
           fprintf(stderr, "fork f Failed" );
            return 1;
        }
    }



	// Parent process
	if (f_id > 0 && g_id > 0)
	{
        //closing write end
        close(fd_f[1]);
        close(fd_g[1]);


        struct pollfd polls[2];

        int f_finished = 0, g_finished = 0;
        int f_res = -1, g_res = -1;
        int user_stop = 0;

        time_t start_time;
        time(&start_time);
        while(!g_finished || !f_finished)
        {
            //prepearing to check if there is data in pipes using poll
            polls[0].fd = fd_f[0];
            polls[0].events = POLLIN;
            polls[1].fd = fd_g[0];
            polls[1].events = POLLIN;

            //polling
            if(!user_stop)
                poll(polls, 2, overtime * 1000);
            else
                poll(polls, 2, 0);

            if(polls[0].revents & POLLIN)
            {
                read(fd_f[0], &f_res, sizeof(int));
                //close(fd_f[0]);
                if(f_res == 1)
                {
                    printf("res: TRUE\n");
                    return 0;
                }
                f_finished = 1;
            }
            if(polls[1].revents & POLLIN)
            {
                read(fd_g[0], &g_res, sizeof(int));
                //close(fd_g[0]);
                if(g_res == 1)
                {
                    printf("res: TRUE\n");
                    return 0;
                }
                g_finished = 1;
            }

            if(user_stop)
            {
                close(fd_f[0]);
                close(fd_g[0]);
                break;
            }

            time_t current_time;
            time(&current_time);

            if(difftime(current_time, start_time) >= overtime)
            {
                printf("terminate program? Y/y/N\n");
                char ans;
                while(ans = getchar()){
                    if(ans == 'Y' || ans == 'N' || ans == 'y')
                        break;
                };
                if(ans == 'y')
                {
                    user_stop = 1;
                }
                else if(ans = 'Y')
                {
                    break;
                }
                else
                {
                    start_time = current_time;
                }

            }
        }

        if(f_res == 0 && g_res == 0)
        {
            printf("res: FALSE");
        }
        else if(f_res == 1 || g_res == 1)
        {
            printf("res: TRUE");
        }
        else
        {
            printf("res: UNDEFINED");
        }
        return 0;

	}
	// f
	else if(f_id == 0 )
    {
        //close read end
        close(fd_f[0]);
        int res = f_func_or(x);
        write(fd_f[1], &res, sizeof(int));
        //close write end
        close(fd_f[1]);
        return 0;

    }
    // g
    else if(g_id == 0)
    {
        //close read end
        close(fd_g[0]);
        int res = g_func_or(x);
        write(fd_g[1], &res, sizeof(int));
        //close write end
        close(fd_g[1]);
        return 0;
    }
}
