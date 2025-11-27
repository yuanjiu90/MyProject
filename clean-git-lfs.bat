@echo off
chcp 65001 > nul
cls

REM 检查git是否在运行
tasklist /FI "IMAGENAME eq git.exe" 2>NUL | find /I /N "git.exe">NUL
if "%ERRORLEVEL%"=="0" (
    echo ====================================
    echo    Git 正在运行中，请稍后再试！
    echo ====================================
    echo.
    echo 提示：请等待其他 Git 操作完成后再运行此脚本。
    echo.
    pause
    exit /b 1
)

echo ====================================
echo    Git LFS 缓存清理工具
echo ====================================
echo.
echo 请选择要执行的清理操作:
echo.
echo 1. 安全清理模式：清理已不再使用的LFS文件
echo 2. 彻底清理模式：完全删除所有LFS缓存文件
echo 9. 超级清理模式：清理所有LFS缓存和未跟踪文件，请谨慎使用！
echo.
echo 温馨提示: 
echo 安全清理模式：只清理确认不再使用的缓存文件，释放空间较少
echo 彻底清理模式：清理全部缓存，包括Stash的内容，可以释放大量空间，少部分必要缓存会在下次拉取时重新下载
echo 超级清理模式：将清理所有缓存并删除所有未跟踪的文件，还原成刚clone下来时的初始状态，请谨慎使用！
echo.

set /p choice=请输入数字 (1、2 或 9) 并按回车: 

if "%choice%"=="1" (
    echo.
    echo 正在执行安全清理...
    echo.
    git lfs prune
    echo.
    echo 清理完成！
) else if "%choice%"=="2" (
    echo.
    echo 正在执行彻底清理...
    echo.
    echo 清理中... 请耐心等待清理完成的提示...
    rd /s /q .git\lfs\objects
    echo.
    echo 清理完成！
) else if "%choice%"=="9" (
    echo.
    echo 开始执行超级清理...
    echo.
    echo 第一步：清理 LFS 缓存，请耐心等待...
    rd /s /q .git\lfs\objects
    echo.
    echo 第二步：清理未跟踪文件...
    git clean -fdx
    echo.
    echo 超级清理完成！
) else (
    echo.
    echo 输入无效！请输入 1、2 或 9
    pause
    exit /b 1
)

echo.
echo 清完缓存一时爽，每天清，每天爽，一直清，一直爽~~~
echo.
echo 按任意键退出...
pause > nul 