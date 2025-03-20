/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_tokenising.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvasilen <dvasilen@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-03-20 21:10:56 by dvasilen          #+#    #+#             */
/*   Updated: 2025-03-20 21:10:56 by dvasilen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*generate_exit_status_str(int last_exit_status)
{
	char	*exit_status_str;

	exit_status_str = malloc(12);
	if (!exit_status_str)
		return (NULL);
	snprintf(exit_status_str, 12, "%d", last_exit_status);
	return (exit_status_str);
}

void	update_pointers_after_exit(char **start, char **end)
{
	(*end)++;
	*start = *end;
}

char	*handle_token_creation(Token **head, Token **current,
	char *str, int create_flag)
{
	Token	*token;

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
