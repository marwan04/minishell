# Minishell: A Guided Deep Dive into Building Your Own Shell

## Overview

This guide walks you through the development process of Minishell, a project inspired by the Bash shell and implemented as part of the 42 Network curriculum. By the end of this guide, you'll understand the components of a shell, how to implement them in C, and how to manage complex behaviors like piping, redirection, environment expansion, and signal handling.

## 🧩 What You'll Build

A custom command-line shell that supports:

- Built-in commands like `cd`, `echo`, `pwd`, `export`, `unset`, `env`, `exit`
- Environment variable expansion
- Pipes and redirection
- Signal handling (`Ctrl+C`, `Ctrl+\`)
- Heredocs (`<<`)
- Wildcards (`*`)

## 🔧 How to Structure the Project

minishell/
├── builtins/ # Internal commands
├── exec/ # Execution engine
├── parser/ # Tokenizer and AST builder
├── expander/ # Quote and env handling
├── heredoc/ # Heredoc logic
├── signals/ # SIGINT, SIGQUIT
├── utils/ # Helpers and memory management
├── includes/ # All headers
├── main.c
└── Makefile

## 🗺️ Step-by-Step Guide

### Step 1: Tokenizer (Lexer)

**Goal:** Turn user input into tokens.

**Input:**
echo "$USER" && ls -l | grep foo > out

**Output:** A linked list of tokens like:
[ECHO] ["$USER"] [AND] [LS] [-l] [PIPE] [GREP] [foo] [REDIR_OUT] [out]

**Tips:**

- Handle quoting: `'`, `"`, and escape sequences
- Support heredocs (`<<`) and special operators (`&&`, `||`)

---

### Step 2: Syntax Validation

**Goal:** Detect malformed sequences

**Examples to catch:**

- `ls ||` (missing command)
- `echo >` (missing file)
- `cat < < file` (double input redirection)

---

### Step 3: AST (Abstract Syntax Tree)

**Goal:** Convert token list into a tree of operations

**Why?** It lets you execute commands in correct logical order.

**Nodes:**

- `NODE_PIPE`
- `NODE_CMD`
- `NODE_AND`, `NODE_OR`
- `NODE_REDIR_IN`, `NODE_REDIR_OUT`, `NODE_APPEND`, `NODE_HEREDOC`

**Resources:**

- How Bash Builds Command Trees

---

### Step 4: Execution Engine

**Goal:** Recursively traverse AST and execute logic

**Example:**
ls | grep .c && echo "Success" || echo "Fail"

**Must Handle:**

- Forking child processes for each pipe stage
- Redirection of `stdin`/`stdout`
- Command path resolution using `PATH`
- Builtins in parent vs child logic

---

### Step 5: Built-in Commands

**Special Case:** Built-ins like `cd`, `exit`, and `export` need to modify the parent process.

**Strategy:**

- Detect whether to run them in parent (no pipes) or in child (within a pipeline)

---

### Step 6: Heredocs

**Goal:** Capture multiline input

**Example:**
cat << END
Hello
World
END

**Key Points:**

- Fork to handle heredoc input
- Set custom `SIGINT` handler
- Store heredoc result in pipe
- Decide whether to expand variables based on quoting

---

### Step 7: Environment Variable Expansion

**Example:**
echo $HOME "'$USER'" "$PATH"

**Steps:**

- Tilde expansion (`~`)
- `$?` for exit status
- `$VAR` and `${VAR}` handling

**Edge Case:**

- `echo "$"`  → should print `$`
- `echo $$$$` → should be handled correctly

---

### Step 8: Signal Handling

**Required Behavior:**

- `Ctrl+C`: should reset prompt without quitting
- `Ctrl+\`: should be ignored
- Heredoc: `Ctrl+C` should not execute the command

**Tip:** Use `sigaction()` and a `volatile sig_atomic_t` global flag

---

### Step 9: Wildcards (Bonus)

**Goal:** Expand `*` to match files in directory

**Example:**
ls *.c

**Use:** `fnmatch()` with `readdir()` to match manually

---

## ✅ Key Edge Cases

- `echo $HOME'$PATH'"$USER"` → should expand selectively
- `export VAR="value with spaces"` → should remain intact
- `cat << EOF | grep something` → heredoc must precede pipe
- Multiple heredocs in one command
- `$""`, `""`, empty expansions

---

## 🧪 How to Test

**Use:**

- `valgrind --leak-check=full ./minishell`
- `lsof -p $(pgrep minishell)` → to check FD leaks

**Try these cases:**

ls | grep .c > files.txt && cat files.txt
export A="Hello" && echo $A
cat << EOF | wc -l
Line1
Line2
EOF
echo '$USER' "$USER" $USER

---

## 🧠 What You’ll Learn

- Process control (`fork`, `execve`, `waitpid`)
- FD management with `dup2`/`pipe`
- Abstract syntax trees and recursive evaluation
- Environment and memory safety in C
- Signal programming and real-world edge case handling

---

## 📚 Resources

- GNU Bash Manual
- `man 2 execve`
- Signal Programming
- 42 Norminette
- `fnmatch()` - Wildcard Matching

---

## 👥 Authors

**Eyad** – AST, syntax handling, signals, pipeline management  
**Marwan** – Expander, tokenizer, redirection & wildcard support

Together, we tackled every corner case and built a fully functional shell that respects all 42 subject constraints and mimics the Bash behavior under the hood.

---

## 🎉 Closing Notes

This project marks a major milestone in our 42 journey. It wasn't just about building a shell, but learning:

- How real Unix shells work
- How to structure large-scale projects in C
- How to collaborate deeply on complex problem-solving

Thanks for reading, and feel free to fork this repo or reach out with questions if you're starting your own Minishell journey!
