# PostToolUse hook: log timestamp, tool name, and file path to .claude/tool-log.txt

$rawInput = [Console]::In.ReadToEnd()
if (-not $rawInput.Trim()) { exit 0 }

try {
    $json      = $rawInput | ConvertFrom-Json
    $toolName  = $json.tool_name

    # Resolve file path from whichever field is present
    $filePath  = $json.tool_input.file_path
    if (-not $filePath) { $filePath = $json.tool_input.path }
    if (-not $filePath) { $filePath = $json.tool_input.pattern }
    if (-not $filePath) { $filePath = $json.tool_input.command }
    if (-not $filePath) { $filePath = "(none)" }

    $timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $logLine   = "$timestamp | $toolName | $filePath"
    $logPath   = ".claude/tool-log.txt"

    Add-Content -Path $logPath -Value $logLine -Encoding UTF8
} catch {
    exit 0
}
