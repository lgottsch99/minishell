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
changes the current working directory of the calling process to
       the directory specified in path.

stat, lstat, fstat (man 2): 
These  functions return information about a file, in the buffer pointed
       to by statbuf.  No permissions are required on the file itself,  but—in
       the  case of stat() and lstat()— execute (search) permission
       is required on all of the directories in  pathname  that  lead  to  the
       file.
       stat()  retrieves information about the file pointed to by
       pathname
       lstat() is identical to stat(), except that if pathname is  a  symbolic
       link,  then  it returns information about the link itself, not the file
       that the link refers to.
       fstat() is identical to stat(), except that the file about which infor‐
       mation is to be retrieved is specified by the file descriptor fd.
  
unlink (man 2):		int unlink(const char *pathname)
deletes a name from the filesystem.  If that name was the last
       link to a file and no processes have the file open, the file is deleted
       and the space it was using is made available for reuse.

opendir(man):
opens a directory stream corresponding to the
       directory name, and returns a pointer to  the  directory  stream.
  
readdir(man 3):
The  readdir()  function returns a pointer to a dirent structure repre‐
       senting the next directory entry in the directory stream pointed to  by
       dirp.
   
closedir(man 3):
The  closedir()  function  closes  the directory stream associated with
       dirp.  A successful call to closedir() also closes the underlying  file
       descriptor  associated with dirp.  The directory stream descriptor dirp
       is not available after this call.

isatty(man): The  isatty()  function tests whether fd is an open file descriptor re‐
       ferring to a terminal.

ttyname(man):
The  function  ttyname() returns a pointer to the null-terminated path‐
       name of the terminal device that is open on the file descriptor fd,  or
       NULL on error (for example, if fd is not connected to a terminal).

ttyslot(man):
find the slot of the current users terminal in some file, ->read more in man

getenv:		char *getenv(const char *name);
searches the environment list to find the environment variable name, 
and returns a pointer to the corresponding value string.

ioctl( man):		int ioctl(int fd, unsigned long request, ...);
The ioctl() system call manipulates the underlying device parameters of
       special files.  In particular, many operating characteristics of  char‐
       acter  special  files  (e.g., terminals) may be controlled with ioctl()
       requests.  The argument fd must be an open file descriptor.

tgetent, 
tgetflag,
tgetnum, 
tgetstr, 
tgoto, 
tputs
for all see: https://www.mkssoftware.com/docs/man3/curs_termcap.3.asp

tcsetattr, 
tcgetattr: see man 



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
