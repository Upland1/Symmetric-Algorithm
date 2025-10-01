# Instalación Completa - CRUX-8 Visual para Windows

## Prerrequisitos

### 1. Verificar Sistema
- **OS**: Windows 10 o superior
- **Arquitectura**: 64 bits (recomendado)
- **Espacio libre**: Al menos 500 MB

### 2. Instalar MinGW-w64
Si no tienes MinGW instalado:

#### Opción A: MSYS2 (Recomendado)
1. Descarga MSYS2 desde: https://www.msys2.org/
2. Instala MSYS2 siguiendo las instrucciones
3. Abre terminal MSYS2 y ejecuta:
   ```bash
   pacman -S mingw-w64-x86_64-gcc
   pacman -S mingw-w64-x86_64-make
   ```

#### Opción B: MinGW-w64 Standalone
1. Descarga desde: https://www.mingw-w64.org/downloads/
2. Instala en `C:\mingw64\`
3. Agrega `C:\mingw64\bin` al PATH del sistema

### 3. Verificar Instalación
Abre PowerShell o CMD y verifica:
```powershell
gcc --version
g++ --version
```
Deberías ver algo como: `gcc.exe (Rev2, Built by MSYS2 project) 13.2.0`

## Instalación de SFML 2.6.1

### Descargar SFML Compatible
1. Identifica tu versión de GCC: `gcc --version`
2. Descarga la versión correspondiente:

**Para GCC 13.x:**
```powershell
Invoke-WebRequest -Uri "https://github.com/SFML/SFML/releases/download/2.6.1/SFML-2.6.1-windows-gcc-13.1.0-mingw-64-bit.zip" -OutFile "SFML-2.6.1-gcc13.zip"
```

**Para GCC 12.x:**
```powershell
Invoke-WebRequest -Uri "https://github.com/SFML/SFML/releases/download/2.6.1/SFML-2.6.1-windows-gcc-12.1.0-mingw-64-bit.zip" -OutFile "SFML-2.6.1-gcc12.zip"
```

### Extraer SFML
```powershell
Expand-Archive -Path "SFML-2.6.1-gcc13.zip" -DestinationPath "." -Force
```

### Copiar DLLs Necesarias
```powershell
copy "SFML-2.6.1\bin\*.dll" .
```

## Compilación del Proyecto

### 1. Preparar Directorio
Crea una carpeta para tu proyecto:
```powershell
mkdir CRUX-8-Visual
cd CRUX-8-Visual
```

### 2. Configurar VS Code (Opcional)
Si usas VS Code, crea `.vscode\c_cpp_properties.json`:
```json
{
  "configurations": [
    {
      "name": "windows-gcc-x64",
      "includePath": [
        "${workspaceFolder}/**",
        "${workspaceFolder}/SFML-2.6.1/include"
      ],
      "compilerPath": "gcc",
      "cStandard": "${default}",
      "cppStandard": "c++20",
      "intelliSenseMode": "windows-gcc-x64"
    }
  ],
  "version": 4
}
```

Y `.vscode\tasks.json`:
```json
{
    "tasks": [
        {
            "type": "cppbuild",
            "label": "C/C++: g++.exe build active file with SFML",
            "command": "g++",
            "args": [
                "-fdiagnostics-color=always",
                "-g",
                "-std=c++20",
                "-I${workspaceFolder}/SFML-2.6.1/include",
                "${file}",
                "-o",
                "${fileDirname}\\${fileBasenameNoExtension}.exe",
                "-L${workspaceFolder}/SFML-2.6.1/lib",
                "-lsfml-graphics",
                "-lsfml-window",
                "-lsfml-system"
            ],
            "options": {
                "cwd": "${fileDirname}"
            },
            "problemMatcher": ["$gcc"],
            "group": {
                "kind": "build",
                "isDefault": true
            }
        }
    ],
    "version": "2.0.0"
}
```

### 3. Compilar desde Terminal
#### Comando Básico
```powershell
g++ -std=c++20 -I"SFML-2.6.1/include" crypto_gui_sfml26.cpp -o crypto_gui_sfml26.exe -L"SFML-2.6.1/lib" -lsfml-graphics -lsfml-window -lsfml-system
```

#### Comando con Debug
```powershell
g++ -fdiagnostics-color=always -g -std=c++20 -I"SFML-2.6.1/include" crypto_gui_sfml26.cpp -o crypto_gui_sfml26.exe -L"SFML-2.6.1/lib" -lsfml-graphics -lsfml-window -lsfml-system
```

#### Comando Release (Optimizado)
```powershell
g++ -O2 -std=c++20 -I"SFML-2.6.1/include" crypto_gui_sfml26.cpp -o crypto_gui_sfml26.exe -L"SFML-2.6.1/lib" -lsfml-graphics -lsfml-window -lsfml-system
```

## Verificación de Instalación

### 1. Probar Compilación Básica
Crea un archivo `test.cpp`:
```cpp
#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    std::cout << "SFML Test - Creando ventana..." << std::endl;
    sf::RenderWindow window(sf::VideoMode(400, 300), "Test SFML");
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        window.clear(sf::Color::Blue);
        window.display();
    }
    
    std::cout << "Test completado exitosamente!" << std::endl;
    return 0;
}
```

Compila y ejecuta:
```powershell
g++ -std=c++20 -I"SFML-2.6.1/include" test.cpp -o test.exe -L"SFML-2.6.1/lib" -lsfml-graphics -lsfml-window -lsfml-system
./test.exe
```

### 2. Verificar DLLs
Asegúrate de que estos archivos estén en tu directorio:
```
sfml-graphics-2.dll
sfml-window-2.dll
sfml-system-2.dll
```

### 3. Ejecutar CRUX-8 Visual
```powershell
./crypto_gui_sfml26.exe
```

## Estructura Final del Proyecto
```
CRUX-8-Visual/
├── crypto_gui_sfml26.exe      # Ejecutable principal
├── crypto_gui_sfml26.cpp      # Código fuente
├── SFML-2.6.1/               # Librerías SFML
│   ├── bin/
│   ├── include/
│   └── lib/
├── sfml-graphics-2.dll        # DLLs necesarias
├── sfml-window-2.dll
├── sfml-system-2.dll
├── .vscode/                   # Configuración VS Code (opcional)
│   ├── c_cpp_properties.json
│   └── tasks.json
└── README.md                  # Documentación
```

## Distribución del Ejecutable

### Para Distribuir a Otros Usuarios
Si quieres compartir tu programa compilado:

1. **Archivos Necesarios**:
   ```
   crypto_gui_sfml26.exe
   sfml-graphics-2.dll
   sfml-window-2.dll
   sfml-system-2.dll
   ```

2. **Empaquetado**:
   ```powershell
   # Crear carpeta de distribución
   mkdir CRUX-8-Distribucion
   copy crypto_gui_sfml26.exe CRUX-8-Distribucion\
   copy *.dll CRUX-8-Distribucion\
   copy README.md CRUX-8-Distribucion\
   
   # Crear ZIP
   Compress-Archive -Path "CRUX-8-Distribucion\*" -DestinationPath "CRUX-8-Visual-v1.0.zip"
   ```

3. **Instrucciones para el Usuario Final**:
   - Extraer todos los archivos en una carpeta
   - Ejecutar `crypto_gui_sfml26.exe`
   - No se requiere instalación adicional

## Solución de Problemas Comunes

### Error: "No such file or directory"
- Verifica que las rutas a SFML sean correctas
- Usa rutas absolutas si es necesario

### Error: "DLL not found"
- Asegúrate de que las DLLs estén en la misma carpeta que el .exe
- Verifica que descargaste la versión correcta de SFML para tu GCC

### Ventana no aparece
- Verifica que tienes drivers gráficos actualizados
- Prueba el programa de test básico primero

### Error de compilación
- Verifica la versión de GCC: debe ser compatible con la versión de SFML
- Asegúrate de usar `-std=c++20` en el comando de compilación

---
*Esta guía cubre la instalación completa desde cero. Para problemas específicos, consulta `TROUBLESHOOTING.md`*