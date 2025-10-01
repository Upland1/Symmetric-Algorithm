# Troubleshooting - CRUX-8 Visual

## Problemas de Compilación

### Error: "SFML/Graphics.hpp: No such file or directory"

**Problema**: El compilador no encuentra los headers de SFML.

**Soluciones**:
1. **Verificar ruta de include**:
   ```powershell
   dir SFML-2.6.1\include\SFML
   ```
   Debería mostrar archivos como `Graphics.hpp`, `Window.hpp`, etc.

2. **Usar ruta absoluta**:
   ```powershell
   g++ -I"C:\ruta\completa\a\tu\proyecto\SFML-2.6.1\include" ...
   ```

3. **Verificar estructura de carpetas**:
   ```
   tu-proyecto/
   ├── SFML-2.6.1/
   │   └── include/
   │       └── SFML/
   │           ├── Graphics.hpp
   │           └── ...
   └── crypto_gui_sfml26.cpp
   ```

### Error: "undefined reference to sf::..."

**Problema**: Los headers se encuentran pero no las librerías.

**Soluciones**:
1. **Verificar librerías**:
   ```powershell
   dir SFML-2.6.1\lib
   ```
   Debería mostrar archivos `.a` como `libsfml-graphics.a`

2. **Orden de enlace correcto**:
   ```powershell
   g++ archivo.cpp -L"SFML-2.6.1/lib" -lsfml-graphics -lsfml-window -lsfml-system
   ```

3. **Librerías faltantes**:
   Asegúrate de incluir todas las librerías necesarias:
   - `-lsfml-graphics` (gráficos)
   - `-lsfml-window` (ventanas)
   - `-lsfml-system` (sistema base)

### Error de Versión de GCC

**Problema**: `version 'GLIBCXX_3.4.XX' not found`

**Causa**: Incompatibilidad entre la versión de GCC usada para compilar SFML y tu GCC.

**Solución**:
1. **Verificar tu versión de GCC**:
   ```powershell
   gcc --version
   ```

2. **Descargar SFML compatible**:
   - GCC 13.x → `SFML-2.6.1-windows-gcc-13.1.0-mingw-64-bit.zip`
   - GCC 12.x → `SFML-2.6.1-windows-gcc-12.1.0-mingw-64-bit.zip`
   - GCC 11.x → `SFML-2.6.1-windows-gcc-11.2.0-mingw-64-bit.zip`

## Problemas de Ejecución

### Error: "The program can't start because *.dll is missing"

**Problema**: Faltan DLLs de SFML en tiempo de ejecución.

**Soluciones**:
1. **Copiar DLLs necesarias**:
   ```powershell
   copy "SFML-2.6.1\bin\sfml-graphics-2.dll" .
   copy "SFML-2.6.1\bin\sfml-window-2.dll" .
   copy "SFML-2.6.1\bin\sfml-system-2.dll" .
   ```

2. **Verificar que las DLLs estén en la misma carpeta que el .exe**:
   ```
   crypto_gui_sfml26.exe
   sfml-graphics-2.dll
   sfml-window-2.dll
   sfml-system-2.dll
   ```

3. **Usar librerías estáticas** (alternativa):
   ```powershell
   g++ ... -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -DSFML_STATIC
   ```

### El programa se ejecuta pero no aparece ventana

**Causas posibles**:

1. **Problema de drivers gráficos**:
   - Actualiza los drivers de tu tarjeta gráfica
   - Verifica soporte para OpenGL 2.0+

2. **Programa se cierra inmediatamente**:
   ```powershell
   # Ejecutar desde terminal para ver errores
   ./crypto_gui_sfml26.exe
   ```

3. **Pantalla fuera de rango**:
   - La ventana puede estar apareciendo fuera de tu pantalla
   - Modifica el código para usar resolución más pequeña

### Error: "Failed to create OpenGL context"

**Problema**: Tu sistema no soporta OpenGL o los drivers están desactualizados.

**Soluciones**:
1. **Actualizar drivers gráficos**:
   - NVIDIA: GeForce Experience
   - AMD: AMD Radeon Software
   - Intel: Intel Driver & Support Assistant

2. **Verificar soporte OpenGL**:
   - Ejecuta `dxdiag` y revisa la sección "Display"
   - Busca información sobre OpenGL version

