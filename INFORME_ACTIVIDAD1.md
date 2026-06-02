# Informe - Actividad 1

## Objetivo

Simular el movimiento de varias particulas circulares dentro de una caja rectangular con obstaculos cuadrados estaticos, aplicando movimiento en tiempo discreto y tres tipos de colision:

- Paredes de la caja: colision perfectamente elastica.
- Obstaculos cuadrados: colision inelastica con coeficiente de restitucion.
- Particula contra particula: colision completamente inelastica con fusion de masas.

## Modelo de movimiento

La simulacion usa un bucle de tiempo discreto. En cada paso se actualiza la posicion y la velocidad de cada particula usando las ecuaciones de movimiento con aceleracion constante:

```text
x(t + dt) = x(t) + vx(t) * dt + 1/2 * ax * dt^2
y(t + dt) = y(t) + vy(t) * dt + 1/2 * ay * dt^2
vx(t + dt) = vx(t) + ax * dt
vy(t + dt) = vy(t) + ay * dt
```

En el programa, `dt = 0.016`, equivalente aproximadamente a 60 actualizaciones por segundo. La aceleracion usada es una gravedad simulada `a = (0, 12)`, porque en Qt el eje `y` positivo apunta hacia abajo. Si se quisiera una simulacion sin gravedad, bastaria con cambiarla a `a = (0, 0)`.

Para verificar diferentes trayectorias, el programa incluye un modo de disparo manual para la particula 1. El usuario puede definir un angulo y una rapidez inicial. El programa convierte esos datos a componentes de velocidad con:

```text
vx = rapidez * cos(angulo)
vy = -rapidez * sin(angulo)
```

El signo negativo en `vy` se usa porque en la pantalla de Qt el eje vertical positivo apunta hacia abajo.

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

Los obstaculos usan coeficiente de restitucion `e`. El programa calcula el punto mas cercano del cuadrado al centro de la particula para obtener la normal de impacto. Con esa normal tambien identifica el lado golpeado del cuadrado: izquierdo, derecho, superior o inferior. Luego separa la velocidad en componente perpendicular y paralela:

```text
v' = v - (1 + e) * (v dot n) * n
```

La componente paralela se conserva y la componente perpendicular rebota perdiendo energia segun `e`.

En `colisiones.txt` cada choque con obstaculo registra tambien el lado detectado.

## Colision entre particulas

Cuando dos particulas se tocan y se estan moviendo una hacia la otra, se fusionan. La velocidad final conserva el momento lineal:

```text
v' = (m1 * v1 + m2 * v2) / (m1 + m2)
```

La posicion final se calcula como centro de masa:

```text
p' = (m1 * p1 + m2 * p2) / (m1 + m2)
```

El radio final se calcula conservando el area aproximada de las dos particulas circulares:

```text
r' = sqrt(r1^2 + r2^2)
```

## Salidas

El programa genera:

- `trayectorias.csv`: posiciones, velocidades, aceleracion, masa y radio en cada instante.
- `colisiones.txt`: registro de colisiones con tiempo, descripcion, pared impactada o lado del obstaculo.
- `trayectorias.png`: grafica final de trayectorias.

## Clases principales

- `Vector2D`: operaciones vectoriales basicas.
- `Particle`: posicion, velocidad, masa, radio, color y fusion.
- `Obstacle`: rectangulo cuadrado y coeficiente de restitucion.
- `Simulation`: bucle de simulacion, ecuaciones de movimiento, colisiones y exportacion.
- `TrajectoryWidget`: animacion y dibujo de trayectorias con Qt.
