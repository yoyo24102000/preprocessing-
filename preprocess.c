#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "structs.h"

int main(int argc, char *argv[])
{
    // Check if there are enough command-line arguments
    if (argc < 2)
    {
        fprintf(stderr, "Error: Not enough arguments.\n");
        fprintf(stderr, "Usage: %s [OPTIONS] [SCRIPT] [ARGUMENTS ...]\n", argv[0]);
        return 1; // Error exit status
    }

    // Initialize a token list to store the processed tokens
    struct token_list *tokens = NULL;

    // Iterate through command-line arguments and preprocess
    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-c") == 0)
        {
            // Preprocess for -c option
            if (i + 1 < argc)
            {
                // Initialize a token for the shell script
                struct token *token = init_token(argv[i]);
                if (!token)
                {
                    fprintf(stderr, "Error: Failed to initialize token.\n");
                    destroy_token_list(tokens);
                    return 1; // Error exit status
                }

                // Set the token type to SHELL_SCRIPT and create arguments
                token->type = SHELL_SCRIPT;
                if (create_arg(token, argv[i + 1]) < 0)
                {
                    fprintf(stderr, "Error: Failed to preprocess -c option.\n");
                    destroy_token_list(tokens);
                    return 1; // Error exit status
                }

                // Add the token to the token list
                if (!tokens)
                {
                    tokens = malloc(sizeof(struct token_list));
                    if (!tokens)
                    {
                        fprintf(stderr, "Error: Memory allocation failed.\n");
                        destroy_token_list(tokens);
                        return 1; // Error exit status
                    }
                    tokens->size = 0;
                    tokens->token = malloc(sizeof(struct token *));
                    if (!tokens->token)
                    {
                        fprintf(stderr, "Error: Memory allocation failed.\n");
                        destroy_token_list(tokens);
                        return 1; // Error exit status
                    }
                }
                tokens->token[tokens->size++] = token;

                // Skip the next argument since it has been processed as part of -c
                ++i;
            }
            else
            {
                fprintf(stderr, "Error: -c option requires a shell script as an argument.\n");
                destroy_token_list(tokens);
                return 1; // Error exit status
            }
        }
        else
        {
            // Preprocess regular arguments
            // Initialize a token for the regular argument
            struct token *token = init_token(argv[i]);
            if (!token)
            {
                fprintf(stderr, "Error: Failed to initialize token.\n");
                destroy_token_list(tokens);
                return 1; // Error exit status
            }

            // Set the token type to COMMANDS and create arguments
            token->type = COMMANDS;
            if (create_arg(token, argv[i]) < 0)
            {
                fprintf(stderr, "Error: Failed to preprocess regular argument.\n");
                destroy_token_list(tokens);
                return 1; // Error exit status
            }

            // Add the token to the token list
            if (!tokens)
            {
                tokens = malloc(sizeof(struct token_list));
                if (!tokens)
                {
                    fprintf(stderr, "Error: Memory allocation failed.\n");
                    destroy_token_list(tokens);
                    return 1; // Error exit status
                }
                tokens->size = 0;
                tokens->token = malloc(sizeof(struct token *));
                if (!tokens->token)
                {
                    fprintf(stderr, "Error: Memory allocation failed.\n");
                    destroy_token_list(tokens);
                    return 1; // Error exit status
                }
            }
            tokens->token[tokens->size++] = token;
        }
    }

    // Process the obtained token list as needed
    // (You might have additional logic for processing tokens here)

    // Cleanup: free allocated memory
    destroy_token_list(tokens);

    return 0; // Successful exit status
}
