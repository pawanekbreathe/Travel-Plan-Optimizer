# PreToolUse hook: block destructive delete commands
# Exits with code 2 and outputs JSON to block execution

$rawInput = [Console]::In.ReadToEnd()
if (-not $rawInput.Trim()) { exit 0 }

try {
    $json     = $rawInput | ConvertFrom-Json
    $command  = $json.tool_input.command

    $blocked = @('rm -rf', 'del /f', 'rmdir /s')
    foreach ($pattern in $blocked) {
        if ($command -match [regex]::Escape($pattern)) {
            @{
                continue   = $false
                stopReason = "BLOCKED: '$pattern' is a destructive command that is not permitted in this project."
            } | ConvertTo-Json -Compress | Write-Output
            exit 2
        }
    }
} catch {
    exit 0
}
