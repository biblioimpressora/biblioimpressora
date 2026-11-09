
#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>

#include "relogio.h"
#include "configuracoes.h"

static RTC_DS3231 rtc;

static bool rtcConectado = false;
static bool relogioPrecisaAjuste = false;

bool inicializarRelogio() {

    Wire.begin(PIN_RTC_SDA, PIN_RTC_SCL);

    rtcConectado = rtc.begin();

    if (!rtcConectado) {
        return false;
    }

    if (rtc.lostPower()) {
        relogioPrecisaAjuste = true;

        Serial.println("AVISO: o RTC perdeu a alimentacao.");
        Serial.println("A data e hora precisam ser configuradas.");

    } else {
        relogioPrecisaAjuste = false;
    }

    return true;
}

bool obterDataHora(DataHora &dataHora) {

    if (!rtcConectado || relogioPrecisaAjuste) {
        return false;
    }

    DateTime agora = rtc.now();

    if (!agora.isValid()) {
        return false;
    }

    dataHora.ano = agora.year();
    dataHora.mes = agora.month();
    dataHora.dia = agora.day();
    dataHora.hora = agora.hour();
    dataHora.minuto = agora.minute();
    dataHora.segundo = agora.second();

    return true;
}

bool ajustarRelogio(
    uint16_t ano,
    uint8_t mes,
    uint8_t dia,
    uint8_t hora,
    uint8_t minuto,
    uint8_t segundo
) {

    if (!rtcConectado) {
        return false;
    }

    DateTime novaDataHora(
        ano,
        mes,
        dia,
        hora,
        minuto,
        segundo
    );

    if (!novaDataHora.isValid()) {
        return false;
    }

    rtc.adjust(novaDataHora);
    relogioPrecisaAjuste = false;

    return true;
}