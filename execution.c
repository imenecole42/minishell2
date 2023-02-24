/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imraoui <imraoui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 13:22:00 by imraoui           #+#    #+#             */
/*   Updated: 2023/02/24 18:56:10 by imraoui          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_cmd(t_cmd *exec, t_data *mini)
{
	if (exec->nbr_cmd == 1)
	{
		//if (builtin_env(exec->cmd) == 1)
			//is_builtin(mini, exec->cmd);
		//else
			printf("nbr de commande egal 1\n");
			exec_one_cmd(exec, mini);
			
	}
	else
	{
		exec_pipe(exec,mini);
	}
}
void 	open_file(t_cmd *pipex)
{
	int i = pipex->nbr_cmd;
	if(pipex->fd_in > 1)
		dup2(pipex->fd_in,0);	
	else if (i != 0)
		dup2(pipex->pfd[i - 1][0],0);
	if(pipex->fd_out > 1)
	{
		dup2(pipex->fd_out,1);	
	}
	else if(i != pipex->nbr_pipe )
		dup2(pipex->pfd[i][1],1);	
}
void	exec_one_cmd(t_cmd *exec, t_data *mini)
{
	exec->pid_one = fork();
	if (exec->pid_one == -1)
		return (write(2, "Error Fork\n", 12), free(NULL));
	else if (exec -> pid_one == 0)
	{
		if(exec->fd_in > 2)
			dup2(exec->fd_in,0);
		if(exec->fd_out > 2)
			dup2(exec->fd_out,1);
		if (exec->cmd[0] && get_cmd(exec->cmd[0], mini->env))
		{
			execve(get_cmd(exec->cmd[0], mini->env), exec->cmd, mini->env);
			ft_free(exec->cmd);
			exit(127);
		}
		else
		{
			ft_putstr_fd(exec->cmd[0], 2);
			write(2, ": command not found\n", 20);
			ft_free(exec->cmd);
			exit(127);
		}
	}
	else
	{
		waitpid(exec->pid_one, 0, 0);
		ft_close_fd(exec);
	}
}

/*void	fork_pipe(t_cmd *pipex, t_data *mini)
{
	// printf("argv[0] = %s\n", pipex->cmd[0]);
	// printf("argv->next->[0] = %s\n", pipex->next->cmd[0]);
	print_liste_cmd(&mini->cmds);
	pipex -> pid = fork();
	if (pipex -> pid == -1)
		return (write(2, "Error Fork\n", 12), free(NULL));
	if (pipex -> pid[pipex -> i] == 0)
	{
		// pipex->fdin = pipe[i][0]
		// --> pipex->fdin = close si redir --> pipex->fdin = redirIN
		// pipex->fdout = pipe[i][0]
		// --> pipex->fdout = close si redir --> pipex->fdout = redirOUT
		// si pipex->fdin
		// dup2(fdin, 0);
		// dup2(fdout, 1);

/*
		if(exec->fd_in != 2)
			dup2(exec->fd_in,0);
		if else (premiere commande)
		{
			exec->fd_in = pipe[i][0];
		}
		if else (ni premiere ni derniere)
		{
			exec->fd_in = pipe[i - 1][0];
		}
		else
		{
			close (pipe[i][0]);
			fdin = 0;
		}
		if(exec->fd_out != 2)
			dup2(exec->fd_out,1);
		if else (premiere commande)
		{
			exec->fd_out = pipe[i][1];
		}
		if else (ni premiere ni derniere)
		{
			exec->fd_out = pipe[i][1];
		}
		else
		{
			close (pipe[i][1]);
			fdout = 1;
		}

		if (pipex ->i == 0)
			child1(pipex);
		else if (pipex -> i == pipex->nbr_cmd - 1)
			child2(pipex);
		else
		{
			ft_dup(pipex -> pfd[pipex -> i][1],
				pipex -> pfd[pipex -> i - 1][0]);
			ft_close(pipex);
		}
		child(pipex, mini->env);
	}
}
/*
void	exec_pipe(t_cmd *pipex,t_data *mini)
{
	// printf("argv[0] = %s\n", pipex->cmd[0]);
	// printf("argv->next->[0] = %s\n", pipex->next->cmd[0]);
	while(pipex != NULL)
	{
		creat_pipes(pipex);
		pipex -> pid = fork();
		if (pipex -> pid == -1)
			return (write(2, "Error Fork\n", 12));
		else if(pipex ->pid == 0)
		{
			open_file(pipex);
			child(pipex, mini->env);
		}
		else 
			ft_close(pipex);
			waitpid(pipex->pid, NULL, 0);
			pipex = pipex->next;
		}
		//free_pfd(pipex);
}

