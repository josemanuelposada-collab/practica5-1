# Informe - Actividad 1

## Objetivo

Simular el movimiento de varias particulas circulares dentro de una caja rectangular con obstaculos cuadrados estaticos, aplicando movimiento en tiempo discreto y tres tipos de colision:

- Paredes de la caja: colision perfectamente elastica.
- Obstaculos cuadrados: colision inelastica con coeficiente de restitucion.
- Particula contra particula: colision completamente inelastica con fusion de masas.

## Modelo de movimiento

La simulacion usa un bucle de tiempo discreto. En cada paso se actualiza la posicion de cada particula con:

```text
x(t + dt) = x(t) + vx * dt
y(t + dt) = y(t) + vy * dt
```

En el programa, `dt = 0.016`, equivalente aproximadamente a 60 actualizaciones por segundo.

## Colision con paredes

Las paredes son perfectamente elasticas. Cuando una particula toca una pared vertical se invierte `vx`; cuando toca una pared horizontal se invierte `vy`.

```text
vx' = -vx
vy' = vy
```

o

```text
vx' = vx
vy' = -vy
```

## Colision con obstaculos cuadrados

Los obstaculos usan coeficiente de restitucion `e`. El programa calcula el punto mas cercano del cuadrado al centro de la particula para obtener la normal de impacto. Luego separa la velocidad en componente perpendicular y paralela:

```text
v' = v - (1 + e) * (v dot n) * n
```

La componente paralela se conserva y la componente perpendicular rebota perdiendo energia segun `e`.

## Colision entre particulas

Cuando dos particulas se tocan y se estan moviendo una hacia la otra, se fusionan. La velocidad final conserva el momento lineal:

```text
v' = (m1 * v1 + m2 * v2) / (m1 + m2)
```

La posicion final se calcula como centro de masa:

```text
p' = (m1 * p1 + m2 * p2) / (m1 + m2)
```

El radio final se mantiene controlado visualmente para que la animacion sea clara, pero sigue representando que el cuerpo final tiene mayor masa.

## Salidas

El programa genera:

- `trayectorias.csv`: posiciones, velocidades, masa y radio en cada instante.
- `colisiones.txt`: registro de colisiones con tiempo y descripcion.
- `trayectorias.png`: grafica final de trayectorias.

## Clases principales

- `Vector2D`: operaciones vectoriales basicas.
- `Particle`: posicion, velocidad, masa, radio, color y fusion.
- `Obstacle`: rectangulo cuadrado y coeficiente de restitucion.
- `Simulation`: bucle de simulacion, colisiones y exportacion.
- `TrajectoryWidget`: animacion y dibujo de trayectorias con Qt.
