# MANUAL DE USUARIO - CRUX-8 Visual

## Introducción
CRUX-8 Visual es una herramienta educativa que permite visualizar paso a paso el funcionamiento del algoritmo de cifrado simétrico CRUX-8. Esta guía te enseñará cómo usar la aplicación de manera efectiva.

## Inicio Rápido

### 1. Ejecutar el Programa
- Abre PowerShell o CMD en la carpeta del programa
- Ejecuta: `./crypto_gui_sfml26.exe`
- Se abrirá la ventana de la aplicación

### 2. Primera Ejecución
Al iniciar verás mensajes en la terminal como:
```
Iniciando programa CRUX-8 Visual SFML 2.6.1...
Creando ventana...
Ventana creada exitosamente.
Intentando cargar fuente: ./CourierNew.ttf
Failed to load font "./CourierNew.ttf" (failed to create the font face)
Intentando cargar fuente: C:\Windows\Fonts\arial.ttf
Fuente cargada exitosamente: C:\Windows\Fonts\arial.ttf
Entrando al bucle principal...
```

**Nota**: Los mensajes de error de fuente son normales. El programa probará varias fuentes hasta encontrar una compatible.

## Interfaz de Usuario

### Diseño de la Ventana
```
┌─────────────────────────────────────────────────────────────────────┐
│ CRUX-8 Visual - Introduce clave y texto                            │
├─────────────────────────────────────────────────────────────────────┤
│ Clave (16 chars): [mi_clave_secreta_]                             │ 
│ Texto (<=8 chars): [mensaje_]                                      │
├─────────────────────────────┬───────────────────────────────────────┤
│ Estado:                     │ Subclaves:                            │
│                            │                                       │
│ [Panel de información       │ k[0]=0x1234567890abcdef             │
│  sobre el paso actual]      │ k[1]=0xfedcba0987654321             │
│                            │ k[2]=0x...                           │
│                            │ ...                                   │
│                            │                                       │
├─────────────────────────────┴───────────────────────────────────────┤
│ ENTER=iniciar | SPACE=siguiente | BACKSPACE=anterior | ESC=salir   │
│ Paso 1 / 20                                                        │
└─────────────────────────────────────────────────────────────────────┘
```

### Componentes de la Interfaz

#### Campos de Entrada
- **Campo Clave**: Acepta exactamente 16 caracteres (128 bits)
- **Campo Texto**: Acepta hasta 8 caracteres (64 bits)
- **Cursor parpadeante**: Indica el campo activo

#### Panel de Estado (Izquierda)
Muestra información detallada sobre el paso actual:
- Valores de entrada
- Operaciones realizadas
- Resultados intermedios
- Valores de salida

#### Panel de Subclaves (Derecha)
Lista las 8 subclaves generadas (k[0] a k[7]) en hexadecimal.

## Controles y Navegación

### Teclado Principal
| Tecla | Función |
|-------|---------|
| **ENTER** | Iniciar proceso de cifrado/descifrado |
| **SPACE** | Avanzar al siguiente paso |
| **BACKSPACE** | Retroceder al paso anterior (o borrar texto) |
| **ESC** | Salir del programa |
| **TAB** | Cambiar entre campo de clave y texto |
| **A** | Activar/desactivar modo autoplay |

### Navegación Adicional
| Tecla | Función |
|-------|---------|
| **↑** | Scroll hacia arriba en panel de información |
| **↓** | Scroll hacia abajo en panel de información |

### Estados de la Aplicación

#### Estado Inicial
- Campos de entrada vacíos
- Panel de estado muestra: "Aún no preparado"
- Panel de subclaves vacío
- Esperando entrada del usuario

#### Estado de Entrada
- Usuario puede escribir en los campos
- **TAB** cambia entre campos
- **BACKSPACE** borra caracteres
- Cursor indica campo activo

#### Estado Preparado
- Después de presionar **ENTER** con datos válidos
- Se muestran todos los pasos del algoritmo
- Navegación con **SPACE** y **BACKSPACE**

#### Modo Autoplay
- Presiona **A** para activar
- Los pasos avanzan automáticamente cada 500ms
- Presiona **A** nuevamente para desactivar

## Flujo de Trabajo Completo

### Paso 1: Introducir Clave
1. El cursor inicia en el campo "Clave"
2. Escribe exactamente 16 caracteres
3. Ejemplos válidos:
   - `mi_clave_secreta`
   - `1234567890abcdef`
   - `password12345678`

**Importante**: La clave debe tener exactamente 16 caracteres.

### Paso 2: Introducir Texto
1. Presiona **TAB** para cambiar al campo "Texto"
2. Escribe hasta 8 caracteres
3. Ejemplos:
   - `hola`
   - `mensaje!`
   - `12345678`

### Paso 3: Iniciar Proceso
1. Presiona **ENTER**
2. El programa genera las subclaves automáticamente
3. Comienza la visualización paso a paso

