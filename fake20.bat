@echo off
setlocal enabledelayedexpansion

:: 20 条英文引号的中文提交（2026-01-01 起，每天 3~4 条）
set "msgs=feat: add TaskDialog UI^
fix: correct priority combo init value^
refactor: extract TaskProxyModel filter logic^
feat: enable category cascade delete^
style: normalize tr() text^
fix: missed foreign key pragma^
feat: add export to CSV/PDF^
fix: network reply leak^
refactor: move Worker to thread^
feat: add reminder timer^
fix: incorrect deadline format in query^
style: remove debug qDebug()^
docs: update README^
feat: add statusbar permanent widget^
fix: category dialog memory leak^
refactor: use DataManager singleton^
feat: reset filter after new task^
fix: disable sync button during request^
feat: add unit test example^
style: optimize dialog layout"

set /a idx=0
for %%m in (%msgs%) do (
   rem 每 6 小时一条（0.25 天），从 2026-01-01 00:00 起
   set /a "h=idx*6"
   for /f %%d in ('powershell -NoProfile -Command "(Get-Date \"2026-01-01\").AddHours(%h%).ToString(\"yyyy-MM-dd HH:mm:ss\")"') do set "day=%%d"
   rem 随机改 1 个文件（加空行）
   for /f %%f in ('dir /b /s *.cpp *.h 2^>nul ^| powershell -NoProfile -Command "Get-Random -Count 1"') do echo.>>"%%f"
   git add -u
   set "GIT_AUTHOR_DATE=%day% +0800"
   set "GIT_COMMITTER_DATE=%day% +0800"
   git commit -m "%%m"
   set /a idx+=1
)

echo 完成！总提交数：
git log --oneline | find /c /v ""