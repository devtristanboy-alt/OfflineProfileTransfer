# PS4 to PS5 Transfer Tool

**Estado: protótipo de engenharia v0.1.0 — núcleo de backup testável no Windows; integração de console ainda pendente.**

Projeto comunitário para criar e verificar arquivos de backup de dados selecionados pelo próprio usuário em consoles PS4/PS5 desbloqueados. O repositório não contém SDKs proprietários, payloads, chaves, credenciais, nem código para contornar autenticação ou alterar registros de troféus/contas.

## O que está implementado

- Núcleo C++17 portátil.
- Cópia recursiva de diretórios selecionados pelo usuário.
- Manifesto JSON com caminhos relativos, tamanhos e SHA-256.
- Verificação de integridade.
- Restauração com proteção contra path traversal.
- Restauração não sobrescreve arquivos por padrão.
- Logs de operações.
- Teste/execução de demonstração no Windows 11.

## O que ainda não está implementado

- Aplicação PKG instalável no PS4.
- Aplicação PKG instalável no PS5.
- Renderizador gráfico nativo de cada console.
- Descoberta automática de usuários e IDs locais.
- Acesso aos diretórios internos protegidos do sistema.
- Integração USB nativa de cada firmware.
- Restauração de bases internas de troféus ou dados de conta.
- Compatibilidade validada com firmware PS5 13.60.

Esses itens exigem APIs, toolchains e testes reais por plataforma. O número de firmware informado pelo usuário é um alvo de teste, não uma garantia de compatibilidade.

## Estrutura

```text
PS4-PS5-Transfer-Tool/
├── CMakeLists.txt
├── CMakePresets.json
├── README.md
├── LICENSE
├── .gitignore
├── docs/
│   ├── ARCHITECTURE.md
│   ├── BACKUP_FORMAT.md
│   ├── BUILD_WINDOWS.md
│   ├── CONSOLE_PORTING.md
│   └── SECURITY.md
├── include/transfer/
│   ├── backup_engine.hpp
│   ├── gui_model.hpp
│   ├── platform.hpp
│   ├── sha256.hpp
│   └── types.hpp
├── src/
│   ├── backup_engine.cpp
│   ├── gui_model.cpp
│   ├── main.cpp
│   ├── platform_stub.cpp
│   └── sha256.cpp
├── tests/
│   └── smoke_test.ps1
├── scripts/
│   ├── build-windows.ps1
│   └── run-demo.ps1
└── .github/workflows/
    └── windows-ci.yml
```

## Segurança / escopo

Use apenas em consoles e dados que você possui ou está autorizado a administrar. Faça um backup oficial adicional antes de qualquer restauração. O programa não deve ser executado como ferramenta de migração de credenciais. Arquivos de autenticação (`auth.dat`, `config.dat` e equivalentes) são deliberadamente excluídos do formato padrão. O módulo de troféus nesta versão permite somente copiar e verificar arquivos que o usuário selecionou; não edita troféus, banco de dados ou estado de sincronização.

## Compilar

Consulte [`docs/BUILD_WINDOWS.md`](docs/BUILD_WINDOWS.md). O build atual produz um executável de teste para Windows, não um PKG de console.

## Licença

MIT. Veja `LICENSE`.
