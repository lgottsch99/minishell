/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:43:31 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/25 15:03:45 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_env	*check_existing_env(char *arg_name, t_env *envp)
{
	t_env	*tmp;

	tmp = envp;
	while (tmp)
	{
		if (ft_strncmp((char *)tmp->key, arg_name, ft_strlen(arg_name)) == 0
			&& (ft_strlen(arg_name) == ft_strlen((char *)tmp->key)))
		{
			if (ft_strlen((char *)tmp->key) == ft_strlen(arg_name))
				return (tmp);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

static int	create_new_env(char *new_var, t_env *envp)
{
	t_env	*new_node;
	char	*equal;

	new_node = (t_env *)malloc(sizeof(t_env) * 1);
	if (!new_node)
		return (1);
	equal = ft_strchr(new_var, '=');
	if (!equal)
		free(new_node);
	else
	{
		set_key_value(new_var, new_node, envp);
		new_node->next = NULL;
		add_env_back(&envp, new_node);
	}
	return (0);
}

static int	update_existing(t_env *existing_var, char *arg)
{
	char	*equal;
	char	*value;

	value = NULL;
	equal = ft_strchr(arg, '=');
	if (!equal)
		return (0);
	value = get_value_only(arg);
	free(existing_var->value);
	existing_var->value = value;
	return (0);
}

static int	update_or_create_env(t_command *cmd, t_env *envp)
{
	int		i;
	char	*arg_name;
	t_env	*existing_var;
	int		exit_stat;

	i = 1;
	while (cmd->args[i])
	{
		if (check_shellvar_rules(cmd->args[i]) == 1)
		{
			i++;
			continue ;
		}
		arg_name = get_name_only(cmd->args[i]);
		if (!arg_name)
			return (1);
		existing_var = check_existing_env(arg_name, envp);
		if (!existing_var)
			exit_stat = create_new_env(cmd->args[i], envp);
		else
			exit_stat = update_existing(existing_var, cmd->args[i]);
		free(arg_name);
		i++;
	}
	return (exit_stat);
}

// //export w/o args: ->UNIX: unspecified!
int	eexport(t_command *cmd, t_env *envp)
{
	int	num_args;
	int	exit_stat;

	exit_stat = 0;
	num_args = get_num_args(cmd->args);
	if (num_args == 1)
		exit_stat = print_env(envp);
	else if (num_args > 1)
		exit_stat = update_or_create_env(cmd, envp);
	else
	{
		printf("sth wrong with cmd list or cmd args\n");
		exit_stat = 1;
	}
	return (exit_stat);
}
