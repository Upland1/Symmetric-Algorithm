# Script de Distribución - CRUX-8 Visual

# Crear paquete de distribución para Windows
# Ejecutar en PowerShell en la carpeta del proyecto

Write-Host "=== CRUX-8 Visual - Script de Distribución ===" -ForegroundColor Green

# Verificar archivos necesarios
$archivos_necesarios = @(
    "crypto_gui_sfml26.exe",
    "sfml-graphics-2.dll",
    "sfml-window-2.dll", 
    "sfml-system-2.dll"
)

Write-Host "Verificando archivos necesarios..." -ForegroundColor Yellow

$archivos_faltantes = @()
foreach ($archivo in $archivos_necesarios) {
    if (!(Test-Path $archivo)) {
        $archivos_faltantes += $archivo
    }
}

if ($archivos_faltantes.Count -gt 0) {
    Write-Host "ERROR: Faltan archivos necesarios:" -ForegroundColor Red
    foreach ($archivo in $archivos_faltantes) {
        Write-Host "  - $archivo" -ForegroundColor Red
    }
    Write-Host "Ejecuta primero la compilación completa." -ForegroundColor Red
    exit 1
}

Write-Host "✓ Todos los archivos necesarios encontrados" -ForegroundColor Green

# Crear carpeta de distribución
$carpeta_dist = "CRUX-8-Visual-Distribucion"
if (Test-Path $carpeta_dist) {
    Remove-Item $carpeta_dist -Recurse -Force
}
New-Item -ItemType Directory -Path $carpeta_dist | Out-Null

Write-Host "Copiando archivos de distribución..." -ForegroundColor Yellow

# Copiar ejecutable y DLLs
Copy-Item "crypto_gui_sfml26.exe" -Destination $carpeta_dist
Copy-Item "*.dll" -Destination $carpeta_dist

# Copiar documentación
$docs = @("README.md", "MANUAL_USUARIO.md", "TROUBLESHOOTING.md")
foreach ($doc in $docs) {
    if (Test-Path $doc) {
        Copy-Item $doc -Destination $carpeta_dist
    }
}

# Crear archivo de instrucciones para el usuario final
$instrucciones = @"
# CRUX-8 Visual - Instrucciones de Uso

## Instalación
1. Extrae todos los archivos en una carpeta
2. No se requiere instalación adicional

## Ejecución
1. Abre PowerShell o CMD en esta carpeta
2. Ejecuta: .\crypto_gui_sfml26.exe
3. O haz doble clic en crypto_gui_sfml26.exe

## Archivos Incluidos
- crypto_gui_sfml26.exe    # Programa principal
- sfml-*.dll               # Librerías necesarias
- README.md                # Documentación completa
- MANUAL_USUARIO.md        # Manual de uso detallado
- TROUBLESHOOTING.md       # Solución de problemas

## Uso Básico
1. Introduce una clave de exactamente 16 caracteres
2. Introduce texto de hasta 8 caracteres
3. Presiona ENTER para iniciar
4. Usa SPACE para avanzar pasos, BACKSPACE para retroceder
5. Usa ESC para salir

## Soporte
Para problemas técnicos, consulta TROUBLESHOOTING.md

## Requisitos del Sistema
- Windows 10 o superior
- 2 GB RAM mínimo
- Tarjeta gráfica compatible con OpenGL 2.0

Desarrollado como proyecto educativo de criptografía.
"@

$instrucciones | Out-File -FilePath "$carpeta_dist\INSTRUCCIONES.txt" -Encoding UTF8

# Verificar tamaños
Write-Host "Información de archivos:" -ForegroundColor Yellow
Get-ChildItem $carpeta_dist | ForEach-Object {
    $size = if ($_.Length -gt 1MB) { 
        "{0:F1} MB" -f ($_.Length / 1MB) 
    } else { 
        "{0:F0} KB" -f ($_.Length / 1KB) 
    }
    Write-Host "  $($_.Name) - $size"
}

# Crear ZIP
$fecha = Get-Date -Format "yyyy-MM-dd"
$zip_name = "CRUX-8-Visual-v1.0-$fecha.zip"

Write-Host "Creando archivo ZIP: $zip_name" -ForegroundColor Yellow

if (Test-Path $zip_name) {
    Remove-Item $zip_name -Force
}

Compress-Archive -Path "$carpeta_dist\*" -DestinationPath $zip_name

# Verificar ZIP
if (Test-Path $zip_name) {
    $zip_size = (Get-Item $zip_name).Length
    $zip_size_mb = "{0:F1} MB" -f ($zip_size / 1MB)
    Write-Host "✓ ZIP creado exitosamente: $zip_name ($zip_size_mb)" -ForegroundColor Green
} else {
    Write-Host "ERROR: No se pudo crear el ZIP" -ForegroundColor Red
    exit 1
}

# Crear script de prueba
$script_prueba = @"
@echo off
echo === CRUX-8 Visual - Prueba Rapida ===
echo.
echo Verificando archivos...

if not exist "crypto_gui_sfml26.exe" (
    echo ERROR: No se encuentra crypto_gui_sfml26.exe
    pause
    exit /b 1
)

if not exist "sfml-graphics-2.dll" (
    echo ERROR: No se encuentra sfml-graphics-2.dll
    pause
    exit /b 1
)

echo Todos los archivos encontrados.
echo.
echo Ejecutando CRUX-8 Visual...
echo Presiona Ctrl+C para cancelar, o cualquier tecla para continuar.
pause > nul

crypto_gui_sfml26.exe

echo.
echo Programa terminado.
pause
"@

$script_prueba | Out-File -FilePath "$carpeta_dist\PRUEBA_RAPIDA.bat" -Encoding ASCII

Write-Host ""
Write-Host "=== DISTRIBUCIÓN COMPLETADA ===" -ForegroundColor Green
Write-Host "Archivos creados:" -ForegroundColor White
Write-Host "  - $carpeta_dist\          # Carpeta con todos los archivos" -ForegroundColor Cyan
Write-Host "  - $zip_name               # Archivo ZIP para distribución" -ForegroundColor Cyan
Write-Host ""
Write-Host "Para distribuir:" -ForegroundColor Yellow
Write-Host "  1. Comparte el archivo $zip_name" -ForegroundColor White
Write-Host "  2. El usuario debe extraer todos los archivos" -ForegroundColor White
Write-Host "  3. Ejecutar crypto_gui_sfml26.exe o PRUEBA_RAPIDA.bat" -ForegroundColor White
Write-Host ""
Write-Host "Para probar localmente:" -ForegroundColor Yellow
Write-Host "  cd $carpeta_dist" -ForegroundColor White
Write-Host "  .\crypto_gui_sfml26.exe" -ForegroundColor White

# Mostrar contenido final
Write-Host ""
Write-Host "Contenido del paquete de distribución:" -ForegroundColor Yellow
Get-ChildItem $carpeta_dist | ForEach-Object {
    Write-Host "  ✓ $($_.Name)" -ForegroundColor Green
}

Write-Host ""
Write-Host "Script completado exitosamente." -ForegroundColor Green