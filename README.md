#Arduino - Auto 4wd

Este es un pequeño repo con algunos desarrollos para Arduino UNO, para manejar un auto 4wd, simil elegoo.
El código está dividido en las carpetas sueltas, a utilizar con la GUI de Arduino, y la carpeta MBlock como input de la aplicación homónima, que aplica bloques al estilo Scratch, para Arduino.

## Obstáculos 
Es una aplicación sencilla que utiliza un detector de ultrasonido para verificar si el vehículo está cerca de un obstáculo. Si lo está, toma una decisión simple y continúa.

[TO DO]
Utilizar el servo para observar hacia qué lado conviene ir

## SigueLíneas

Este proyecto hace que sobre una línea negra dibujada sobre fondo blanco, el vehículo pueda seguir esa pista. Si las curvas son muy cerradas, puede que no llegue a girar. Hoy está utilizando dos detectores, uno a cada lado, por lo que si ve algo que no refleja de un lado, gira hacia el otro.
Ví implementaciones que usan tres detectores, de modo de que siempre el de medio sea opaco y los demás reflejen.

## MBlock

Esta carpeta tiene una aplicación similar a Obstáculos, pero orientada al lenguaje Mblock


