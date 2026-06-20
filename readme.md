SMS Everdrive source
====================

Esse projeto é um fork dos fontes para gerar o firmware do everdrive V1 de Master System do Krikzz

Os fontes originais podem ser encontrados aqui: https://github.com/smspower/sms-everdrive-os

Importante: Esse firmware funciona no everdrive chinês vendido no Ali Express.

------------------------------------------------------------------------------------
Configurar ambiente
------------------------------------------------------------------------------------

1) Instalar SDCC 3.3 na pasta c:\SDCC

- Pegar em https://sourceforge.net/projects/sdcc/files/sdcc-win64/3.3.0/

2) Ferramentas que estão na pasta "everdrive-sdk":

- Foram obtidas do arquivo mega-ed-sdk.7z em https://krikzz.com/pub/support/mega-everdrive/v1/dev/

3) Compilar os fontes

- Rodar aquivo build_.bat ou no prompt digitar: 

make clean_sms

make make_sms

4) Pegar o arquivo MSOS.SMS (esse é o firmware usado no everdrive)