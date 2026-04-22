# Otimizador de Carregamento para Mass Effect 2 (Refatorado 2026)

![C++](https://img.shields.io/badge/C++-17-blue.svg) ![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey.svg) ![License](https://img.shields.io/badge/License-MIT-green.svg)

Um utilitário de sistema de alta performance projetado para corrigir o infame bug das telas de carregamento infinitas no **Mass Effect 2 (Legacy Edition)**. Este projeto é uma refatoração de 2026 da minha ferramenta original de 2021, implementando padrões modernos de C++ e interações mais seguras com a API do Windows.

## 🔴 O Problema: Stall de Sincronização da Unreal Engine 3
O Mass Effect 2, construído na **Unreal Engine 3**, sofre de um gargalo de sincronização em CPUs modernas de múltiplos núcleos. Durante as telas de carregamento, a lógica de descompressão de assets e sincronização de vídeos trava enquanto tenta coordenar threads em vários núcleos. Isso resulta em tempos de espera significativamente maiores do que o necessário em hardware atual.

## 🟢 A Solução: Otimização de Afinidade de CPU
Ao forçar o processo do jogo a rodar em um único núcleo lógico (**Núcleo 1**) durante a fase de carregamento, eliminamos o overhead de troca de threads e as condições de corrida (race conditions) de sincronização.

**Este utilitário automatiza o processo:**
1. Identifica o caminho do jogo via um arquivo `.ini` local ou pelo Registro do Windows.
2. Inicia o jogo em estado suspenso para definir a **Máscara de Afinidade de CPU para 2 (0010 em binário)**.
3. Monitora uma tecla de atalho (**F9**) definida pelo usuário para restaurar todos os núcleos assim que o carregamento termina, garantindo FPS máximo durante o gameplay.

## 🛠️ Destaques da Refatoração 2026
Comparado à versão de 2021, esta refatoração introduz melhorias de nível profissional:
* **Manipulação Dinâmica de Registro:** Substituição de buffers de tamanho fixo por alocação dinâmica de memória, evitando estouros de buffer (overflows) ao ler caminhos longos do Registro do Windows.
* **Padrões Modernos C++17:** Utiliza a biblioteca `<filesystem>` para manipulação robusta de caminhos e navegação de diretórios.
* **API Nativa do Windows:** Troca de chamadas `system()` por `CreateProcessA` para gerenciamento direto de processos, proporcionando melhor controle de handles e segurança.
* **Monitoramento em Tempo Real:** Implementa `GetAsyncKeyState` para detecção de teclas de atalho em segundo plano, mesmo com o jogo em foco de tela cheia.

## 🚀 Como Compilar
Para compilar o inicializador usando o **MinGW (g++)**, execute o seguinte comando no terminal:

'''bash
g++ -O3 -std=c++17 main.cpp -o me2lsf.exe -luser32
'''

## 📖 Como Usar
* Opcionalmente, crie e defina o caminho do seu jogo no arquivo `me2lsf.ini`. Caso o arquivo não esteja presente, o programa localizará o jogo automaticamente via Registro do Windows.
* Execute '''me2lsf.exe'''. (Nota: O arquivo `.ini` tem prioridade; ele funciona como uma sobreposição e ignora a detecção automática do Registro se estiver presente).
* Pressione **[F9]** assim que a tela de carregamento terminar para restaurar a performance total da CPU.

---
**Desenvolvido por [0xRobert](https://github.com/zahanzo)**
*Estudante de Análise e Desenvolvimento de Sistemas | Entusiasta de Programação de Baixo Nível*