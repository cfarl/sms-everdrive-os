SMS Everdrive source
====================

Esse projeto é um fork do projeto encontrado em https://github.com/smspower/sms-everdrive-os

Esse firmware funciona no everdrive chinês vendido no Ali Express.

------------------------------------------------------------------------------------
Para configurar o ambiente de desenvolvimento:
------------------------------------------------------------------------------------

1) Instalar SDCC 3.3 na pasta c:\SDCC

- Fazer download do SDCC em https://sourceforge.net/projects/sdcc/files/sdcc-win64/3.3.0/

2) Abrir a pasta "Source" no Visual Studio Code, para alterar os fontes

------------------------------------------------------------------------------------
Para compilar o projeto:
------------------------------------------------------------------------------------

1) Executar o arquivo "_build.bat" no terminal do Visual Studio Code

2) Copiar o arquivo MSOS.mso para o everdrive (esse é o firmware usado no everdrive)

------------------------------------------------------------------------------------
Anotações:
------------------------------------------------------------------------------------

- As ferramentas que estão na pasta "everdrive-sdk" foram obtidas do arquivo "mega-ed-sdk.7z" que está disponível em https://krikzz.com/pub/support/mega-everdrive/v1/dev/

- O arquivo "_clean.bat" pode ser usado para excluir os arquivos gerados na compilação.