### Paso 4: Navegación
- **SPACE**: Ver siguiente paso
- **BACKSPACE**: Volver al paso anterior
- **A**: Modo automático

## Interpretación de los Pasos

### Fase 1: Generación de Subclaves
**Pasos 1-7**: Muestra cómo se generan las 8 subclaves a partir de la clave maestra.

```
Paso 1: Generación subclave 0/1
- k[0] y k[1] extraídas directamente de la clave

Pasos 2-7: Generando subclave X
- Rotación de subclave anterior
- XOR con subclave dos posiciones atrás
- Suma de constante mágica
```

### Fase 2: Cifrado
**Pasos 8-15**: 8 rondas de cifrado del texto plano.

Para cada ronda:
```
Ronda X:
- Entrada: [valor hexadecimal]
- Subclave: k[X] = [valor hexadecimal]
- L (high32): [32 bits superiores]
- R (low32): [32 bits inferiores]
- L rot: [L rotado]
- R rot: [R rotado]
- XOR/comb: [resultado combinado]
- After rotate (D1): [después de rotaciones]
- Output: [resultado de la ronda]
```

### Fase 3: Resultado Cifrado
**Paso 16**: Muestra el texto cifrado final.

### Fase 4: Descifrado
**Pasos 17-24**: Proceso inverso para recuperar el texto original.

### Fase 5: Resultado Final
**Paso 25**: Muestra el texto plano recuperado.

## Ejemplos Prácticos

### Ejemplo 1: Mensaje Corto
```
Clave: "clave_de_ejemplo"  (16 caracteres)
Texto: "hola"              (4 caracteres)

Resultado esperado:
- 8 subclaves generadas
- Texto cifrado en hexadecimal
- Texto descifrado: "hola"
```

### Ejemplo 2: Mensaje Completo
```
Clave: "1234567890ABCDEF"  (16 caracteres)
Texto: "secreto!"          (8 caracteres)

Proceso:
- Generación de subclaves
- 8 rondas de cifrado
- 8 rondas de descifrado
- Recuperación: "secreto!"
```

### Ejemplo 3: Análisis Detallado
1. Introduce clave: `mi_super_clave12`
2. Introduce texto: `test`
3. Presiona **ENTER**
4. Usa **SPACE** para ver cada paso
5. Observa cómo cambian los valores en cada ronda
6. Verifica que el descifrado recupera "test"

## Interpretación de Valores Hexadecimales

### Formato de Salida
- **64 bits**: `0x1234567890abcdef` (16 dígitos hex)
- **32 bits**: `0x12345678` (8 dígitos hex)

### Correspondencia
- Cada dígito hexadecimal = 4 bits
- 2 dígitos hex = 1 byte = 8 bits
- 16 dígitos hex = 8 bytes = 64 bits

### Ejemplo de Conversión
```
Texto: "hola" 
ASCII: h=104, o=111, l=108, a=97
Hex:   h=68,  o=6F,  l=6C,  a=61
64-bit: 0x000000006C6F6C61 (little-endian)
```

## Consejos de Uso

### Para Aprendizaje
1. **Usa textos simples**: Comienza con "test", "hola", etc.
2. **Observa patrones**: Nota cómo pequeños cambios en la entrada afectan la salida
3. **Modo autoplay**: Útil para presentaciones
4. **Pausa y analiza**: Usa navegación manual para estudiar cada paso

### Para Demostración
1. **Prepara ejemplos**: Ten claves y textos de ejemplo listos
2. **Explica cada fase**: Generación, cifrado, descifrado
3. **Destaca la reversibilidad**: El descifrado recupera exactamente el texto original

### Para Desarrollo
1. **Verifica implementación**: Compara con especificaciones de CRUX-8
2. **Prueba casos límite**: Textos de 1 y 8 caracteres
3. **Documenta**: Anota valores interesantes para referencia

## Solución de Problemas de Uso

### "No pasa nada al presionar ENTER"
- Verifica que la clave tenga exactamente 16 caracteres
- Verifica que el texto tenga entre 1 y 8 caracteres

### "Los caracteres no aparecen"
- Verifica que el campo correcto esté activo
- Usa solo caracteres ASCII estándar (evita acentos)

### "El programa se cierra"
- Ejecuta desde terminal para ver mensajes de error
- Consulta la sección de troubleshooting

### "No puedo ver todo el texto"
- Usa las flechas ↑↓ para hacer scroll
- El panel se ajusta automáticamente al contenido

## Limitaciones

### Técnicas
- **Clave**: Exactamente 16 caracteres
- **Texto**: Máximo 8 caracteres
- **Caracteres**: Solo ASCII estándar (0-127)

### Funcionales
- No guarda/carga archivos
- No encripta archivos grandes
- Solo propósitos educativos

---
*Para problemas técnicos, consulta `TROUBLESHOOTING.md`. Para instalación, consulta `INSTALACION_COMPLETA.md`.*