/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_tokenising.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 21:10:56 by dvasilen          #+#    #+#             */
/*   Updated: 2025/03/25 16:56:15 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*generate_exit_status_str(int last_exit_status)
{
	char	*exit_status_str;
	int		num_len;
	int		temp;

	num_len = 1;
	temp = last_exit_status;
	while (temp >= 10)
	{
		temp /= 10;
		num_len++;
	}
	exit_status_str = malloc(num_len + 1);
	if (!exit_status_str)
		return (NULL);
	exit_status_str[num_len] = '\0';
	while (num_len--)
	{
		exit_status_str[num_len] = (last_exit_status % 10) + '0';
		last_exit_status /= 10;
	}
	return (exit_status_str);
}

void	update_pointers_after_exit(char **start, char **end)
{
	(*end)++;
	*start = *end;
}

char	*handle_token_creation(t_token **head, t_token **current,
	char *str, int create_flag)
{
	t_token	*token;

	if (create_flag)
	{
		token = create_token(str, TOKEN_WORD);
		if (!token)
		{
			free(str);
			return (NULL);
		}
		add_token(head, current, token);
		return (NULL);
	}
	else
		return (str);
}

char	*extract_var_name(char **end)
{
	char	*var_start;

	var_start = *end;
	while (**end && (**end == '_' || ft_isalnum(**end)))
		(*end)++;
	return (ft_strndup(var_start, *end - var_start));
}

char	*extract_var_name_and_value(char **end, char **envp)
{
	char	*var_name;
	char	*var_value;
	char	*value_copy;

	var_name = extract_var_name(end);
	if (!var_name)
		return (NULL);
	var_value = ft_getenv(var_name, envp);
	free(var_name);
	if (var_value)
		value_copy = ft_strdup(var_value);
	else
		value_copy = ft_strdup("");
	return (value_copy);
}
