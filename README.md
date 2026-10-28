# Sistemas Embebidos — LPC1769

Workspace de apoio aos laboratórios de Sistemas Embebidos, preparada para o MCUXpresso IDE e para o microcontrolador LPC1769.

## Estrutura

- `CMSIS_CORE_LPC17xx/` — cabeçalhos CMSIS e configuração do LPC17xx.
- `drivers/` — biblioteca partilhada com os gestores de LED e de temporização.
- `labs/lab_01/` a `labs/lab_03/` — projetos dos três primeiros laboratórios.
- `documentation/` — configuração do Doxygen e proposta do gestor LCD do Laboratório 4.

## Abrir noutro computador

1. Clonar este repositório para uma pasta local chamada `SE`.
2. Abrir o MCUXpresso IDE e usar essa pasta como workspace.
3. Selecionar **File → Import → General → Existing Projects into Workspace**.
4. Escolher a pasta `SE` como raiz da pesquisa e ativar a pesquisa de projetos em subpastas.
5. Importar em conjunto `CMSIS_CORE_LPC17xx`, `drivers` e os laboratórios pretendidos.
6. Compilar os projetos importados. O IDE recria as pastas `Debug` e os índices locais.

Os ficheiros `.project` e `.cproject` guardam a configuração de cada projeto. A pasta `.metadata` é criada pelo IDE para cada workspace e não deve ser partilhada. As dependências dos laboratórios usam os nomes dos projetos na workspace, por isso importa `CMSIS_CORE_LPC17xx` e `drivers` juntamente com o laboratório correspondente.

As configurações de depuração (`*.launch`) foram deixadas de fora porque identificam o probe LinkServer deste computador. No outro computador, cria uma configuração de depuração para o respetivo probe.

## Nota sobre o Laboratório 4

`documentation/SE_Lab04_Proposta_Gestor_LCD.pdf` contém a proposta do gestor LCD. Ainda não representa uma implementação do código do Laboratório 4.
