# Space Invaders (C++ / SDL2)

Implementação do clássico Space Invaders do zero, sem game engine, usando C++ e SDL2 puro. Projeto educacional focado em praticar Programação Orientada a Objetos (herança, polimorfismo, encapsulamento, composição) em um ambiente minimalista de terminal Linux.

![status](https://img.shields.io/badge/status-em%20desenvolvimento-yellow)

## Sumário

- [Tecnologias usadas](#tecnologias-usadas)
- [Como rodar](#como-rodar)
  - [Linux](#linux)
  - [Windows](#windows)
- [Controles](#controles)
- [Estrutura do projeto](#estrutura-do-projeto)
- [Arquitetura](#arquitetura)
- [Trabalhos futuros](#trabalhos-futuros)

## Tecnologias usadas

| Tecnologia | Uso no projeto |
|---|---|
| **C++17** | Linguagem principal |
| **CMake** | Sistema de build |
| **SDL2** | Janela, renderer, input, game loop |
| **SDL2_image** | Carregamento de sprites (PNG) |
| **SDL2_ttf** | Renderização de texto (menu, HUD, game over) |
| **SDL2_mixer** | Efeitos sonoros e música de fundo (MP3) |

Ambiente de desenvolvimento original: Ubuntu (Omakub) + Alacritty + Zellij + Neovim (LSP via `clangd`).

## Como rodar

### Linux

**1. Instale as dependências de build e as bibliotecas SDL2:**

```bash
sudo apt update
sudo apt install build-essential cmake pkg-config gdb
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev
```

**2. Clone o repositório:**

```bash
git clone git@github.com:Georgeluks/definitely-not-space-invaders.git
cd space-invaders
```

**3. Configure e compile com CMake:**

```bash
cmake -S . -B build
cmake --build build
```

**4. Execute (a partir da raiz do projeto, por causa dos caminhos relativos dos assets):**

```bash
./build/space_invaders
```

> Sempre que alterar um arquivo `.cpp`/`.hpp`, rode `cmake --build build` novamente antes de executar. Só é necessário rodar `cmake -S . -B build` de novo se o `CMakeLists.txt` for alterado (ex: novo arquivo fonte).

### Windows

O jeito mais simples é usar o **vcpkg** para gerenciar as dependências SDL2, integrado ao CMake.

**1. Instale as ferramentas base:**
- [Visual Studio 2022](https://visualstudio.microsoft.com/) com a carga de trabalho "Desenvolvimento para desktop com C++" (inclui CMake e o compilador MSVC)
- [Git para Windows](https://git-scm.com/download/win)

**2. Instale o vcpkg:**

```powershell
git clone https://github.com/microsoft/vcpkg.git C:\vcpkg
cd C:\vcpkg
.\bootstrap-vcpkg.bat
```

**3. Instale as dependências SDL2 via vcpkg:**

```powershell
.\vcpkg install sdl2 sdl2-image sdl2-ttf sdl2-mixer --triplet x64-windows
```

**4. Clone o repositório do projeto:**

```powershell
git clone git@github.com:SEU_USUARIO/space-invaders.git
cd space-invaders
```

**5. Configure o CMake apontando para o toolchain do vcpkg:**

```powershell
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake
cmake --build build --config Release
```

**6. Execute a partir da raiz do projeto:**

```powershell
.\build\Release\space_invaders.exe
```

> Alternativa: o projeto também abre normalmente no **CLion** ou no **VS Code** (com a extensão CMake Tools), bastando apontar a variável `CMAKE_TOOLCHAIN_FILE` acima nas configurações do CMake da IDE.

## Controles

| Tecla | Ação |
|---|---|
| `←` / `→` | Mover a nave |
| `Espaço` | Atirar / Confirmar no menu / Reiniciar após game over |

## Estrutura do projeto

```
space-invaders/
├── CMakeLists.txt
├── src/
│   ├── main.cpp           # Ponto de entrada
│   ├── Game.hpp/.cpp       # Orquestração: loop, estados, colisões
│   ├── Entity.hpp/.cpp     # Classe base de todas as entidades do jogo
│   ├── Player.hpp/.cpp     # Nave do jogador
│   ├── Enemy.hpp/.cpp      # Inimigo individual
│   ├── EnemyGrid.hpp/.cpp  # Grade coordenada de inimigos
│   ├── Bullet.hpp/.cpp     # Projéteis (player e inimigo)
│   ├── TextRenderer.hpp/.cpp    # Renderização de texto via SDL2_ttf
│   ├── TextureManager.hpp/.cpp  # Cache de texturas via SDL2_image
│   └── AudioManager.hpp/.cpp    # Efeitos sonoros e música via SDL2_mixer
├── assets/
│   ├── sprites/            # Imagens PNG
│   ├── fonts/               # Fontes TTF
│   └── sfx/                 # Áudio (MP3)
└── build/                   # Gerado pelo CMake (ignorado no git)
```

## Arquitetura

O projeto segue um design orientado a objetos com:

- **Herança e polimorfismo**: `Player`, `Enemy` e `Bullet` herdam de `Entity`, sobrescrevendo `update()`/`render()` com comportamento próprio via métodos `virtual`.
- **Composição**: `Game` não implementa a lógica de movimento dos inimigos diretamente — delega para um objeto `EnemyGrid`, que encapsula essa responsabilidade.
- **RAII**: destrutores em cascata garantem liberação correta de memória (`Game` → `EnemyGrid` → `Enemy`s, por exemplo) sem vazamentos.
- **State machine**: o jogo transita entre os estados `Menu`, `Playing` e `GameOver` (`enum class GameState`), cada um com seu próprio `update`/`render`.
- **Gerenciadores de recursos**: `TextureManager` e `AudioManager` centralizam o carregamento/cache de sprites e sons, evitando releituras repetidas do disco.

## Trabalhos futuros

### Gameplay
- [ ] Power-ups (tiro triplo, escudo temporário, vida extra)
- [ ] Dificuldade progressiva por wave (velocidade/frequência de tiro dos inimigos)
- [ ] Sistema de waves — nova grade ao limpar todos os inimigos
- [ ] Padrões de tiro variados por tipo de inimigo
- [ ] Barreiras destrutíveis entre player e inimigos
- [ ] Combo/multiplicador de score por acertos consecutivos

### Arquitetura / boas práticas de C++
- [ ] Substituir `new`/`delete` manuais por smart pointers (`std::unique_ptr`)
- [ ] Extrair um `CollisionSystem` dedicado, separado do `Game`
- [ ] Configuração externa (arquivo `.json`) para velocidades, tamanhos e cores, em vez de valores hardcoded
- [ ] Testes automatizados (Catch2 ou GoogleTest) para lógica de colisão e movimento

### Persistência
- [ ] High score salvo em arquivo local
- [ ] Configurações do jogador (volume, controles) persistidas entre sessões

### Polish visual/sonoro
- [ ] Animações de sprite via spritesheet
- [ ] Efeito de partículas na explosão de inimigos
- [ ] Tela de pause
- [ ] Volume ajustável in-game
- [ ] Suporte a gamepad/controle (`SDL_GameController`)

### Infraestrutura
- [ ] CI no GitHub Actions compilando o projeto a cada push
- [ ] Screenshots/GIF do jogo neste README

## Licença

Projeto pessoal com fins educacionais.
