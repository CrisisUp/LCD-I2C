# 🌤️ Estação de Monitorização de Temperatura e Humidade com ESP32

[![PlatformIO](https://img.shields.io/badge/PlatformIO-Framework-orange.svg)](https://platformio.org/)
[![Framework](https://img.shields.io/badge/Framework-Arduino-blue.svg)](https://www.arduino.cc/)
[![ESP32](https://img.shields.io/badge/Hardware-ESP32--WROOM--32D-red.svg)](https://www.espressif.com/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

Projeto de um sistema embebido com **ESP32** capaz de monitorizar em tempo real a temperatura e a humidade relativa do ar através de um sensor digital **DHT22 (AM2302)**, exibindo os dados num display **LCD 16x2 com interface I2C** e enviando telemetria de depuração via porta série.

📸 Demonstração do Projeto

Adicione fotografias reais da sua montagem e bancada na pasta docs/ para enriquecer a apresentação visual.
Montagem na BreadboardDisplay em Funcionamento

🛠️ Componentes Utilizados
ComponenteQuantidadeDescrição / EspecificaçõesESP32 NodeMCU1Placa de desenvolvimento com microcontrolador Xtensa Dual-CoreSensor DHT22 (AM2302)1Sensor digital de temperatura (-40 a 80 °C) e humidade (0 a 100%)Display LCD 16x21Módulo de ecrã alfanumérico HD44780Módulo Adaptador I2C (PCF8574)1Interface de comunicação I2C de 2 fios para o LCDResistor de 4.7kΩ ou 10kΩ1Resistor de pull-up para a linha de dados do DHT22 (opcional se o módulo já integrar)Protoboard & Jumpers-Cablagem e matriz de contactos para ligações🔌 Esquema de Ligações (Pinout & Wiring)O diagrama abaixo descreve as ligações elétricas entre os pinos do ESP32, o módulo LCD I2C e o sensor DHT22:Plaintext                  +-----------------------------------+
                  |             ESP32                 |
                  |                                   |
                  |  [3V3 / VIN] ----+-----------+    |
                  |                  |           |    |
                  |  [GND] ----------+-------+   |    |
                  |                  |       |   |    |
                  |  [GPIO 21] ------|---+   |   |    |
                  |  [GPIO 22] ------|---|---+   |    |
                  |  [GPIO 23] ------+---|---|---|--+ |
                  +------------------|---|---|---|--|-+
                                     |   |   |   |  |
                                     |   |   |   |  |
           +-------------------------+   |   |   |  |
           |                             |   |   |  |
           |      +----------------------+   |   |  |
           |      |                          |   |  |
     +-----v------v------+             +-----v---v--v------+
     |   Módulo LCD I2C  |             |   Sensor DHT22    |
     |   (Endereço 0x27) |             |   (Vista Frontal) |
     +-------------------+             +-------------------+
     | Pin 1: GND  (GND) |             | Pin 1: VCC (3.3V) |
     | Pin 2: VCC  (VIN) |             | Pin 2: DAT (G23)  |
     | Pin 3: SDA  (G21) |             | Pin 3: NC  (Vazio)|
     | Pin 4: SCL  (G22) |             | Pin 4: GND (GND)  |
     +-------------------+             +-------------------+
Tabela de Correspondência de PinosPeriféricoPino do DispositivoPino do ESP32NotasDisplay LCD (I2C)GNDGNDReferência de terra comumVCCVIN / 5VAlimentação recomendada de 5V para o backlightSDAGPIO 21Barramento I2C - Linha de DadosSCLGPIO 22Barramento I2C - Linha de RelógioSensor DHT22VCC (Pino 1)3V3 / VINAlimentação do sensorDATA (Pino 2)GPIO 23Linha digital de leitura (com pull-up)NC (Pino 3)Não conectadoSem ligaçãoGND (Pino 4)GNDReferência de terra

💻 Funcionalidades do FirmwareLeitura Estável não-bloqueante: Amostragem periódica do DHT22 a cada 2,5 segundos utilizando millis(), mantendo o processador livre.Gráficos Customizados na CGRAM: Ícones desenhados em matriz $5 \times 8$ para representar visualmente termómetro, gota de água e o símbolo de grau (°C).Validação de Erros: Verificação de leitura digital com fallback visual (Erro no sensor!) no display e alerta no terminal série caso ocorra desconexão física.Telemetria Série: Transmissão dos dados a 115200 bauds para depuração em tempo real no VS Code.

🚀 Como Executar o ProjetoPré-requisitosVisual Studio CodeExtensão PlatformIO IDEControladores da porta série do ESP32 instalados (CP2102 ou CH340)Passos de Instalação e CompilaçãoClone este repositório:Bashgit clone [https://github.com/SEU-USUARIO/SEU-REPOSITORIO.git](https://github.com/SEU-USUARIO/SEU-REPOSITORIO.git)
cd SEU-REPOSITORIO
Abra o projeto no VS Code (Ficheiro -> Abrir Pasta).Ligue o ESP32 ao computador via cabo USB.Compile e envie o código para a placa através do terminal do PlatformIO:Bashpio run --target upload
Abra o Monitor Série para acompanhar as medições:Bashpio device monitor

📁 Estrutura do Repositório

Plaintext├── .vscode/               # Configurações do editor
├── docs/                  # Imagens e fotografias da montagem física
│   ├── circuito.jpg
│   └── display.jpg
├── include/               # Ficheiros de cabeçalho (.h)
├── lib/                   # Bibliotecas privadas do projeto
├── src/
│   └── main.cpp           # Código-fonte principal com lógica do LCD e DHT22
├── .gitignore             # Ficheiros e diretórios ignorados pelo Git
├── platformio.ini         # Configuração de placas, velocidade série e dependências
└── README.md              # Documentação técnica do projeto

📜 LicençaDistribuído sob a licença MIT. 

Consulte o ficheiro LICENSE para mais detalhes.

Dica para incluir as imagens:

Crie uma pasta chamada docs na raiz do seu projeto.
Tire duas fotos com o telemóvel (uma do circuito na protoboard e outra do LCD a exibir a temperatura).
Guarde-as como circuito.jpg e display.jpg dentro dessa pasta docs.
Faça o commit e push para o GitHub:PowerShell

git add README.md docs/
git commit -m "docs: adiciona documentação profissional e diagrama de pinagem"
git push