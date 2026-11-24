# Laboratorio 5 - ligacoes do LCD 16x2

Este esquema segue a proposta final do grupo G08: interface de 4 bits e so
escrita. Os numeros `J2-n` identificam contactos do conector da LPCXpresso;
os numeros `LCD n` identificam pinos do modulo.

```mermaid
flowchart LR
    subgraph LPC["LPCXpresso LPC1769 - conector J2"]
      P00["J2-9 / P0.0"]
      P01["J2-10 / P0.1"]
      P02["J2-21 / P0.2"]
      P03["J2-22 / P0.3"]
      P04["J2-38 / P0.4"]
      P05["J2-39 / P0.5"]
      GND["J2-1 ou J2-54 / GND"]
    end
    subgraph DISPLAY["LCD 16x2"]
      DB4["11 / DB4"]
      DB5["12 / DB5"]
      DB6["13 / DB6"]
      DB7["14 / DB7"]
      RS["4 / RS"]
      E["6 / E"]
      RW["5 / R/W"]
      VSS["1 / VSS"]
      VDD["2 / VDD"]
      VO["3 / VEE ou Vo"]
    end
    P00 --> DB4
    P01 --> DB5
    P02 --> DB6
    P03 --> DB7
    P04 --> RS
    P05 --> E
    GND --> RW
    GND --> VSS
    EXT["Fonte regulada +5 V"] --> VDD
    EXT --> POT["Potenciometro 10 kohm: extremos +5 V e GND"]
    GND --> POT
    POT -->|cursor| VO
```

DB0 a DB3 (pinos 7 a 10) ficam sem ligacao. No modulo NMTC-S16205DRGHS,
LEDA e LEDK (pinos 15 e 16) tambem nao sao usados. A fonte de +5 V e a
LPCXpresso devem partilhar GND. J2-2 e uma entrada VIN, nao uma saida de +5 V.

Antes de testar, confirmar o modelo exato do LCD montado e comparar os fios
com este esquema. A ligacao USB so confirma a interface de depuracao da placa.
