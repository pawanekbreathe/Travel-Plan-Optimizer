# PostToolUse hook: run clang-format on .c and .h files after Write or Edit

$rawInput = [Console]::In.ReadToEnd()
if (-not $rawInput.Trim()) { exit 0 }

try {
    $json     = $rawInput | ConvertFrom-Json
    $filePath = $json.tool_input.file_path

    if ($filePath -and ($filePath -match '\.(c|h)$')) {
        & clang-format -i $filePath 2>$null
    }
} catch {
    exit 0
}
