path = ..\everdrive-sdk\bin;
make clean_sms
make make_sms
type MSOS.SMX font_sms.bin > MSOS.mso
copy MSOS.mso ..\dist\MSOS.mso