# Lab 06 — ligações e teste

[Ver esquema ilustrado das ligações](lab_06_esquema.svg).

[Ver mapa completo do LCD, dos botões e da LPCXpresso](lab_06_mapa_completo.svg).

O módulo de três botões contém interruptores simples. Cada botão liga uma
entrada GPIO ao GND quando é premido; os pull-ups são ativados por software.

Nas fotografias de 18/11/2026, o botão A está em cima e C em baixo:
**CN1 é a coluna de furos à direita e CN2 é a coluna à esquerda**. O pino 1
é o furo superior de cada coluna; os pinos 3 e 5 são, respetivamente, o
terceiro e o quinto furo a contar de cima. Os esquemas ilustrados mostram o
módulo rodado, com A à esquerda e C à direita.

O módulo da montagem atual tem **pinos soldados por baixo** de CN1 e CN2 e
inseridos na breadboard, embora na fotografia superior só se vejam os furos.
Cada jumper deve entrar **na mesma linha horizontal da breadboard** que o
pino correspondente do módulo. Um furo da linha imediatamente acima ou abaixo
fica eletricamente separado. As duas breadboards encostadas não unem
eletricamente as suas linhas nem os seus barramentos de alimentação: é preciso
um jumper explícito para atravessar a junção quando necessário.

| Botão | Lado ligado ao GND | Lado ligado à LPCXpresso | Entrada |
|---|---|---|---|
| A | CN2-1 | CN1-1 → J2-42 | P2.0 |
| B | CN2-3 | CN1-3 → J2-43 | P2.1 |
| C | CN2-5 | CN1-5 → J2-44 | P2.2 |

CN2-1, CN2-3 e CN2-5 podem partilhar o mesmo GND da LPCXpresso. O LCD mantém
as ligações usadas no Laboratório 5. O LED verde está na própria placa, ligado
a P3.25; não precisa de um fio externo.

Num teste inicial, P2.0–P2.2 permaneceram em `111` mesmo com A premido.
Depois de rever as ligações, o funcionamento dos botões foi confirmado na
placa. Caso volte a falhar, verifica o GND comum e as linhas de CN1/CN2.
Uma ligação temporária entre J2-42 (P2.0) e GND J2-54 deve mudar a leitura
de `111` para `110` e fazer `BUTTONS_Wait()` devolver `BUTTONS_A` (valor 1).

## O que aparece ao executar `Lab_06`

1. O LCD pede para premir A, B ou C. `BUTTONS_Wait()` espera e mostra o
   número obtido. Os valores 1, 2 e 4 correspondem a A, B e C; uma combinação
   soma os bits dos botões premidos.
2. Ao soltar o botão, o RTC começa em **17/11/2026 12:00:00 UTC**. A primeira
   linha mostra data e hora; a segunda mostra segundos, o valor devolvido por
   `BUTTONS_Read()` após `B:` e o estado do alarme.
3. Sem carregar em C, às **12:03:00** aparece `DISPAROU` e o LED verde acende
   durante 2 segundos.
4. Para testar o cancelamento, reinicia a placa e carrega em C antes das
   12:03:00. Aparece `CANCELADO` e o LED fica apagado.

O RTC conta a partir de uma data fixa de teste; não obtém a hora do computador.
