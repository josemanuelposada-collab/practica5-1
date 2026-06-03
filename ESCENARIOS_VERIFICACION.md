# Escenarios de verificacion

Estos escenarios ayudan a comprobar que la simulacion cumple los requisitos de la actividad.

## Escenario 1: simulacion base

Comando:

```powershell
Actividad1Colisiones.exe --no-gui
```

Que revisar:

- `salida/trayectorias.csv` debe contener columnas de posicion, velocidad, aceleracion, masa y radio.
- `salida/colisiones.txt` debe incluir colisiones contra paredes, obstaculos y entre particulas.
- `salida/resumen.txt` debe reportar 4 obstaculos y al menos 4 particulas configuradas.
- `salida/trayectorias.png` debe mostrar la caja, los obstaculos y las trayectorias.

## Escenario 2: disparo manual alto

Comando:

```powershell
Actividad1Colisiones.exe --no-gui --angulo=45 --velocidad=140
```

Que revisar:

- La primera fila de la particula 1 debe mostrar `vx` y `vy` con magnitudes similares.
- `vy` debe iniciar negativo porque en Qt el eje vertical positivo apunta hacia abajo.
- La trayectoria debe curvarse por la aceleracion `ay`.

## Escenario 3: disparo manual rasante

Comando:

```powershell
Actividad1Colisiones.exe --no-gui --angulo=15 --velocidad=180
```

Que revisar:

- La particula 1 debe recorrer mas distancia horizontal al inicio.
- Deben conservarse los rebotes elasticos con paredes.
- Si se produce una colision con obstaculo, `colisiones.txt` debe registrar el lado impactado.

## Evidencias esperadas

Al finalizar cualquiera de los escenarios, la carpeta `salida` debe contener:

- `trayectorias.csv`
- `colisiones.txt`
- `resumen.txt`
- `trayectorias.png`
