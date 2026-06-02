# Actividad 1 - Simulacion de colisiones multiples

Proyecto Qt listo para abrir en Qt Creator.

## Que incluye

- Clase `Simulation` con bucle de tiempo discreto.
- Al menos 4 particulas simultaneas dentro de una caja rectangular.
- 4 obstaculos cuadrados estaticos.
- Colisiones con paredes perfectamente elasticas.
- Colisiones particula-obstaculo inelasticas con coeficiente de restitucion.
- Colisiones particula-particula completamente inelasticas, con fusion de masas y conservacion del momento lineal.
- Exportacion de datos a `salida/trayectorias.csv` y `salida/colisiones.txt`.
- Animacion de las particulas con `QTimer` y grafica de trayectorias con Qt.
- Exportacion de la grafica final a `salida/trayectorias.png`.
- Diagrama de clases en `DIAGRAMA_CLASES.md`.
- Informe de implementacion en `INFORME_ACTIVIDAD1.md`.

## Como ejecutarlo

1. Abrir Qt Creator.
2. Elegir `File > Open File or Project`.
3. Abrir `Actividad1Colisiones.pro`.
4. Configurar un kit de escritorio.
5. Ejecutar el proyecto.

Al correr, la ventana reproduce la simulacion animada y se crea una carpeta `salida` dentro del directorio del ejecutable.

Para probar solo la exportacion desde consola se puede ejecutar:

```powershell
Actividad1Colisiones.exe --no-gui
```
