## Hello
it's my c++ project with OpenCl for Matrix multiplication.

For run project you should have OpenCL-sdk

My project run on Windows
Download: \
[Start on Windows](https://github.com/KhronosGroup/OpenCL-Guide/blob/main/chapters/getting_started_windows.md) \
Or:
```bash
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
```

```bash
.\vcpkg.exe --triplet=x64-windows install opencl
```

### Connect OpenCL to the project
Project → Properties

C/C++ → General → Additional Include Directories (add path to directory CL) (C:\Program Files (x86)\Intel\OpenCL SDK\include)

Linker → General → Additional Library Directories (add path to directory lib) (C:\Program Files (x86)\Intel\OpenCL SDK\lib\)

Linker → Input → Additional Dependencies (add: OpenCL.lib) 
