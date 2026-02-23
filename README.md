# Ber-o-Inteligente-


Sistema de Berço Inteligente baseado em Arduino que monitora temperatura, umidade, som e movimento do bebê. Controla ventilador, música automática e servo para balanço, além de enviar alertas por SMS. Possui display LCD para visualização em tempo real e comandos remotos via mensagem.

---

# 📘 README – Berço Inteligente com Arduino

## 📖 Sobre o Projeto

O **Berço Inteligente** é um sistema automatizado que monitora as condições do ambiente do bebê e reage automaticamente para garantir conforto e segurança.

O sistema utiliza sensores de temperatura, umidade, som e movimento, além de ventilação automática, música calmante, balanço com servo motor e envio de alertas por SMS.

---

## ⚙️ Funcionalidades

* 🌡 Monitoramento de temperatura (DHT11)
* 💨 Controle automático do ventilador
* 💧 Alerta de umidade elevada
* 🎵 Reprodução automática de música ao detectar choro
* 👶 Detecção de movimento com sensor PIR
* 🔄 Balanço automático com servo motor
* 📲 Envio de alertas por SMS
* 📟 Exibição de informações em LCD I2C
* 📩 Comandos remotos via SMS

---

## 🛠️ Componentes Utilizados

* Arduino
* Sensor DHT11
* Sensor PIR
* Sensor de Som
* Sensor de Umidade
* Servo Motor
* Módulo GSM
* DFPlayer Mini
* Display LCD I2C
* Ventilador

---

## 🔌 Funcionamento do Sistema

### 🌡 Controle de Temperatura

* Se temperatura > 30°C → Ventilador liga
* Se temperatura < 28°C → Ventilador desliga

### 💧 Umidade

* Detecta solo molhado
* Envia SMS de alerta a cada 10 segundos (intervalo de segurança)

### 🎵 Som Detectado

* Ao detectar choro:

  * Música é reproduzida por 30 segundos
  * Envia SMS notificando

### 👶 Movimento Detectado

* Servo realiza balanço por 30 segundos
* Após ciclo, desliga automaticamente

---

## 📲 Comandos por SMS

Você pode enviar:

* `DESLIGA VENTILADOR`
* `DESLIGA SERVO`

---

## 📟 Informações no LCD

O visor mostra:

* Temperatura atual
* Estado da umidade (Molhado/Seco)
* Status do Ventilador
* Status da Música
* Status do Balanço

---

## 🚀 Como Usar

1. Conecte todos os sensores conforme os pinos definidos.
2. Insira o número de telefone no código.
3. Carregue o programa no Arduino.
4. Ligue o sistema.
5. Monitore pelo LCD ou receba alertas por SMS.

---

## 🔐 Segurança Implementada

* Histerese na temperatura
* Debounce no sensor PIR
* Intervalo entre alertas de umidade
* Tempo máximo de funcionamento do servo

---


