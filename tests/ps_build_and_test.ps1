

try {
    $compiled = get-item ./test_rolling_average.t
    Remove-item $compiled
}
catch {}

try {
    $compiled = get-item ./test_var.t
    Remove-item $compiled
}
catch {}


Write-Output "Building Rolling Average Test..."
g++ test_rolling_average.cpp -o test_rolling_average.t
Write-Output "Running Tests..."
start-process -FilePath ./test_rolling_average.t -Wait -NoNewWindow

Write-Output "Building Var.hpp Test..."
g++ test_var_class.cpp -o test_var.t
Write-Output "Running Tests..."
start-process -FilePath ./test_var.t -Wait -NoNewWindow

