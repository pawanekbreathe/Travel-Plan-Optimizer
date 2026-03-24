Use the code-reviewer agent to review $ARGUMENTS

If no file is specified, ask the user which file to review.

The review must cover:
- Memory safety (malloc NULL checks, buffer overflows, memory leaks, use-after-free)
- Security vulnerabilities (unchecked scanf returns, unbounded string ops, integer overflow)
- Logic errors and undefined behaviour
- Code quality and style (naming, formatting, duplication, magic numbers)
- Performance concerns
- Violations of the coding standards in CLAUDE.md

Present findings in a prioritised table: Critical → High → Medium → Low.
For each issue include: severity, line number(s), description, and a concrete fix.
