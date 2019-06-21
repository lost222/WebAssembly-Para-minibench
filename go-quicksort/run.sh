go build quicksort.go 
./quicksort

GOARCH=wasm GOOS=js go build -o test.wasm quicksort.go
emrun wasm_exec.html
