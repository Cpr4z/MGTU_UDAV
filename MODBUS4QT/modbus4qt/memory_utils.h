#ifndef MEMORY_UTILS_H
#define MEMORY_UTILS_H

#include <cstdint>

union WordRec
{
    uint16_t word;
    uint8_t bytes[2];
};


/**
  * @brief
  * @en Retrun hi byte of word
  * @ru Возвращает старший байт двухбайтного целого числа
  *
  * @param
  * @en word
  * @ru word - 2-х байтное слово
  *
  * @return
  * @en Hi byte of word
  * @ru Старший байт двухбайтного целого числа
  */
uint8_t hi(uint16_t word);

/**
  * @brief
  * @en Retrun low byte of word
  * @ru Возвращает младшиц байт двухбайтного целого числа
  *
  * @param
  * @en word
  * @ru word - 2-х байтное слово
  *
  * @return
  * @en Low byte of word
  * @ru Младший байт двухбайтного целого числа
  */
uint8_t lo(uint16_t word);

/**
  * @brief
  * @en Swap two bytes in word and return result
  * @ru Переставляет местами байты в 2-х байтном целом и возвращает результат
  *
  * @param
  * @en word
  * @ru word - 2-х байтное слово
  *
  * @return
  * @en Word with swapped bytes
  * @ru Слово с переставленными байтами
  */
uint16_t swap(uint16_t word);

/**
  * @brief
  * @en Convert 2 bytes word from local byte order to net byte order
  * @ru Конвертирует 16-битную беззнаковую величину из локального порядка байтов в сетевой
  *
  * @param
  * @en word
  * @ru word - 2-х байтное слово
  *
  * @return
  * @en Two bytes word with net byte order
  * @ru Двухбайтное слово с сетевым порядком байт
  */
uint16_t host2net(uint16_t word);

/**
  * @brief
  * @en Convert 2 bytes word from  net byte order to local byte order
  * @ru Конвертирует 16-битную беззнаковую величину из сетевого порядка байтов в локальный
  *
  * @param
  * @en word
  * @ru word - 2-х байтное слово
  *
  * @return
  * @en Two bytes word with local byte order
  * @ru Двухбайтное слово с локальным порядком байт
  */
uint16_t net2host(uint16_t word);

#endif // MEMORY_UTILS_H
