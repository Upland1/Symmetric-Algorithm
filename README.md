# CRUX-8 Visual - Documentación para Windows

## Descripción
CRUX-8 Visual es una aplicación gráfica que demuestra visualmente el funcionamiento del algoritmo de cifrado simétrico CRUX-8. Permite ver paso a paso cómo se generan las subclaves, cómo se cifra y descifra información, mostrando todas las operaciones internas del algoritmo.

## Características
- Interfaz gráfica intuitiva con SFML 2.6.1
- Visualización paso a paso del algoritmo CRUX-8
- Generación y visualización de subclaves
- Proceso completo de cifrado y descifrado
- Navegación interactiva entre pasos
- Modo autoplay automático
- Compatible con Windows 10/11

## Requisitos del Sistema

### Software Necesario
- **Windows 10 o superior**
- **MinGW-w64 con GCC 13.x** (recomendado)
- **Git** (opcional, para clonar el repositorio)

### Hardware Mínimo
- RAM: 2 GB
- Espacio en disco: 100 MB
- Tarjeta gráfica compatible con OpenGL 2.0

## Instalación Rápida

### Opción 1: Ejecutable Precompilado (Recomendado)
1. Descarga el archivo `crypto_gui_sfml26.exe`
2. Descarga todas las DLLs de SFML (*.dll)
3. Coloca todos los archivos en la misma carpeta
4. Ejecuta `crypto_gui_sfml26.exe`

### Opción 2: Compilar desde Código Fuente
Sigue la guía detallada en `INSTALACION_COMPLETA.md`

## Uso Básico

### Iniciar el Programa
1. Abre una terminal (PowerShell o CMD) en la carpeta del programa
2. Ejecuta: `./crypto_gui_sfml26.exe`
3. Se abrirá la ventana de la aplicación

### Interfaz de Usuario
```
┌─────────────────────────────────────────────────┐
│ CRUX-8 Visual - Introduce clave y texto        │
├─────────────────────────────────────────────────┤
│ Clave (16 chars): [________________]           │
│ Texto (<=8 chars): [________]                  │
├─────────────────────────────────────────────────┤
│ Estado:                    │ Subclaves:         │
│ [Panel de información]     │ [Lista de claves]  │
│                           │                    │
├─────────────────────────────────────────────────┤
│ ENTER=iniciar | SPACE=siguiente | ESC=salir    │
└─────────────────────────────────────────────────┘
```

### Controles
- **ENTER**: Iniciar el proceso de cifrado/descifrado
- **SPACE**: Avanzar al siguiente paso
- **BACKSPACE**: Retroceder al paso anterior
- **A**: Activar/desactivar modo autoplay
- **TAB**: Cambiar entre campo de clave y texto
- **ESC**: Salir del programa
- **↑/↓**: Hacer scroll en el panel de información

### Flujo de Trabajo
1. **Introducir Clave**: Escribe exactamente 16 caracteres
2. **Introducir Texto**: Escribe hasta 8 caracteres a cifrar
3. **Iniciar**: Presiona ENTER
4. **Navegar**: Usa SPACE para ver cada paso del algoritmo
5. **Observar**: El programa muestra:
   - Generación de 8 subclaves
   - 8 rondas de cifrado
   - 8 rondas de descifrado
   - Texto plano recuperado

## Ejemplos de Uso

### Ejemplo 1: Cifrado Básico
```
Clave: "mi_clave_secreta"  (16 caracteres)
Texto: "hola"              (4 caracteres)
```

### Ejemplo 2: Texto Completo
```
Clave: "1234567890abcdef"  (16 caracteres)
Texto: "mensaje!"          (8 caracteres)
```

## Estructura de Archivos
```
CRUX-8/
├── crypto_gui_sfml26.exe    # Ejecutable principal
├── crypto_gui_sfml26.cpp    # Código fuente
├── SFML-2.6.1/             # Librerías SFML
│   ├── bin/                 # DLLs necesarias
│   ├── include/             # Headers (solo para compilar)
│   └── lib/                 # Librerías estáticas
├── *.dll                    # DLLs de SFML copiadas
├── README.md                # Este archivo
├── INSTALACION_COMPLETA.md  # Guía de instalación
└── TROUBLESHOOTING.md       # Solución de problemas
```

## Algoritmo CRUX-8

### Visión General
CRUX-8 es un algoritmo de cifrado simétrico de 64 bits que:
1. Genera 8 subclaves de 64 bits cada una
2. Realiza 8 rondas de cifrado
3. Cada ronda incluye XOR, rotaciones y suma
4. El descifrado invierte el proceso

### Generación de Subclaves
- K0, K1: Primeras dos claves de la clave maestra (16 bytes)
- K2-K7: Generadas usando rotaciones, XOR y constante mágica

### Rondas de Cifrado
Para cada ronda i (0-7):
1. XOR con subclave Ki
2. Dividir en L (32 bits altos) y R (32 bits bajos)
3. Rotar L y R según desplazamiento de Ki
4. Combinar resultados
5. Sumar subclave Ki

## Soporte Técnico

### Problemas Comunes
- **Error de fuente**: Normal, el programa probará varias fuentes automáticamente
- **Ventana no aparece**: Verificar que las DLLs estén en la misma carpeta
- **Error de compilación**: Verificar versión de GCC (debe ser 13.x)

### Archivos de Ayuda
- `TROUBLESHOOTING.md`: Solución de problemas específicos
- `INSTALACION_COMPLETA.md`: Instalación paso a paso detallada

## Autor y Licencia
Desarrollado como proyecto educativo para demostrar algoritmos de criptografía simétrica.

**Uso**: Educativo y de investigación
**Plataforma**: Windows con MinGW/GCC
**Gráficos**: SFML 2.6.1

---
*Para más información técnica, consulta los archivos de documentación adicionales.*