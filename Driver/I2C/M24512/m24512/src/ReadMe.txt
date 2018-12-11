0x50 I2C adresinde olan  EEPROM.
Bu EEPROM'a yazabilmek için WC pininin low sürülmesi gerekmektedir.
EEPROM_WC_L hattını 0x74 adresindeki I2C Expander sürmekterdir.
BU yazılımı koşturmada önce IOExpander.c koşturulmalıdır.