3. **Usar software rendering** (último recurso):
   Modificar código para forzar software rendering

### Problemas con Fuentes

**Síntomas**: Texto no aparece o caracteres extraños.

**Soluciones**:
1. **Los mensajes de error de fuente son normales**:
   ```
   Failed to load font "./CourierNew.ttf" (failed to create the font face)
   ```
   El programa probará múltiples fuentes automáticamente.

2. **Si no carga ninguna fuente**:
   - Verifica que `C:\Windows\Fonts\arial.ttf` existe
   - Proporciona una fuente TTF en la carpeta del proyecto

3. **Caracteres especiales**:
   - Usa solo caracteres ASCII (0-127) para evitar problemas
   - Evita acentos y caracteres especiales en la entrada

## Problemas de Rendimiento

### Programa muy lento

**Soluciones**:
1. **Compilar con optimizaciones**:
   ```powershell
   g++ -O2 -std=c++20 ...
   ```

2. **Verificar antivirus**:
   - Algunos antivirus pueden ralentizar la ejecución
   - Agrega excepción para la carpeta del proyecto

3. **Cerrar programas innecesarios**:
   - Libera memoria RAM
   - Cierra aplicaciones pesadas

### Uso alto de CPU

**Causa**: El bucle principal puede estar consumiendo demasiados recursos.

**Solución**: El programa ya incluye `window.setFramerateLimit(60)` para limitar FPS.

## Problemas Específicos de Windows

### Error de Permisos

**Problema**: "Access denied" al compilar o ejecutar.

**Soluciones**:
1. **Ejecutar terminal como administrador**
2. **Cambiar ubicación del proyecto**:
   - Evita carpetas del sistema como `C:\Program Files`
   - Usa carpetas de usuario como `C:\Users\tu_usuario\Documents`

### Antivirus Bloquea el Ejecutable

**Síntomas**: El .exe se elimina después de compilar.

**Soluciones**:
1. **Agregar excepción en el antivirus**:
   - Windows Defender: Configuración → Protección contra virus → Exclusiones
   - Agregar carpeta del proyecto

2. **Compilar con nombres diferentes**:
   ```powershell
   g++ ... -o mi_programa.exe
   ```

### Problemas con PowerShell

**Error**: "execution of scripts is disabled on this system"

**Solución**:
```powershell
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
```

## Diagnóstico Paso a Paso

### 1. Verificar Compilador
```powershell
gcc --version
g++ --version
where gcc
where g++
```

### 2. Verificar SFML
```powershell
dir SFML-2.6.1
dir SFML-2.6.1\include\SFML
dir SFML-2.6.1\lib
dir SFML-2.6.1\bin
```

### 3. Test de Compilación Mínimo
```cpp
// test_minimal.cpp
#include <iostream>
int main() {
    std::cout << "Test básico OK" << std::endl;
    return 0;
}
```

```powershell
g++ test_minimal.cpp -o test_minimal.exe
./test_minimal.exe
```

### 4. Test SFML Básico
```cpp
// test_sfml_minimal.cpp
#include <SFML/System.hpp>
#include <iostream>
int main() {
    sf::Clock clock;
    std::cout << "SFML funciona!" << std::endl;
    return 0;
}
```

```powershell
g++ -I"SFML-2.6.1/include" test_sfml_minimal.cpp -o test_sfml_minimal.exe -L"SFML-2.6.1/lib" -lsfml-system
./test_sfml_minimal.exe
```

## Obtener Ayuda Adicional

### Información del Sistema
Para reportar problemas, incluye:
```powershell
# Versión de Windows
winver

# Información del compilador
gcc --version

# Información de SFML
dir SFML-2.6.1

# Variables de entorno
echo $env:PATH
```

### Logs de Debug
Para generar logs detallados, modifica el código agregando más mensajes `std::cout` o ejecuta:
```powershell
./crypto_gui_sfml26.exe > log.txt 2>&1
```

### Recursos Adicionales
- **SFML Documentation**: https://www.sfml-dev.org/documentation/2.6.1/
- **MinGW-w64**: https://www.mingw-w64.org/
- **MSYS2**: https://www.msys2.org/

---
*Si el problema persiste, crea un nuevo issue con toda la información del diagnóstico.*