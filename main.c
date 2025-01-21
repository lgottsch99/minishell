//pseudocode structure
/*

int main(int argc, char *argv[], char *envp[]) //envp is the environment variable, ->type env in bash 
{
  // Load config files, builtins etc

  // Run basic loop of shell
  basic_loop(); //see below

  // Perform any shutdown/cleanup/freeing

  return EXIT_SUCCESS; 
}

void basic_loop()
{
    while ()
    {
        INSIDE LOOP:
        "REPL" read, evaluate, print, loop (loop only terminates when program is closed)

        0. display prompt
        1. read the line (= user input)
        2. evaluate/parse the line (->tokenization: check how ft strtok works) (pipes? ""? redirections? etc...)
        3. execute

    }
}

*/