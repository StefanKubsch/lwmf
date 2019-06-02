RD .\x64\Release\DemoGFX /S /Q
RD .\x64\Release\Shader /S /Q

xcopy .\DemoGFX .\x64\Release\DemoGFX\ /S
xcopy .\Shader .\x64\Release\Shader\ /S
pause