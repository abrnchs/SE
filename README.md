# Sistemas Embebidos — LPC1769

Workspace de apoio aos laboratórios de Sistemas Embebidos, preparada para o MCUXpresso IDE e para o microcontrolador LPC1769.

## Estrutura

- `CMSIS_CORE_LPC17xx/` — cabeçalhos CMSIS e configuração do LPC17xx.
- `drivers/` — biblioteca partilhada com os gestores de LED, temporização, LCD, botões, RTC e FLASH.
- `labs/lab_01/` a `labs/lab_03/`, `labs/lab_05/` e `labs/lab_06/` — projetos de laboratório.
- `documentation/` — configuração do Doxygen e esquema de ligações do LCD.

## Abrir noutro computador

1. Clonar este repositório para uma pasta local chamada `SE`.
2. Abrir o MCUXpresso IDE e usar essa pasta como workspace.
3. Selecionar **File → Import → General → Existing Projects into Workspace**.
4. Escolher a pasta `SE` como raiz da pesquisa e ativar a pesquisa de projetos em subpastas.
5. Importar em conjunto `CMSIS_CORE_LPC17xx`, `drivers` e os laboratórios pretendidos.
6. Compilar os projetos importados. O IDE recria as pastas `Debug` e os índices locais.

Os ficheiros `.project` e `.cproject` guardam a configuração de cada projeto. A pasta `.metadata` é criada pelo IDE para cada workspace e não deve ser partilhada. As dependências dos laboratórios usam os nomes dos projetos na workspace, por isso importa `CMSIS_CORE_LPC17xx` e `drivers` juntamente com o laboratório correspondente.

As configurações de depuração (`*.launch`) foram deixadas de fora porque identificam o probe LinkServer deste computador. No outro computador, cria uma configuração de depuração para o respetivo probe.

## Laboratório 5

O projeto `Lab_05` usa a biblioteca `drivers` e a API em `drivers/inc/lcdtext.h`.
As ligações do grupo são P0.0 a P0.5; consultar `documentation/lab_05_ligacoes.md` antes de ligar o LCD. Chamar `DELAY_Init()` antes de `LCD_init()`.
O teste apresenta `TESTE LCD` alternadamente nas duas linhas, durante 1 s em
cada linha, por um total de 10 s. Depois limpa o mostrador por 500 ms e repete.

Para atualizar a documentação local, abrir um terminal na pasta `documentation`
e executar `doxygen documentation_se.doxyfile`. O resultado fica em
`documentation/html/index.html` (ficheiro gerado, excluido do Git).

## Laboratório 6

O projeto `Lab_06` usa os novos gestores em `drivers/inc/buttons.h` e
`drivers/inc/rtc.h`, juntamente com o LCD, o LED verde e a temporização.
As ligações dos três botões e a sequência de teste estão em
`documentation/lab_06_ligacoes.md`. Importar o projeto `Lab_06` no MCUXpresso
juntamente com `drivers` e `CMSIS_CORE_LPC17xx` e compilar primeiro a biblioteca
`drivers`.

## Laboratório 7 (em preparação)

O gestor FLASH já está na biblioteca `drivers`, em `drivers/inc/flash.h` e
`drivers/src/flash.c`. O projeto de teste `Lab_07` ainda não foi criado. Antes
de usar o setor 29 para dados, será preciso reservá-lo no mapa de memória do
novo projeto e reservar os últimos 32 bytes da RAM local para as operações IAP.
