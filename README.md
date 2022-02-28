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
Después de analizar y revisar alternativas de servicios que corran sobre la plataforma de inteligencia artificial de amazon,  se decidió utilizar como servicio de enlace [Sinric Pro](https://sinric.pro/es-index.html) el cual tiene tarifas muy bajas y te permite utilizar dos dispositivos gratis. Esto es solo para la interfaz entre aws de amazon y nuestro dispositivo, en un futuro se utilizará uno propio.
Para poder configurar la conexión wifi de manera dinámica sin tener que estar programando el dispositivo, se integra un lector de tarjeta microSD, son muy económicas y potencian el dispositivo.

# Televisión SAMSUNG y Aire Acondicionado
Control remoto para televisores SAMSUNG y aire acondicionado CARRIER (pueden ser modificados si se tienen los códigos correctos de cada botón), que no son dispositivos directamente smart o no cuenten con la conexión directa a Alexa de Amazon.<br>
Para configurar la red, solo coloque un archivo en una microSD llamado [wifi.json](https://github.com/bernardosegura/esp8266Alexa/blob/master/template/wifi.json). No es necesario utilizar siempre una microSD, ya que se almacena la información en la EEPROM del dispositivo y de esta manera solo se actualiza los datos de la red (nombre y contraseña) cuando sea necesario.Solo coloque y reinicie el dispositivo o enciendalo con la microSD dentro.

#### Datos que se tienen que modificar del archivo.
```json
{"WIFI_SSID":"nombre de tu red","WIFI_PASS":"clave de tu red"}
```
> Nota: la microSD debe tener el formato Fat16, Fat32 o exFat.<br>
Para realizar el desarrollo se utiliza [Arduino](https://www.arduino.cc/) y con la integración de la librerías para el desarrollo de este chip.

# Diagrama y Componentes
### Componentes
- **2 Leds Infrarojos.**
- 1 **Transistor 2N2222.**
- 1 **nodeMCU esp8266.**
- 1 **Protoboard.**
- 1 **Lector de tarjeta micro SD.**

### Lector de tarjeta microSD

<img width="429" height="420" src="https://github.com/bernardosegura/esp8266Alexa/blob/master/img/circuitoPlaca.jpg?raw=true" />

> Nota: si el lector no tiene grabado los nombres de los pines, se pueden identificar con el grabado que tiene 10K vista de donde esta el modulo de microSD y el primer pin es GND.

### Diagrama

<img src="https://github.com/bernardosegura/esp8266Alexa/blob/master/img/diagrama-nodemcu-esp8266.jpg?raw=true" />

### PinOut del Esp8266
```
    -----------
    |         |
    |      D1 |------------------> (B) Base del transistor --> (Transmisor IR)
    |      D2 |------------------> (3V3 VCC) Alimentación de lector de tarjeta microSD
    |         |  
    |         |             
    | ESP8266 | 
    |         |
    |      D5 |------------------> CLK  de lector de tarjeta microSD               
    |      D6 |------------------> MISO de lector de tarjeta microSD               
    |      D7 |------------------> MOSI de lector de tarjeta microSD
    |      D8 |------------------> CS   de lector de tarjeta microSD        
    |         | 
    |     GND |------------------> GND  de lector de tarjeta microSD  
    |    3.3v |
    -----------
```
### Imagenes

<img width="729" height="420" src="https://github.com/bernardosegura/esp8266Alexa/blob/master/img/IMG_20210730_140147.jpg?raw=true" />

<img width="629" height="420" src="https://github.com/bernardosegura/esp8266Alexa/blob/master/img/IMG_20220205_194111.jpg?raw=true" />

<img width="629" height="520" src="https://github.com/bernardosegura/esp8266Alexa/blob/master/img/IMG_20220206_114219.jpg?raw=true" />

<img width="629" height="620" src="https://github.com/bernardosegura/esp8266Alexa/blob/master/img/IMG_20220206_114257.jpg?raw=true" />
