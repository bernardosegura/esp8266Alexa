# Esp8266
El chip ESP8266 es de bajo costo y cuenta Wi-Fi con un stack TCP/IP completo y un microcontrolador, fabricado por Espressif, una empresa afincada en Shanghái, China.

# Características Técnicas del Chip
- CPU RISC de 32-bit: Tensilica Xtensa LX106 a un reloj de 80 MHz
- RAM de instrucción de 64 KB, RAM de datos de 96 KB
- Capacidad de memoria externa flash QSPI - 512 KB a 4 MB* (puede soportar hasta 16 MB)
- IEEE 802.11 b/g/n Wi-Fi
   1. Tiene integrados: TR switch, balun, LNA, amplificador de potencia de RF y una red de adaptación de impedancias
   2. Soporte de autenticación WEP y WPA/WPA2
- 16 pines GPIO (Entradas/Salidas de propósito general)
- SPI, I²C,
- Interfaz I²S con DMA (comparte pines con GPIO)
- Pines dedicados a UART, más una UART únicamente para transmisión que puede habilitarse a través del pin GPIO2
- 1 conversor ADC de 10-bit

# NodeMCU ESP8266 Placa de Desarrollo
La placa NodeMCU ESP8266 contiene un módulo ESP-12E que contiene un microprocesador de 32 bits. Integra un transceptor WiFi que le permite conectarse a las redes existentes o crear su propia red.
La red WiFi es una red de radio que funciona en las frecuencias de 2,45 GHz y 5 GHz.

<img src="https://github.com/bernardosegura/esp8266Alexa/blob/master/img/module-nodemcu-esp8266.jpg?raw=true" />

# Pines de la Placa de Desarrollo 
<img src="https://github.com/bernardosegura/esp8266Alexa/blob/master/img/pines-nodemcu-esp8266.jpg?raw=true" />

<img src="https://github.com/bernardosegura/esp8266Alexa/blob/master/img/pinout-nodemcu-esp8266.jpg?raw=true" />

# Controlar Televisión y Aire ACondicionado con Alexa de Amazon y Sinric Pro
Después de analizar y revisar alternativas de servicios que corran sobre la plataforma de inteligencia artificial de amazon,  se decidió utilizar como servicio de enlace [Sinric Pro](https://sinric.pro/es-index.html) el cual tiene tarifas muy bajas y te permite utilizar dos dispositivos gratis. Esto es solo para la interfaz entre aws de amazon y nuestro dispositivo, en un futuro se utilizará uno propio.

# Televisión SAMSUNG y Aire Acondicionado
Control remoto para televisores SAMSUNG y aire acondicionado CARRIER (pueden ser modificados si se tienen los codigos correctos de cada botón), que no son dispositivos directamente smart o no cuenten con la conexión directa a Alexa de Amazon.

> Datos que se tienen que modificar para su funcionalidad.
```c
#define WIFI_SSID         "Nombre de la red"    
#define WIFI_PASS         "pass de la red"
#define APP_KEY           "clave que proviene de sinric pro"      
#define APP_SECRET        "dato que proviene de sinric pro"  
#define TV_ID             "ID que proviene de sinric pro"  
```
> Para realizar el desarrollo se utiliza [Arduino](https://www.arduino.cc/) y con la integración de la librerías para el desarrollo de este chip.

# Diagrama y Componentes
### Componentes
- **2 Leds Infrarojos.**
- 1 **Transistor 2N2222.**
- 1 **nodeMCU esp8266.**
- 1 **Protoboard.**

### Diagrama

<img src="https://github.com/bernardosegura/esp8266Alexa/blob/master/img/diagrama-nodemcu-esp8266.jpg?raw=true" />

<img src="https://github.com/bernardosegura/esp8266Alexa/blob/master/img/IMG_20210730_140147.jpg?raw=true" />
