Compile $ARGUMENTS using gcc and report the result.

If no file is specified, look for the .c file in the current project (Travel-Plan-Optimizer-main/).

Steps:
1. Run: gcc "$ARGUMENTS" -o travel_planner -Wall -Wextra -std=c99
   (wrap the filename in quotes to handle spaces)
2. If compilation succeeds (exit code 0):
   - Report "Build successful" and the output binary name
   - Show any warnings that were emitted even on success
3. If compilation fails:
   - Show the full compiler error output
   - Group errors by category: syntax errors, type errors, undeclared identifiers, linker errors
   - For each error explain what it means in plain English and suggest a fix
   - Do NOT automatically edit the file; only report and explain
