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

        0. display path and ask for input
        1. read the line (= user input), if len != 0 keep line in history
        2. evaluate/parse the line (->tokenization), build command table
        3. execute according to command table?

    }
}

*/