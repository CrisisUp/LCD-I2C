#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>


// Configuração do ecrã LCD I2C (endereço padrão 0x27, 16 colunas e 2 linhas)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Configuração do sensor DHT22 no GPIO 23
#define DHTPIN 23
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Ícone de Termómetro (matriz 5x8)
byte tempIcon[8] = {
  0b00100,
  0b01010,
  0b01010,
  0b01110,
  0b01110,
  0b11111,
  0b11111,
  0b01110
};

// Ícone de Gota de Água (matriz 5x8)
byte dropIcon[8] = {
  0b00100,
  0b00100,
  0b01010,
  0b01010,
  0b10001,
  0b10001,
  0b10001,
  0b01110
};

// Símbolo de Grau (°)
byte degreeIcon[8] = {
  0b00110,
  0b01001,
  0b01001,
  0b00110,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

// Temporização não-bloqueante (2500 ms = intervalo ideal para o DHT22)
unsigned long previousMillis = 0;
const unsigned long interval = 2500;

void setup() {
  // Inicialização da comunicação série para depuração
  Serial.begin(115200);

  // Inicialização do barramento I2C do ESP32 (SDA = GPIO 21, SCL = GPIO 22)
  Wire.begin(21, 22);

  // Ativação do pull-up interno no pino do DHT22 para assegurar a estabilidade do sinal
  pinMode(DHTPIN, INPUT_PULLUP);

  // Inicialização do ecrã LCD
  lcd.init();
  lcd.backlight();

  // Registo dos carateres personalizados na memória CGRAM
  lcd.createChar(0, tempIcon);
  lcd.createChar(1, dropIcon);
  lcd.createChar(2, degreeIcon);

  // Inicialização do sensor DHT22
  dht.begin();

  // Ecrã de arranque
  lcd.setCursor(0, 0);
  lcd.print("Estacao Clima");
  lcd.setCursor(0, 1);
  lcd.print("Iniciando...");
  Serial.println("Sistema iniciado. Aguardando primeira leitura...");
  delay(1500);
  lcd.clear();
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Leitura da humidade e temperatura
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature(); // Leitura em graus Celsius

    // Validação de erro de leitura
    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("[ERRO] Falha ao comunicar com o DHT22. Verifique o GPIO 23 e a alimentacao.");
      lcd.setCursor(0, 0);
      lcd.print("Erro no sensor! ");
      lcd.setCursor(0, 1);
      lcd.print("Checar GPIO 23  ");
      return;
    }

    // Saída no Monitor Série para confirmação
    Serial.print("Temperatura: ");
    Serial.print(temperature, 1);
    Serial.print(" *C | Humidade: ");
    Serial.print(humidity, 1);
    Serial.println(" %");

    // Linha 0: Ícone e Temperatura (ex.: [Termo] Temp: 24.5°C)
    lcd.setCursor(0, 0);
    lcd.write(byte(0));
    lcd.print(" Temp: ");
    lcd.print(temperature, 1);
    lcd.write(byte(2));
    lcd.print("C  ");

    // Linha 1: Ícone e Humidade (ex.: [Gota] Umid: 58.2%)
    lcd.setCursor(0, 1);
    lcd.write(byte(1));
    lcd.print(" Umid: ");
    lcd.print(humidity, 1);
    lcd.print("%   ");
  }
}

