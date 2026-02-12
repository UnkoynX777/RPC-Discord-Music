# 🎵 RPC Discord Music

Mostre o que você está ouvindo no seu PC diretamente no seu perfil do Discord com integração rica e automática.

Desenvolvido para ser leve, eficiente e invisível.

## 📋 Sobre o Projeto
O **RPC Discord Music** é uma aplicação standalone escrita em C++ moderno (C++20/WinRT) que detecta automaticamente a mídia sendo reproduzida no Windows (Spotify, Chrome, Edge, etc.) e atualiza seu Status do Discord (Rich Presence).
Diferente de outros bots ou plugins, este projeto roda nativamente no Windows, consumindo recursos mínimos e sem necessidade de configurações complexas.

## ✨ Funcionalidades

*   **🎧 Detecção Universal:** Suporte para qualquer player que use o Windows Media Overlay (Spotify, Web Players, Deezer, etc.).
*   **🚀 Leve e Rápido:** Escrito em C++ nativo, roda minimizado na bandeja do sistema.
*   **⚙️ Configurável:** Menu na bandeja para ativar/desativar o RPC rapidamente.
*   **Tray Icon:** Feedback visual do status (Ativado/Desativado) diretamente no tooltip do ícone.
*   **Logs:** Sistema de logs integrado para fácil diagnóstico.

## 🛠️ Como Compilar

### Pré-requisitos
*   **Visual Studio 2019 ou 2022** (Workload C++ Desktop).
*   **Windows 10/11 SDK**.
*   **Discord RPC Library**.

### Passo a Passo

1.  **Clone o repositório:**
    ```bash
    git clone https://github.com/UnkoynX777/RPC-Discord-Music/RPC-Discord-Music.git
    cd RPC-Discord-Music
    ```

2.  **Configure as dependências:**
    *   Baixe a release do [discord-rpc](https://github.com/discord/discord-rpc/releases).
    *   Extraia e copie `win64-dynamic/lib/discord-rpc.lib` para `RPC-Discord-Music/libs/discord-rpc/lib/`.
    *   *Nota:* Os headers já estão inclusos no projeto.

3.  **Compile:**
    *   Abra `RPC-Discord-Music.slnx` no Visual Studio.
    *   Selecione a configuração **Release / x64**.
    *   Compile a solução (`Ctrl+Shift+B`).

4.  **Finalização:**
    *   Copie a DLL `win64-dynamic/bin/discord-rpc.dll` para a pasta onde o executável foi gerado (ex: `x64/Release/`).

## 🚀 Como Usar

1.  Execute o `RPC-Discord-Music.exe`.
2.  Um ícone aparecerá na bandeja do sistema (perto do relógio).
3.  Abra seu player de música favorito (Spotify, Youtube Music no Chrome, etc.).
4.  O status do Discord será atualizado automaticamente!
5.  Clique com o botão direito no ícone da bandeja para:
    *   **Enable RPC:** Ativar ou desativar a integração.
    *   **Exit:** Fechar o aplicativo.

## 💻 Tecnologias

*   **C++20**
*   **C++/WinRT** (Windows Media API)
*   **Win32 API** (Tray Icon & Windowing)
*   **Discord RPC**

## 📄 Licença
Este projeto é livre para uso e estudos. Sinta-se à vontade para contribuir!

---
Feito com 💜 por [UnkoynX7](https://github.com/UnkoynX777)