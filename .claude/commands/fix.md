---
model: opus
---
Fix all compiler warnings in $ARGUMENTS

If no file is specified, look for the .c file in the current project (Travel-Plan-Optimizer-main/).

Steps:
1. Read the file first.
2. Run: gcc "$ARGUMENTS" -o travel_planner -Wall -Wextra -std=c99
   Capture all warnings (lines containing "warning:").
3. If there are no warnings, report "No warnings found — file is clean." and stop.
4. For each warning:
   - Identify the root cause
   - Apply the minimal fix that resolves the warning without changing behaviour
   - Obey all coding standards in CLAUDE.md (NULL checks, bounds guards, strncpy pattern, etc.)
   - Do NOT suppress warnings with pragmas or casts unless there is genuinely no other option
5. After all edits, run gcc again to confirm zero warnings remain.
6. Report a summary: how many warnings were fixed, and one line per fix describing what changed and why.
