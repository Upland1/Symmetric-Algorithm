# Algoritmo CRUX-8 - Especificación Técnica

## Resumen
CRUX-8 es un algoritmo de cifrado simétrico de bloque que opera sobre bloques de 64 bits utilizando claves de 128 bits. Diseñado con fines educativos para demostrar conceptos fundamentales de criptografía simétrica.

## Parámetros del Algoritmo

| Parámetro | Valor |
|-----------|-------|
| Tamaño de bloque | 64 bits |
| Tamaño de clave | 128 bits (16 bytes) |
| Número de rondas | 8 |
| Número de subclaves | 8 |
| Tamaño de subclave | 64 bits cada una |

## Estructura General

### 1. Generación de Subclaves
```
Entrada: Clave maestra K (128 bits)
Salida: 8 subclaves K₀, K₁, ..., K₇ (64 bits cada una)

Proceso:
K₀ = K[0:63]     # Primeros 64 bits de K
K₁ = K[64:127]   # Segundos 64 bits de K

Para i = 2 hasta 7:
    rotación = (i × 3) mod 64
    K_i = ROL(K_{i-1}, rotación) ⊕ K_{i-2} + 0x9E3779B97F4A7C15
```

### 2. Función de Cifrado
```
Entrada: Texto plano P (64 bits), Subclaves K₀...K₇
Salida: Texto cifrado C (64 bits)

X = P
Para r = 0 hasta 7:
    X = FuncionRonda(X, K_r)
C = X
```

### 3. Función de Ronda
```
FuncionRonda(X, K_r):
    # Paso 1: XOR con subclave
    C₁ = X ⊕ K_r
    
    # Paso 2: Extraer desplazamiento
    d = K_r & 0x3F  # 6 bits menos significativos
    
    # Paso 3: División en mitades
    L = C₁ & 0xFFFFFFFF00000000  # 32 bits superiores
    R = C₁ & 0x00000000FFFFFFFF  # 32 bits inferiores
    
    # Paso 4: Rotaciones dependientes de la clave
    L_d = ROL(L, d)
    R_d = ROR(R, 64-d)
    
    # Paso 5: Combinación
    D₁ = L_d | R_d
    
    # Paso 6: Suma con subclave
    Salida = D₁ + K_r
    
    return Salida
```

### 4. Función de Descifrado
```
Entrada: Texto cifrado C (64 bits), Subclaves K₀...K₇
Salida: Texto plano P (64 bits)

X = C
Para r = 7 hasta 0 (orden inverso):
    X = FuncionRondaInversa(X, K_r)
P = X
```

### 5. Función de Ronda Inversa
```
FuncionRondaInversa(X, K_r):
    # Paso 1: Resta con subclave
    D₁ = X - K_r
    
    # Paso 2: Extraer desplazamiento
    d = K_r & 0x3F
    
    # Paso 3: División en mitades
    L_d = D₁ & 0xFFFFFFFF00000000
    R_d = D₁ & 0x00000000FFFFFFFF
    
    # Paso 4: Rotaciones inversas
    L = ROR(L_d, d)
    R = ROL(R_d, 64-d)
    
    # Paso 5: Combinación
    C₁ = L | R
    
    # Paso 6: XOR con subclave
    Salida = C₁ ⊕ K_r
    
    return Salida
```

## Operaciones Primitivas

### Rotación a la Izquierda (ROL)
```
ROL(data, shift):
    return (data << shift) | (data >> (64 - shift))
```

### Rotación a la Derecha (ROR)
```
ROR(data, shift):
    return (data >> shift) | (data << (64 - shift))
```

### Constante Mágica
La constante `0x9E3779B97F4A7C15` es derivada de la proporción áurea:
- φ = (1 + √5) / 2 ≈ 1.618033988749895
- 2⁶⁴ / φ ≈ 0x9E3779B97F4A7C15

## Ejemplo de Ejecución

### Datos de Entrada
```
Clave: "mi_clave_secreta" (ASCII)
       = 0x6D695F636C6176655F73656372657461 (hex)
       
Texto: "hola" (ASCII)  
       = 0x686F6C61 → 0x000000006C6F6C61 (64-bit, little-endian)
```

