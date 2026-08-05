cl /std:c++latest /utf-8 /Fo:out\debug\ /EHsc /Fd:out\debug\ /Fe:bin\debug\mtti2t /Iinclude /W4 /Zi /Od /MDd /c src\*
lib /OUT:bin\debug\mtti2t.lib out\debug\*.obj
cl /std:c++latest /utf-8 /Fo:out\debug\ /EHsc /Fd:out\debug\ /Fe:bin\debug\test.exe /Iinclude /W4 /Zi /Od /MDd tests\* bin\debug\mtti2t.lib