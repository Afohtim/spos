#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<poll.h>
#include<time.h>
#include<ncurses.h>
#include<stdbool.h>

#include "demofuncs.h"

//#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)

int x;

int prompt_timeout = 2; //in sec

int open_pipes(int* fd_f, int* fd_g)
{
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
	return 0;
}

void f_process(int x, int* fd)
{
    close(fd[0]);
    int res = f_func_or(x);
    write(fd[1], &res, sizeof(int));
    //close write end
    close(fd[1]);
}

void g_process(int x, int* fd)
{
    close(fd[0]);
    int res = g_func_or(x);
    write(fd[1], &res, sizeof(int));
    //close write end
    close(fd[1]);
}

void parent_process_1(int x, int* fd_f, int* fd_g)
{
    //closing write end
    close(fd_f[1]);
    close(fd_g[1]);


    struct pollfd polls[2];

    int f_finished = 0, g_finished = 0;
    int f_res = -1, g_res = -1;

    WINDOW* window = initscr();
    nodelay(window, true);
    set_escdelay(0);
    clear();
    wprintw(window, "press ESC to exit program\n");
    refresh();
    keypad(stdscr,TRUE);
    noecho();

    while(!g_finished || !f_finished)
    {
        //prepearing to check if there is data in pipes using poll
        polls[0].fd = fd_f[0];
        polls[0].events = POLLIN;
        polls[1].fd = fd_g[0];
        polls[1].events = POLLIN;

        //polling
        poll(polls, 2, 0);

        if(polls[0].revents & POLLIN)
        {
            read(fd_f[0], &f_res, sizeof(int));
            //close(fd_f[0]);
            if(f_res == 1)
            {
                printf("res: TRUE\n");
                return;
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
                return;
            }
            g_finished = 1;
        }
        char c = wgetch(window);
        if(c == 27)//ESC
        {
            break;
        }
    }
    char ans[50] = "";
    if(f_res == 0 && g_res == 0)
    {
        strcat(ans, "res: FALSE\n");

    }
    else if(f_res == 1 || g_res == 1)
    {
        strcat(ans, "res: TRUE\n");
    }
    else
    {
        strcat(ans, "res: UNDEFINED\n");

        if(f_res == -1)
        {
            strcat(ans, "f haven't returned value\n");

        }
        if(g_res == -1)
        {
            strcat(ans, "g haven't returned value\n");
        }
    }
    strcat(ans, "press q to close window\n");
    wprintw(window, ans);

    char c = 0;
    while(c != 'q')
    {
        c = wgetch(window);
    }
    endwin();
}

void parent_process_2(int x, int* fd_f, int* fd_g)
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
        if(user_stop == -1)
        {
            poll(polls, 2, -1);
        }
        else
        {
            poll(polls, 2, 0);
        }


        if(polls[0].revents & POLLIN)
        {
            read(fd_f[0], &f_res, sizeof(int));
            //close(fd_f[0]);
            if(f_res == 1)
            {
                printf("res: TRUE\n");
                return;
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
                return;
            }
            g_finished = 1;
        }

        if(user_stop == 1)
        {
            close(fd_f[0]);
            close(fd_g[0]);
            break;
        }

        time_t current_time;
        time(&current_time);

        if(user_stop >= 0 && difftime(current_time, start_time) >= prompt_timeout)
        {
            printf("terminate program? yes(y)/no(n)/continue without prompt(c)\n");
            char ans;
            while(ans = getchar()){
                if(ans == 'Y' || ans == 'y' || ans == 'c' || ans == 'n')
                    break;
            }
            if(ans == 'Y' || ans == 'y')
            {
                user_stop = 1;
            }
            else if(ans == 'c')
            {
                user_stop = -1;
            }
            else
            {
                time(&start_time);
            }

        }
    }
    char ans[50] = "";
    if(f_res == 0 && g_res == 0)
    {
        strcat(ans, "res: FALSE\n");

    }
    else if(f_res == 1 || g_res == 1)
    {
        strcat(ans, "res: TRUE\n");
    }
    else
    {
        strcat(ans, "res: UNDEFINED\n");

        if(f_res == -1)
        {
            strcat(ans, "f haven't returned value\n");

        }
        if(g_res == -1)
        {
            strcat(ans, "g haven't returned value\n");
        }
    }
    printf(ans);
}

int main()
{
    int cancellation_type = 0;
    while(cancellation_type != 1 && cancellation_type != 2)
    {
        printf("Chose cancellaion type\n 1 for canelations by ESC and 2 for periodic user prompt\n");
        scanf("%d", &cancellation_type);
    }

    printf("Input x:\n");
	scanf("%d", &x);

	int fd_f[2];
	int fd_g[2];
	pid_t f_id = -1, g_id = -1;

	if(open_pipes(fd_f, fd_g) == 1)
    {
        return 1;
    }

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
	    if(cancellation_type == 1)
        {
            parent_process_1(x, fd_f, fd_g);
        }
        else
        {
            parent_process_2(x, fd_f, fd_g);
        }

        return 0;

	}
	// f
	else if(f_id == 0)
    {
        f_process(x, fd_f);
        return 0;

    }
    // g
    else if(g_id == 0)
    {
        g_process(x, fd_g);
        return 0;
    }

}
