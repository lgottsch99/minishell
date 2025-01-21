
/*
A shell does three main things in its lifetime:
Initialize: 
    In this step, a typical shell would read and execute its configuration files. These change aspects of the shell’s behavior.
Interpret: 
    Next, the shell reads commands from stdin (which could be interactive, or a file) and executes them.
Terminate: 
    After its commands are executed, the shell executes any shutdown commands, frees up any memory, and terminates.

int main(int argc, char **argv)
{
  // Load config files, if any.

  // Run command loop.
  lsh_loop();

  // Perform any shutdown/cleanup.

  return EXIT_SUCCESS;
}

INSIDE LOOP
REPL read, evaluate, print, loop (loop only terminates when program is closed)

1. read the line (into buffer? ggf needed to make bigger dynamically?)
2. evaluate/parse the line (->tokenization: check how ft strtok works) (pipes? ""? redirections? ...)
3. execute


??? 
minishell = interactive bc we ask for propmt? yes
what is a good hierarchy to parse and structure program? like 1. redirections, then pipes then etc? -> parse tree

idea:
parse and count pipes ->set loop for pipes

*/

/*-------------------------------------------------------------------------------------------------------------------------
chatgpt on how to work in a team:

Here is a structured approach to successfully complete the project:
---

### **1. Establish a common understanding**

Before you start, make sure you both have a clear understanding of the requirements and goals.
**Documentation of requirements**: Collaborate on creating a document summarizing all the features and specifications of the shell.
**Clarification of ambiguities**: Discuss what certain requirements mean, e.g. B. how exactly characters like `$` or `"` should be handled.

---

### **2. Allocation of tasks and roles**

Since you have never worked together before, it helps to define clear responsibilities:
**Roles based on strengths**: Consider which of you feels more comfortable with certain areas (e.g. system calls, string parsing, signal handling).
**Component-oriented allocation**: Divide the tasks according to logical modules of the shell. 
Examples:
   **Input handling**: Processing user input and parsing.
   **Command execution**: Execution of commands (e.g. built-ins, external programs, pipes).
   **Redirections**: Dealing with `<`, `>`, `<<`, `>>`.
   **Signal handling**: Implementation of `Ctrl-C`, `Ctrl-D`, `Ctrl-\`.
   **Environment handling**: Processing variables and `$?`.
   **Testing**: Creating tests to make sure everything works as expected.

**Suggested starting division:**

Teammate 1: Command parsing and built-ins (e.g. `cd`, `export`, `unset`).
Teammate 2: Pipes, redirections and signal handling.

You can work on each other's modules later, but a clear division makes it easier to get started.

---

### **3. Technical planning**

Create an overview of the shell's architecture:
**Main Loop**: The main loop of the shell that reads and processes input.
**Parser**: Breaks down the input into commands and arguments, taking quotes and meta characters into account.
**Executor**: Executes the commands (e.g. calling built-ins or external programs).
**Redirection & Pipes**: Handles I/O redirection and connections between processes.
**Signal Handling**: Ensures clean behavior for `Ctrl-C`, `Ctrl-D`, etc.

---

### **4. Versioning and tools**

Use tools that make collaboration easier:
**Git**: Create a shared repository and work in feature branches to minimize conflicts.
Example branches: `parsing`, `redirection`, `pipes`.
**Code Reviews**: Conduct regular code reviews to ensure quality and learn from each other.
*Task Tracking**: Use a tool like Trello, GitHub Issues or Notion to track tasks.

---

### **5. Development workflow**

1. **First steps**: Work together on a basic framework:
Implement a minimally executable shell that reads input and executes a simple command (e.g. `echo`).

2. **Iterative development**: Build the project step by step:
Start with built-ins (`cd`, `pwd`) and then add more complex functions such as pipes and redirections.

3. **Code reviews and tests**:
Write unit tests for the parser, built-ins and command processing.
Test edge cases (e.g. empty input, nesting of quotes).

---

### **6. Communication**

**Regular meetings**: Schedule daily or weekly sync meetings to discuss progress.
**Pair programming**: Work together on the same code for more difficult tasks (e.g. via live share).
**Feedback culture**: Discuss openly what is going well and where you are having difficulties.

---

### **7. Examples of task planning**

#### **Week 1**:
Parser framework: Recognizing commands, arguments and quotes.
Implementation of the first built-ins (`echo`, `cd`).

#### **Week 2**:
Adding pipes and simple I/O redirection (`<`, `>`).
Signal handling for `Ctrl-C`, `Ctrl-D`.

#### **Week 3**:
Extending the parser to include complex redirections (`>>`, `<<`) and handling of `$` and `$?`.
Testing of the existing functions.

#### **Week 4**:
Integration and debugging.
Documentation and fine-tuning.

---

### **8. Tips for collaboration**
**Actively share knowledge**: Explain to each other how you solved problems.
**Respect deadlines**: Agree on deadlines for modules and test your work early.
**Stay flexible**: If an area is too difficult, you can redistribute responsibilities.

With this approach, you can successfully implement the project both technically and collaboratively. Good luck!
*/