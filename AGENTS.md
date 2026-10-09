# Environment

- The development environment is Windows 11.
- The shell is PowerShell.
- Always use PowerShell-compatible commands.
- Never use Unix/Linux shell commands such as:
  - find
  - grep
  - sed
  - awk
  - head
  - tail
  - ls
- Prefer native PowerShell equivalents:
  - Get-ChildItem instead of find/ls
  - Select-String instead of grep
  - Get-Content instead of cat/head/tail
  - Where-Object for filtering
  - Select-Object for selecting properties/results

# Tool usage

- Before running a shell command, verify that it is valid PowerShell syntax.
- Do not assume bash is available.
- When a command fails because of shell incompatibility, switch immediately to a PowerShell-native solution.
- Prefer OpenCode's built-in file reading/search tools over shell commands when possible.

# Coding behavior

- Inspect relevant files before making changes.
- Make the smallest necessary change.
- Do not invent contents of files that were not inspected.
- If a file is empty, explicitly treat it as empty.
- After code changes, compile or run relevant tests when possible.
- After finishing a Task, give a detailed Summary about the Changes. 