# Sincronização do Metrô

Implementação da atividade 4 de Infraestrutura de Software. O projeto simula a sincronização entre passageiros e carros de metrô por meio de threads, mutexes e variáveis de condição da biblioteca POSIX Threads (`pthread`).

## Objetivo

Garantir que os passageiros só possam embarcar quando um carro estiver na estação e houver assentos disponíveis. A função responsável pelo carro só termina quando todos os passageiros que conseguiram reservar um assento concluíram o embarque e não há mais passageiros aguardando que possam ocupar os assentos restantes.

A implementação considera um único carro na estação por vez, conforme as condições da atividade.

## Arquivos do projeto

| Arquivo | Descrição |
|---|---|
| `sync_car.c` | Implementa a estrutura `station` e as funções de inicialização, chegada do carro, espera dos passageiros e confirmação do embarque. |
| `sync_car-runner.c` | Programa de teste fornecido para exercitar a implementação com threads de passageiros e de carros. O Makefile gera uma cópia temporária adaptada para incluir `sync_car.c`. |
| `Makefile` | Automatiza a compilação do executável `sync_car` e a remoção dos arquivos gerados. |
| `evidencias.log` | Registro de terminal com evidências de compilação e execução dos testes. |

## Como funciona a sincronização

A estrutura `station`, definida em `sync_car.c`, mantém:

- `lock`: mutex único que protege o estado compartilhado da estação.
- `car_arrived`: variável de condição usada para acordar passageiros quando um carro chega.
- `all_boarded`: variável de condição usada para avisar ao carro que os embarques pendentes terminaram.
- `waiting`: quantidade de passageiros aguardando.
- `free_seats`: quantidade de assentos ainda disponíveis no carro.
- `boarding`: quantidade de passageiros que reservaram assento, mas ainda não confirmaram o embarque.
- `car_present`: indica se há um carro na estação.

As funções implementadas são:

- `station_init`: inicializa o mutex, as variáveis de condição e os contadores/indicadores da estação.
- `station_load_car`: registra a chegada do carro, informa a quantidade de assentos, acorda os passageiros e aguarda enquanto houver embarques pendentes ou passageiros esperando que ainda possam ocupar assentos.
- `station_wait_for_car`: registra a chegada de um passageiro e o mantém bloqueado enquanto não houver carro ou assento disponível. Quando pode embarcar, reserva um assento e atualiza os contadores.
- `station_on_board`: registra a conclusão do embarque de um passageiro. Quando não restam embarques pendentes, sinaliza a variável de condição do carro.

O acesso às informações compartilhadas é protegido pelo mutex. As esperas são feitas com `pthread_cond_wait`, sem laços de espera ativa (*busy-wait*). O uso de `pthread_cond_broadcast` permite acordar os passageiros quando o carro chega, possibilitando que vários passageiros prossigam e reservem assentos de forma concorrente, com o estado protegido pelo mutex.

## Requisitos

- Compilador C compatível com GCC.
- `make`.
- Biblioteca POSIX Threads (`pthread`).
- Sistema operacional compatível com POSIX: Linux, Unix ou macOS.

A atividade solicita as primitivas `pthread_mutex_init`, `pthread_mutex_lock`, `pthread_mutex_unlock`, `pthread_cond_init`, `pthread_cond_signal`, `pthread_cond_wait` e `pthread_cond_broadcast`, sem uso de semáforos ou de outras primitivas de sincronização.

## Compilação

No terminal, entre no diretório do projeto e execute:

```bash
make
```

O Makefile utiliza:

```make
CC = gcc
CFLAGS = -Wall -Wextra -pthread
```

Durante a compilação, o Makefile substitui no arquivo de teste a inclusão de `sync_car.h` por `sync_car.c`, gera temporariamente `runner_build.c`, compila esse arquivo e remove a cópia temporária. O resultado é o executável `sync_car`.

## Execução

Depois de compilar, execute:

```bash
./sync_car
```

O programa cria threads de passageiros e simula a chegada de carros com diferentes quantidades de assentos. A saída informa quantos passageiros embarcaram em cada carro. Ao final, a mensagem `Looks good!` indica que o teste principal terminou com a quantidade esperada de passageiros.

## Testes registrados

O arquivo `evidencias.log` registra:

1. Compilação com `make`, utilizando GCC e a opção `-pthread`.
2. Execução de `./sync_car`, com embarques em carros com diferentes quantidades de assentos.
3. Verificação de que a quantidade de passageiros embarcados corresponde à quantidade esperada em cada rodada, finalizando com `Looks good!`.
4. Repetição da execução 100 vezes, sem registro de falha no laço de teste.
5. Execução de um teste adicional (`teste_extra`), cujo registro apresenta a mensagem `Extras OK: todos retornaram imediatamente`.

O teste adicional aparece no registro de evidências, mas o respectivo arquivo-fonte não está entre os arquivos listados no repositório.

## Limpeza

Para remover o executável e o arquivo temporário de compilação, execute:

```bash
make clean
```

## Ambiente de desenvolvimento

As evidências de terminal indicam compilação e execução em ambiente Linux acessado pelo WSL, com GCC e Make. O registro não informa a distribuição Linux nem a versão das ferramentas.

---

**Atividade:** Infraestrutura de Software — Implementação 4: Sincronização do Metrô.
