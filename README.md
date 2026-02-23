# Employee Database 員工資料庫系統 <br> A System Focused Look --> https://hackmd.io/pD5_OFWoQz6D9crf0KyMhQ

Implemented a custom binary employee database in C using a structured 
ondisk file format with explicit big endian encoding for portability. 
The system uses low level POSIX I/O (read, write, lseek, ftruncate) and 
dynamic memory management to safely manage records. 
File integrity is validated via a magic number, versioning, and size 
checks to detect corruption.

使用 C 語言實作的自訂二進位員工資料庫系統，採用結構化磁碟檔案格式，
並使用明確的 Big Endian 編碼以確保跨平台可攜性。
系統透過低階 POSIX I/O（read、write、lseek、ftruncate）
與動態記憶體管理（realloc）安全地管理資料紀錄。
檔案完整性透過 magic number、版本號與大小檢查來驗證，以偵測檔案損毀。
------------------------------------------------------------------------

## Overview 專案概述

This is a lightweight database style cli tool designed for
learning and practicing:

本專案是一個輕量級資料庫風格的命令列工具，主要用於學習與練習：

-   Command line argument parsing (`getopt`)
-   File I/O (`open`, `read`, `write`)
-   Binary file persistence
-   Dynamic memory management (`malloc`, `realloc`, `free`)
-   Defensive programming
-   Clean modular project structure

The project has no external dependencies and builds using `gcc`.

本專案無外部相依套件，使用 gcc 即可編譯。

------------------------------------------------------------------------

## Usage 使用方式

Basic syntax 基本語法：

``` bash
./bin/dbview [OPTIONS]
```

Operations require a database file to be specified using `-f`.

Example 範例：

``` bash
./bin/dbview -f employees.db -a "John Doe,Manhatten Street,900"
```

------------------------------------------------------------------------

## Commands 指令說明

| Option | Description | 說明 |
|--------|------------|------|
| `-n` | Initialize a new database file | 建立新的資料庫檔案 |
| `-f <file>` | Specify database file (required) | 指定資料庫檔案（必填） |
| `-a "<name>,<address>,<hours>"` | Add employee record | 新增員工資料 |
| `-u "<name>,<hours>"` | Update employee hours | 更新員工工時 |
| `-r "<name>"` | Remove employee by name | 依姓名刪除員工 |
| `-l` | List all employees | 列出所有員工 |

## Examples 使用範例

Create a new database 建立新的資料庫：

``` bash
./bin/dbview -n -f employees.db
```

Add a record 新增紀錄：

``` bash
./bin/dbview -f employees.db -a "Alice,President Street,750"
```

List all records 列出所有紀錄：

``` bash
./bin/dbview -f employees.db -l
```

Update a record 更新紀錄：

``` bash
./bin/dbview -f employees.db -u "Alice,800"
```

Remove a record 刪除紀錄：

``` bash
./bin/dbview -f employees.db -r "Alice"
```

------------------------------------------------------------------------

## Build 編譯方式

### Manual Compilation 手動編譯

``` bash
gcc -Wall -Wextra -g source/*.c -Iinclude -o bin/bin/dbview
```

### 使用 Makefile

Build 編譯:

``` bash
make
```

Run 執行:

``` bash
make run
```

Clean build artifacts 清除編譯產物:

``` bash
make clean
```

------------------------------------------------------------------------

## Project Structure 專案結構

    .
    ├── include/
    │   ├── common.h
    │   ├── file.h
    │   └── parse.h
    ├── source/
    │   ├── main.c
    │   ├── file.c
    │   └── parse.c
    ├── obj/
    ├── bin/
    ├── Makefile
    └── README.md

------------------------------------------------------------------------

## Memory & System Inspection 記憶體與系統檢測工具

-	Valgrind  記憶體洩漏檢查工具
-   AddressSanitizer 記憶體錯誤偵測工具
-   Strace 系統呼叫追蹤工具
-   ldd 動態函式庫相依性檢查
-   readelf ELF 檔案分析工具

------------------------------------------------------------------------

## License

This project is for educational purposes.

本專案僅供教育與學習用途。

Also Check (Short Blog): https://hackmd.io/pD5_OFWoQz6D9crf0KyMhQ
