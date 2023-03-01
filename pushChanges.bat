@echo off
set /p version=Enter new version:
git pull
git add .
git commit -m "%version%"
git push
exit