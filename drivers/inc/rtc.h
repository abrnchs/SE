/**
 * @file rtc.h
 * @brief Relogio de tempo real e alarme do LPC1769.
 *
 * As datas e os segundos desta API usam UTC. As datas em struct tm comecam
 * em 1 de janeiro de 1970 e vao ate ao ano 4095. As funcoes que usam time_t
 * ficam ainda limitadas pelo intervalo desse tipo no compilador.
 */
#ifndef DRIVERS_RTC_H
#define DRIVERS_RTC_H

#include <stdbool.h>
#include <time.h>

/**
 * @brief Inicializa o RTC e inicia a contagem a partir de seconds.
 * @param seconds Segundos desde 1970-01-01 00:00:00 UTC.
 * @note Um valor fora do intervalo representavel inicia o RTC na epoca Unix.
 */
void RTC_Init(time_t seconds);

/**
 * @brief Le a data e a hora correntes do RTC.
 * @param dateTime Destino de um struct tm; NULL e ignorado.
 * @details tm_mon comeca em 0, tm_year conta anos desde 1900 e tm_yday
 *          comeca em 0, como definido em time.h.
 */
void RTC_GetTimeDate(struct tm *dateTime);

/**
 * @brief Define a data e a hora correntes do RTC.
 * @param dateTime Data e hora em UTC; NULL ou valores invalidos sao ignorados.
 */
void RTC_SetTimeDate(const struct tm *dateTime);

/**
 * @brief Define a data e a hora usando segundos desde a epoca Unix.
 * @param seconds Segundos desde 1970-01-01 00:00:00 UTC; invalidos sao ignorados.
 */
void RTC_SetSeconds(time_t seconds);

/**
 * @brief Devolve a data e a hora correntes em segundos desde a epoca Unix.
 * @return Segundos desde 1970-01-01 00:00:00 UTC, ou zero se a data
 *         estiver fora do intervalo representavel por time_t.
 */
time_t RTC_GetSeconds(void);

/**
 * @brief Programa um alarme, deixando-o desativado ate RTC_EnableAlarm().
 * @param dateTime Data e hora em UTC; NULL ou valores invalidos sao ignorados.
 * @details So sao comparados dia do mes, mes, ano, hora e minuto.
 */
void RTC_SetAlarm(const struct tm *dateTime);

/** @brief Ativa o alarme programado e a interrupcao do RTC. */
void RTC_EnableAlarm(void);

/** @brief Desativa o alarme e descarta uma ocorrencia ainda nao consultada. */
void RTC_DisableAlarm(void);

/**
 * @brief Consulta e limpa a indicacao de alarme produzida pela interrupcao.
 * @return true se o alarme ocorreu desde a consulta anterior; false caso contrario.
 * @details O alarme e de ocorrencia unica; deve ser novamente ativado para
 *          uma nova ocorrencia.
 */
bool RTC_AlarmTriggered(void);

#endif /* DRIVERS_RTC_H */
