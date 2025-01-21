//extern fts allowed and what they do 

printf, 
malloc, 
free,
write,

//prompting, getting user input, history
readline (man 3):    use for prompt + getting the prompt line
    see also: https://tiswww.case.edu/php/chet/readline/readline.html 

rl_clear_history:   void rl_clear_history (void)
    Clear the history list by deleting all of the entries, in the same manner as the History library’s clear_history() function. This differs from clear_history because it frees private data Readline saves in the history list.

rl_on_new_line:     int rl_on_new_line (void)
    Tell the update functions that we have moved onto a new (empty) line, usually after outputting a newline

rl_replace_line:    void rl_replace_line (const char *text, int clear_undo)
Replace the contents of rl_line_buffer with text. The point and mark are preserved, if possible. If clear_undo is non-zero, the undo list associated with the current line is cleared.

rl_redisplay:   void rl_redisplay (void) 
Change what’s displayed on the screen to reflect the current contents of rl_line_buffer

add_history:    void add_history (const char *string) 
Place string at the end of the history list. The associated data field (if any) is set to NULL.
see also: https://linux.die.net/man/3/history


//processes /piping
fork: clone current process (creating one parent + one child process)

wait: make process wait until one child finishes

pipe: create pipe

exit:	void exit(int status)
The  exit()  function  causes  normal process termination and the least
significant byte of status (i.e., status & 0xFF)  is  returned  to  the
parent (see wait(2)).


waitpid, 
wait3, 
wait4

//I/O redirections
dup, 
dup2


//execution
	//checking if files /cmds exist/ are executable
access, open, read, close,

execve: execute command that is not built-in


//signal handling
signal,  sigaction, sigemptyset, sigaddset, kill, 

//get paths, working directories etc
getcwd(man):		char *getcwd(char *buf, size_t size)
The getcwd() function copies an absolute pathname of the current  work‐
       ing directory to the array pointed to by buf, which is of length size.

chdir(man): 	int chdir(const char *path)
chdir() changes the current working directory of the calling process to
       the directory specified in path.


  stat, lstat, fstat, unlink, 
  opendir, readdir, closedir,
 isatty, ttyname, ttyslot, ioctl,
getenv, tcsetattr, tcgetattr, tgetent, tgetflag,
tgetnum, tgetstr, tgoto, tputs

//error message handling 
strerror (man): function returns a pointer to a  string  that  describes
       the  error  code  passed  in  the  argument  errnum, possibly using the
       LC_MESSAGES part of the current locale to select the  appropriate  lan‐
       guage.   (For  example,  if  errnum is EINVAL, the returned description
       will be "Invalid argument".)  This string must not be modified  by  the
       application,  but may be modified by a subsequent call to strerror() or
       strerror_l().  No other library  function,  including  perror(3),  will
       modify this string. ...


perror (man): The  perror()  function produces a message on standard error describing
       the last error encountered during a call to a system or  library  func‐
       tion.
