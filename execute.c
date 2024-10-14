#include"execute.h"


void execute_custom_command(char **tokens, int num_tokens, char *self_commands[], char *cur, char *home)
{
    if (num_tokens > 0)
    {
        for (int a = 0; a < num_tokens; a++)
        {
            for (int u = 0; u < 11; u++)
            {
                if (strcmp(self_commands[u], tokens[a]) == 0 && tokens[a] != NULL)
                {
                    // Command execution starts here
                    if (strcmp(tokens[a], "hop") == 0)
                    {
                        if (tokens[a+1] == NULL)
                        {
                            char *modi = hop(&tokens[a+1], home);
                            strcpy(cur, modi);
                        }
                        else
                        {
                            int ww = a+1;
                            while (tokens[ww] != NULL)
                            {
                                char *modi = hop(&tokens[ww], home);
                                strcpy(cur, modi);
                                ww += 1;
                            }
                        }
                    }
                    else if (strcmp(tokens[a], "reveal") == 0)
                    {
                        char *tt_da[4096];
                        int u = 0;
                        if (tokens[a+1] != NULL)
                        {
                            for (int i = a + 1; i < num_tokens; i++)
                            {
                                tt_da[u] = (char *)malloc(strlen(tokens[i]) + 1);
                                if (tt_da[u] == NULL)
                                {
                                    printf("Memory allocation failed\n");
                                    exit(1);
                                }
                                strcpy(tt_da[u++], tokens[i]);
                            }
                        }

                        tt_da[u] = NULL;
                        reveal(tt_da);

                        for (int j = 0; j < u; j++)
                        {
                            free(tt_da[j]);
                        }
                    }
                    else if (strcmp(tokens[a], "log") == 0)
                    {
                        int execute[2] = {0, -1};
                        int purge = 0;
                        int print = 0;
                        if (a + 1 < num_tokens)
                        {
                            if (strcmp(tokens[a + 1], "execute") == 0)
                            {
                                execute[0] = 1;
                                if (a + 2 < num_tokens)
                                {
                                    execute[1] = atoi(tokens[a + 2]);
                                }
                            }
                            else if (strcmp(tokens[a + 1], "purge") == 0)
                            {
                                purge = 1;
                            }
                            else
                            {
                                print = 1;
                            }
                        }
                        else
                        {
                            print = 1;
                        }
                        log_in_file(print, purge, execute);
                    }
                    else if (strcmp(tokens[a], "proclore") == 0)
                    {
                        proclore(&tokens[a + 1]);
                    }
                    else if (strcmp(tokens[a], "seek") == 0)
                    {
                        char *tt_da[4096];
                        int u = 0;

                        for (int i = a + 1; i < num_tokens; i++)
                        {
                            if (strcmp(tokens[i], ";") == 0)
                            {
                                break;
                            }
                            tt_da[u] = (char *)malloc(strlen(tokens[i]) + 1);
                            if (tt_da[u] == NULL)
                            {
                                printf("Memory allocation failed\n");
                                exit(1);
                            }
                            strcpy(tt_da[u++], tokens[i]);
                        }
                        tt_da[u] = NULL;

                        seek(tt_da);

                        for (int j = 0; j < u; j++)
                        {
                            free(tt_da[j]);
                        }
                        
                        // Skip further processing after `seek`
                        return;  // Exit after seek is executed
                    }
                    else if (strcmp(tokens[a], "iMan") == 0)
                    {
                        controller(&tokens[a + 1]);
                    }
                    else if (strcmp(tokens[a], "activities") == 0)
                    {
                        activities();
                    }
                    else if (strcmp(tokens[a], "ping") == 0)
                    {
                        int pid = atoi(tokens[a + 1]);
                        int belh = atoi(tokens[a + 2]);
                        ping_process(pid, belh);
                    }
                    else if (strcmp(tokens[a], "fg") == 0)
                    {
                        int pid = atoi(tokens[a + 1]);
                        fg(pid);
                    }
                    else if (strcmp(tokens[a], "bg") == 0)
                    {
                        int pid = atoi(tokens[a + 1]);
                        bg(pid);
                    }
                    else if (strcmp(tokens[a], "neonate") == 0)
                    {
                        if (strcmp(tokens[a + 1], "-n") == 0)
                        {
                            if (tokens[a + 2] != NULL)
                            {
                                int t = atoi(tokens[a + 2]);
                                neonate(t);
                            }
                            else
                            {
                                printf("Error: Time not given\n");
                            }
                        }
                        else
                        {
                            printf("Error in command\n");
                        }
                    }
                }
            }
        }
    }
}