### Generación de Subclaves
```
K₀ = 0x6D695F636C617665  # Primeros 8 bytes
K₁ = 0x5F73656372657461  # Segundos 8 bytes

K₂ = ROL(K₁, 6) ⊕ K₀ + 0x9E3779B97F4A7C15
   = ROL(0x5F73656372657461, 6) ⊕ 0x6D695F636C617665 + 0x9E3779B97F4A7C15
   = 0xDCD959190995D840 ⊕ 0x6D695F636C617665 + 0x9E3779B97F4A7C15
   = 0xB1B006FAE5F4AE25 + 0x9E3779B97F4A7C15
   = 0x4FE78094653F2A3A

K₃ = ROL(K₂, 9) ⊕ K₁ + 0x9E3779B97F4A7C15
   = ...

[Continúa para K₄, K₅, K₆, K₇]
```

### Proceso de Cifrado
```
Ronda 0:
X₀ = 0x000000006C6F6C61
C₁ = X₀ ⊕ K₀ = 0x000000006C6F6C61 ⊕ 0x6D695F636C617665 = 0x6D695F6300181804
d = K₀ & 0x3F = 0x6D695F636C617465 & 0x3F = 0x25 = 37
L = 0x6D695F6300000000
R = 0x0000000000181804
L_d = ROL(0x6D695F6300000000, 37) = 0x00000036B4AFB180
R_d = ROR(0x0000000000181804, 27) = 0x0000C0200000000
D₁ = 0x00000036B4AFB180 | 0x0000C0200000000 = 0x00000036B4AFB180
X₁ = D₁ + K₀ = 0x00000036B4AFB180 + 0x6D695F636C617465 = 0x6D695F9A210126E5

[Continúa para rondas 1-7]
```

## Propiedades de Seguridad

### Fortalezas Diseñadas
1. **Difusión**: Cada bit de entrada afecta múltiples bits de salida
2. **Confusión**: Relación compleja entre clave y texto cifrado
3. **Avalancha**: Pequeños cambios en entrada causan grandes cambios en salida
4. **Reversibilidad**: Todas las operaciones son invertibles

### Limitaciones Conocidas
1. **Tamaño de clave**: 128 bits puede ser vulnerable a ataques futuros
2. **Tamaño de bloque**: 64 bits es susceptible a ataques de cumpleaños
3. **Rondas**: 8 rondas pueden ser insuficientes para máxima seguridad
4. **Propósito educativo**: No está diseñado para uso en producción

## Vectores de Prueba

### Vector 1
```
Clave: "0123456789ABCDEF"
Texto plano: "FEDCBA98"
Texto cifrado esperado: [calcular]
```

### Vector 2
```
Clave: "FFFFFFFFFFFFFFFF"
Texto plano: "00000000"
Texto cifrado esperado: [calcular]
```

### Vector 3
```
Clave: "mi_clave_secreta"
Texto plano: "hola"
Texto cifrado esperado: [calcular]
```

## Implementación en C++

### Tipos de Datos
```cpp
typedef unsigned long long u64;  // 64 bits
typedef unsigned int u32;        // 32 bits
```

### Funciones Principales
```cpp
std::vector<u64> generar_subclaves(const std::string& clave_maestra);
u64 cifrar_bloque(u64 texto_plano, const std::vector<u64>& subclaves);
u64 descifrar_bloque(u64 texto_cifrado, const std::vector<u64>& subclaves);
```

## Análisis de Complejidad

### Temporal
- Generación de subclaves: O(1) - 8 operaciones constantes
- Cifrado/Descifrado: O(1) - 8 rondas constantes
- Total: O(1) para bloques individuales

### Espacial
- Almacenamiento de subclaves: 8 × 64 bits = 512 bits
- Variables temporales: ~200 bits
- Total: O(1) espacio constante

## Variantes y Extensiones Posibles

### CRUX-16 (Hipotético)
- Bloques de 128 bits
- Claves de 256 bits  
- 16 rondas

### CRUX-8 con S-boxes
- Agregar cajas de sustitución
- Aumentar no-linealidad

### Modo de Operación
- CBC (Cipher Block Chaining)
- CTR (Counter Mode)
- GCM (Galois/Counter Mode)

---
*Esta especificación describe CRUX-8 tal como está implementado en la aplicación visual. Para uso en producción, se recomiendan algoritmos estándar como AES.*