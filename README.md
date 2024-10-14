[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Qiz9msrr)
# Description
Welcome to the custom POSIX shell !

This shell has following major components: 
1. main.c : The main function, from where the shell is run 
2. hop.c : Implementing the hop functionality, which enables one to jump from one directory to the other
3. log.c : Implementing the log functionality, which displays list of commands implemented yet
4. proclore.c: Implementing the proclore functionality , which displays process details
5. prompt.c : Implementing the prompt functionality
6. reveal.c : Implementing the reveal functionality, similar to cd in normal shell.
7. seek.c : Implementing the seek functionality, displays list of files and directories matching the search value.
8. background-handler.c: Implementation of system commands and handling of background processes
9. execute.c : To help implement the custom commands
10. readlines.c: Reads a file and returns the contents of a file in an array of strings
11. record_keeper.c : Used to add commands to the file "example.txt" that is created on execution 
12. alias_parsing.c : Helper method used to parse aliases present in .myshrc file
13. .myshrc : contains all aliases created by us
14. fgbg.c: Implementation of the fg and bg commands
15. headers.h : Contains some structs and process list
16. iMan.c : Implementation of iMan specification, used to fetch man pages
17. neonate.c : Implementation of neonate specification, which prints the most recently created process id

# Assumptions:
1. If a command is not found, it will be treated like a system command
2. The code prints out a message for both the background processes and incase of termination by ctrl+c, this is intended behaviour
3. Ctrl+D can be used to exit only when waiting for a prompt to be entered
4. If a command is not found, it'll be treated as a system command
5. Neonate, being a custom command, will not be affected by Ctrl+C or Ctrl+Z, as they work only for system commands
6. In activities, if a command receives a SIGKILL then it is removed from activities.
7. In activities, if a command receives a SIGTERM, it will be retained in activities and the status will be stopped.
8. In activities, the status stopped is an umbrella term that will include all states except when the process is running in the foreground or the background.
9. In Iman, for the case of invalid commands, it returns nothing, thereby preventing shell crash.
10. snprintf gives a warning, which can be ignored safely.
11. sed does not work with execvp
12. While doing echo,we can pass the string without putting quotations.
13. neonate command only takes integer values are valid argument for time.
14. While piping for custom commands, there is a warning raised, which can be ignored safely.
15. Please note that there is a slight difference in the implementation of Ctrl+C and Ctrl+Z in terms of displaying the message or not, please ignore as the actual implementation is taking place.
16. If a program receives SIGINT, it cannot be restarted using bg or SIGCONT
17. Aliases have to be created by the user themselves and they are is no implementation to pass them as a argument in the shell.

