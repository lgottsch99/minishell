#include "../includes/minishell.h"

Token *create_token(char *value, Token_type type) {
    Token *token = malloc(sizeof(Token));
    if (!token) {
        return NULL; 
    }
    token->value = value;
    token->type = type;
    token->next = NULL;
    //printf("Created token: %s, Type: %d\n", value, type); 
    return token;
}

void add_token(Token **head, Token **current, Token *token) {
    if (!*head) {
        *head = token;
    }
    if (*current) {
        (*current)->next = token; 
    }
    *current = token; 
}

void handle_pipe(char **start, char **end, Token **head, Token **current) {
    Token *token;
    if (*end > *start) {
        char *value = ft_strndup(*start, *end - *start);
        token = create_token(value, TOKEN_WORD);
        //printf("Assigning TOKEN_WORD to token: %s\n", value); 
        add_token(head, current, token);
    }
    token = create_token(ft_strdup("|"), TOKEN_PIPE);
    *start = ++(*end);
    //printf("Assigning TOKEN_PIPE to token: %s\n", "|"); 
    add_token(head, current, token);
}

void handle_single_quote(char **start, char **end, Token **head, Token **current) {
    (*end)++;
	*start = *end;
	while(**end && **end != '\'')
		(*end)++;
	char *value = ft_strndup(*start, *end - *start);
	Token *token = create_token(value, TOKEN_WORD);
	add_token(head, current, token);
	(*end)++;
	*start = *end;
}

void handle_double_quote(char **start, char **end, Token **head, Token **current) {
    (*end)++;
	*start = *end;
	while(**end && **end != '"')
		(*end)++;
	char *value = ft_strndup(*start, *end - *start);
	Token *token = create_token(value, TOKEN_WORD);
	add_token(head, current, token);
	(*end)++;
	*start = *end;
}

void handle_env_var(char **start, char **end, Token **head, Token **current) {
    Token *token;
	(*end)++;
	char *var_start = *end;
	while(**end && (**end == '_' || isalnum(**end))) //todo
		(*end)++;
	char *var_name = ft_strndup(var_start, *end - var_start);
	char *var_value = getenv(var_name); //todo 
	if (var_value){
		token = create_token(var_value, TOKEN_WORD);
		add_token(head, current, token);
	} else{
		token = create_token(ft_strdup(""), TOKEN_WORD);
		add_token(head, current, token);
	}
	free(var_name);
	*start = *end;
}

void handle_redir_more(char **start, char **end, Token **head, Token **current) {
    Token *token;
    if (*end > *start) {
        char *value = ft_strndup(*start, *end - *start);
        token = create_token(value, TOKEN_WORD);
        //printf("Assigning TOKEN_WORD to token: %s\n", value); 
        add_token(head, current, token);
    }
    if (*(*end + 1) == '>') {
        token = create_token(ft_strdup(">>"), TOKEN_REDIRECT_APPEND);
        //printf("Assigning TOKEN_REDIRECT_APPEND to token: %s\n", ">>"); 
        add_token(head, current, token);
        (*end)++;
    } else {
        token = create_token(ft_strdup(">"), TOKEN_REDIRECT_OUT);
        //printf("Assigning TOKEN_REDIRECT_OUT to token: %s\n", ">"); 
        add_token(head, current, token);
    }
    *start = ++(*end);
}

void handle_redir_less(char **start, char **end, Token **head, Token **current) {
    Token *token;
    if (*end > *start) {
        char *value = ft_strndup(*start, *end - *start);
        token = create_token(value, TOKEN_WORD);
        //printf("Assigning TOKEN_WORD to token: %s\n", value); // Debug print
        add_token(head, current, token);
    }
    if (*(*end + 1) == '<') {
        token = create_token(ft_strdup("<<"), TOKEN_REDIRECT_HEREDOC);
        //printf("Assigning TOKEN_REDIRECT_HEREDOC to token: %s\n", "<<"); // Debug print
        add_token(head, current, token);
        (*end)++;
    } else {
        token = create_token(ft_strdup("<"), TOKEN_REDIRECT_IN);
        //printf("Assigning TOKEN_REDIRECT_IN to token: %s\n", "<"); // Debug print
        add_token(head, current, token);
    }
    *start = ++(*end);
}

Token *tokenize(char *input) {
    Token *head = NULL;
    Token *current = NULL;
    char *start = input;
    char *end = input;

    while (*end) {
        if (isspace(*end)) {
            if (end > start) {
                char *value = ft_strndup(start, end - start);
                Token *token = create_token(value, TOKEN_WORD);
                //printf("Assigning TOKEN_WORD to token: %s\n", value); // Debug print
                add_token(&head, &current, token);
            }
            start = ++end;
        } else if (*end == '|') {
            handle_pipe(&start, &end, &head, &current);
        } else if (*end == '>') {
            handle_redir_more(&start, &end, &head, &current);
        } else if (*end == '<') {
            handle_redir_less(&start, &end, &head, &current);
        } else if (*end == '"') {
            handle_double_quote(&start, &end, &head, &current);
        } else if (*end == '\'') {
            handle_single_quote(&start, &end, &head, &current);
        } else if (*end == '$') {
			handle_env_var(&start, &end, &head, &current);
        } else 
            end++;
    }

    if (start != end) {
        char *value = ft_strndup(start, end - start);
        Token *token = create_token(value, TOKEN_WORD);
        //printf("Assigning TOKEN_WORD to token: %s\n", value); // Debug print
        add_token(&head, &current, token);
    }
    Token *token = create_token(NULL, TOKEN_END);
    //printf("Assigning TOKEN_END to token: (null)\n"); // Debug print
    add_token(&head, &current, token);

    return head;
}

void print_tokens(Token *tokens) {
    while (tokens) {
        printf("Token: %s, Type: %d\n", tokens->value, tokens->type);
        tokens = tokens->next;
    }
}

void free_tokens(Token *tokens) {
    Token *tmp;
    while (tokens) {
        tmp = tokens;
        tokens = tokens->next;
        if (tmp->value) {
            free(tmp->value);
        }
        free(tmp);
    }
}