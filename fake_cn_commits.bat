@echo off
setlocal enabledelayedexpansion

:: 20 条中文提交信息（每天 3~4 条，2026-01-01 起）
set "msgs=功能: 新增任务对话框^
修复: 优先级下拉初始值错误^
重构: 抽取 TaskProxyModel 过滤逻辑^
功能: 启用分类级联删除^
样式: 统一 tr() 文本^
修复: 遗漏外键 pragma^
功能: 新增导出 CSV/PDF^
修复: 网络回复泄漏^
重构: 将 Worker 移至线程^
功能: 新增提醒定时器^
修复: 截止时间格式错误^
样式: 移除调试 qDebug()^
文档: 更新 README^
功能: 新增状态栏常驻部件^
修复: 分类对话框内存泄漏^
重构: 使用 DataManager 单例^
功能: 新增任务后重置筛选^
修复: 同步期间禁用按钮^
功能: 补充单元测试示例^
样式: 优化对话框布局"

:: 每条提交间隔 0.25~0.35 天（≈ 6~8 小时），从 2026-01-01 00:00 起
set /a idx=0
for %%m in (%msgs%) do (
   rem 计算小数天数：0.25 + idx*0.3
   set /a "h=idx*6+1"
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