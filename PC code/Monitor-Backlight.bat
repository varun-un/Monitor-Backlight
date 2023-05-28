for /F "delims=" %%f in ('dir   /b /od /a-d "%*" ') do set File=%%f
if NOT %*\%File%=="\main.exe" g++ main.cpp SerialPort.cpp -lgdi32 -o main.exe
.\main.exe