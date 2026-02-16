# 🌡️ MQTT Temperature Monitoring with Raspberry Pi 3 & Home Assistant (KNX)

Version : 12.02.2026  
Author : Manitra  

## 🎯 Objectif du projet

Récupérer les données d'un capteur de température **DHT11** connecté à un **Raspberry Pi 3**, 
puis les envoyer via **MQTT/TCP** vers un **Raspberry Pi 5** exécutant **Home Assistant**.

Home Assistant utilise ensuite ces données pour déclencher automatiquement un ventilateur 
connecté au réseau **KNX-IP** lorsque la température dépasse un certain seuil.

➡️ Le ventilateur est activé lorsque la température est ≥ 21°C pendant 5 minutes.
➡️ Il reste actif pendant 1 heure.

## 🧱 Architecture

[DHT11]
   ↓
[Raspberry Pi 3]
   ↓ MQTT
[Home Assistant - Raspberry Pi 5]
   ↓ KNX/IP
[Ventilateur]
