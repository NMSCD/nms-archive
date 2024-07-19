@echo off
set /p version=Enter new version:
git pull
git add .
echo staged!
git commit -m "%version%"
echo committed!
git push
exit