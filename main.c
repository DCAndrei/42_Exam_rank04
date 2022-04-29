/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acretu <acretu@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/29 22:03:44 by acretu            #+#    #+#             */
/*   Updated: 2022/04/29 22:08:05 by acretu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//exit code 10 : failing fork
//exit code 21 : failing dup or dup2
//exit code 20 : failing pipe
//exit code 13 : failing execve
//exit code 11 : cd without correct arguments
//exit code 12 : chdir failed

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define ERR2 "error: fatal\n"
#define ERR3 "error: cd: bad arguments\n"
#define ERR4 "error: cd: cannot change directory to "
#define ERR5 "error: cannot execute "

int 	ft_strlen(char *str);
void    ft_cd(char *argv[]);
void    ft_execute_semicolon(int end, char *argv[], char *envp[]);
void    ft_execute_pipes(int nb_pipes, int end, char *argv[], char *envp[]);
void	ft_execute(char *argv[], char *envp[]);

int main(int argc, char *argv[], char *envp[])
{
    int     i;
    int     j;

    if (argc == 1)
    {
        return (1);
    }
    i =0;
    argc -= 1;
    argv += 1;
    while (i < argc)
    {
        j = 0;
        while (j < argc)
        {
            if (strcmp(argv[j], ";") == 0)
                break;
            j += 1;
        }
        ft_execute_semicolon(j, argv, envp);
        argc -= j + 1;
        argv += j + 1;
        i = 0;
    }
    return (0);
}

int ft_strlen(char *str)
{
    int     len;

    len =0;
    while (str[len] !='\0')
        len += 1;
    return (len);
}

void    ft_execute_semicolon(int end, char *argv[], char *envp[])
{
    int     pid;
    int     nb_pipes;
    int     i;

    i = 0;
    nb_pipes = 0;
    while (i < end)
    {
        if (strcmp(argv[i], "|") == 0)
            nb_pipes += 1;
        i += 1;
    }
    if (nb_pipes == 0)
    {
        argv[end] = NULL;
        pid = fork();
        if (pid == -1)
        {
            write(2, ERR2, ft_strlen(ERR2));
            exit (10);
        }
        if (pid == 0)
        {
           ft_execute(argv, envp);
        }
        waitpid(pid, NULL, 0);
    }
    else
    {
        ft_execute_pipes(nb_pipes, end, argv, envp);
    }
}

void    ft_execute_pipes(int nb_pipes, int end, char *argv[], char *envp[])
{
    int     i;
    int     pid;
    int     fd[2];
    int     fd_out;
    int     next_pipe;

    fd_out = dup(1);
    argv[end] = NULL;
    if (fd_out == -1)
    {
        write(2, ERR2, ft_strlen(ERR2));
        exit (21);
    }
    i = 0;
    while (i < nb_pipes)
    {
        next_pipe = 0;
        while (strcmp(argv[next_pipe], "|") != 0 && argv[next_pipe] != NULL)
            next_pipe += 1;
        if (pipe(fd) == -1)
        {
            write(2, ERR2, ft_strlen(ERR2));
            exit (20);
        }
        pid = fork();
        if (pid == -1)
        {
            write(2, ERR2, ft_strlen(ERR2));
            exit (10);
        }
        if (pid == 0)
        {
            close (fd_out);
            close (fd[0]);
            if (dup2(fd[1], 1) == -1)
            {
                write(2, ERR2, ft_strlen(ERR2));
                exit (21);
            }
            close (fd[1]);
            argv[next_pipe] = NULL;
			ft_execute(argv, envp);
        }
        waitpid(pid, NULL, 0);
        close (fd[1]);
        if (dup2(fd[0], 0) == -1)
        {
            write(2, ERR2, ft_strlen(ERR2));
            exit (21);
        }
        close (fd[0]);
        i += 1;
        argv += next_pipe + 1;
    }
    if (dup2(fd_out, 1) == -1)
    {
        write(2, ERR2, ft_strlen(ERR2));
        exit (21);
    }
    pid = fork();
    if (pid == -1)
    {
        write(2, ERR2, ft_strlen(ERR2));
        exit (10);
    }
    if (pid == 0)
    {
       ft_execute(argv, envp);
    }
    waitpid(pid, NULL, 0);
}

void    ft_cd(char *argv[])
{
    //char  s[100];
    
    if (argv[1] == NULL)
    {
        write(2, ERR3, ft_strlen(ERR3));
        exit (11);
    }
    if (argv[2] != NULL)
    {
        write(2, ERR3, ft_strlen(ERR3));
        exit (11);
    }
    if (chdir(argv[1]) == -1)
    {
        write(2, ERR4, ft_strlen(ERR4));
        write(2, argv[1], ft_strlen(argv[1]));
        write(2, "\n", 1);
        exit (12);
    }
    //printf("%s\n", getcwd(s, sizeof(s)));
    exit (0);
}

void	ft_execute(char *argv[], char *envp[])
{
	if (strcmp(argv[0], "cd") == 0)
		ft_cd(argv);
	else if (execve(argv[0], argv, envp) == -1)
	{
		write(2, ERR5, ft_strlen(ERR5));
		write(2, argv[0], ft_strlen(argv[0]));
		write(2, "\n", 1);
		exit (13);
	}
}