/*

namespace Config {
    constexpr uint8_t I2C_SDA_PIN = 21;
    constexpr uint8_t I2C_SCL_PIN = 22;
    constexpr uint8_t DHT_DATA_PIN = 23;
    constexpr uint8_t LCD_ADDRESS = 0x27;
    constexpr uint8_t LCD_COLUMNS = 16;
    constexpr uint8_t LCD_ROWS = 2;

    constexpr unsigned long SENSOR_POLL_INTERVAL_MS = 2500;
    constexpr unsigned long SERIAL_BAUD_RATE = 115200;
}

namespace DisplayCustomGlyphs {
    constexpr uint8_t GLYPH_INDEX_TEMP = 0;
    constexpr uint8_t GLYPH_INDEX_DROP = 1;
    constexpr uint8_t GLYPH_INDEX_DEGREE = 2;

    const uint8_t THERMOMETER[8] = {
        0b00100, 0b01010, 0b01010, 0b01110,
        0b01110, 0b11111, 0b11111, 0b01110
    };

    const uint8_t WATER_DROP[8] = {
        0b00100, 0b00100, 0b01010, 0b01010,
        0b10001, 0b10001, 0b10001, 0b01110
    };

    const uint8_t DEGREE_SYMBOL[8] = {
        0b00110, 0b01001, 0b01001, 0b00110,
        0b00000, 0b00000, 0b00000, 0b00000
    };
}

struct ClimateSample {
    float temperatureCelsius = 0.0f;
    float humidityPercent = 0.0f;
    bool isValid = false;
};

// Instâncias de Hardware
LiquidCrystal_I2C lcd(Config::LCD_ADDRESS, Config::LCD_COLUMNS, Config::LCD_ROWS);
DHT dht(Config::DHT_DATA_PIN, DHT22);

// Protótipos das Funções
void initializeDisplay();
ClimateSample acquireClimateSample();
void renderToLCD(const ClimateSample& sample);
void logTelemetryToSerial(const ClimateSample& sample);

void setup() {
    Serial.begin(Config::SERIAL_BAUD_RATE);
    Wire.begin(Config::I2C_SDA_PIN, Config::I2C_SCL_PIN);
    pinMode(Config::DHT_DATA_PIN, INPUT_PULLUP);

    initializeDisplay();
    dht.begin();

    Serial.println(F("[SISTEMA] Inicializacao concluida com sucesso."));
}

void loop() {
    static unsigned long lastAcquisitionTimestamp = 0;
    const unsigned long currentTimestamp = millis();

    if (currentTimestamp - lastAcquisitionTimestamp >= Config::SENSOR_POLL_INTERVAL_MS) {
        lastAcquisitionTimestamp = currentTimestamp;

        const ClimateSample currentSample = acquireClimateSample();
        
        renderToLCD(currentSample);
        logTelemetryToSerial(currentSample);
    }
}

// -------------------------------------------------------------
// Implementação dos Módulos
// -------------------------------------------------------------

void initializeDisplay() {
    lcd.init();
    lcd.backlight();

    lcd.createChar(DisplayCustomGlyphs::GLYPH_INDEX_TEMP, const_cast(DisplayCustomGlyphs::THERMOMETER));
    lcd.createChar(DisplayCustomGlyphs::GLYPH_INDEX_DROP, const_cast(DisplayCustomGlyphs::WATER_DROP));
    lcd.createChar(DisplayCustomGlyphs::GLYPH_INDEX_DEGREE, const_cast(DisplayCustomGlyphs::DEGREE_SYMBOL));

    lcd.setCursor(0, 0);
    lcd.print("Estacao Clima");
    lcd.setCursor(0, 1);
    lcd.print("Iniciando...");
    delay(1200);
    lcd.clear();
}

ClimateSample acquireClimateSample() {
    ClimateSample sample;
    sample.humidityPercent = dht.readHumidity();
    sample.temperatureCelsius = dht.readTemperature();

    // Guard Clause de validação
    if (isnan(sample.humidityPercent) || isnan(sample.temperatureCelsius)) {
        sample.isValid = false;
        return sample;
    }

    sample.isValid = true;
    return sample;
}

void renderToLCD(const ClimateSample& sample) {
    if (!sample.isValid) {
        lcd.setCursor(0, 0);
        lcd.print("Erro no sensor! ");
        lcd.setCursor(0, 1);
        lcd.print("Checar GPIO 23  ");
        return;
    }

    // Linha 0: Temperatura
    lcd.setCursor(0, 0);
    lcd.write(byte(DisplayCustomGlyphs::GLYPH_INDEX_TEMP));
    lcd.print(" Temp: ");
    lcd.print(sample.temperatureCelsius, 1);
    lcd.write(byte(DisplayCustomGlyphs::GLYPH_INDEX_DEGREE));
    lcd.print("C  ");

    // Linha 1: Umidade
    lcd.setCursor(0, 1);
    lcd.write(byte(DisplayCustomGlyphs::GLYPH_INDEX_DROP));
    lcd.print(" Umid: ");
    lcd.print(sample.humidityPercent, 1);
    lcd.print("%   ");
}

void logTelemetryToSerial(const ClimateSample& sample) {
    if (!sample.isValid) {
        Serial.println(F("[ERRO] Falha de comunicacao com o sensor DHT22."));
        return;
    }

    Serial.print(F("[TELEMETRIA] Temp: "));
    Serial.print(sample.temperatureCelsius, 1);
    Serial.print(F(" C | Umid: "));
    Serial.print(sample.humidityPercent, 1);
    Serial.println(F(" %"));
}


*/


/*

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Matriz de bytes para criar o caractere de coração (5x8 pixels)
byte heartIcon[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};

// Variáveis de controle de tempo não-bloqueante
unsigned long previousMillis = 0;
const long interval = 500; // Atualiza a cada 500 ms
int progress = 0;

// Função auxiliar para efeito de digitação letra por letra
void typewriteText(const char* text, int col, int row, int delayMs) {
  lcd.setCursor(col, row);
  for (int i = 0; text[i] != '\0'; i++) {
    lcd.print(text[i]);
    delay(delayMs);
  }
}

void setup() {
  Wire.begin(21, 22); // Pinos I2C padrão do ESP32: SDA=21, SCL=22
  lcd.init();
  lcd.backlight();

  // Registra o caractere customizado na posição 0 da memória CGRAM
  lcd.createChar(0, heartIcon);

  // Animação de boas-vindas
  typewriteText("Iniciando ESP32", 0, 0, 80);
  typewriteText("Carregando...", 0, 1, 60);
  delay(1200);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ESP32 ");
  lcd.write(byte(0)); // Desenha o coração
  lcd.print(" LCD I2C");
}

void loop() {
  unsigned long currentMillis = millis();

  // Atualiza as animações a cada intervalo
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Cálculo do tempo de funcionamento (Uptime)
    unsigned long totalSeconds = currentMillis / 1000;
    int seconds = totalSeconds % 60;
    int minutes = (totalSeconds / 60) % 60;
    int hours = (totalSeconds / 3600);

    // Linha 0: Uptime formatado (ex.: "Up: 00:01:25")
    lcd.setCursor(0, 0);
    char timeBuffer[17];
    snprintf(timeBuffer, sizeof(timeBuffer), "Up: %02d:%02d:%02d ", hours, minutes, seconds);
    lcd.print(timeBuffer);

    // Linha 1: Barra de carregamento contínua de 16 blocos
    lcd.setCursor(0, 1);
    for (int i = 0; i < 16; i++) {
      if (i <= progress) {
        lcd.write(255); // 255 é o caractere de bloco cheio nativo da controladora HD44780
      } else {
        lcd.print(" ");
      }
    }

    // Avança e reinicia o ciclo da barra
    progress++;
    if (progress >= 16) {
      progress = 0;
    }
  }
}


*/

/*

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
    lcd.init();
    lcd.backlight();

    lcd.setCursor(0, 0);
    lcd.print("Ola, turma!");

    lcd.setCursor(0, 1);
    lcd.print("ESP32 + LCD");
}

void loop() {
    
}

*/