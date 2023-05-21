#ifndef FSM_PWM_H_
#define FSM_PWM_H_

#include <stdint.h>
#include <stdbool.h>
#include "fsm.h"

/**
 * @brief Crea una nueva maquina de estados para generar correctamente el PWM y establecer la direccion
 * 
 * @return fsm_t* Puntero a la FSM del PWM
 */
fsm_t * fsm_pwm_new();

/**
 * @brief Inicializa todos los parametros de la FSM para generar correctamente el PWM y su direccion
 * 
 * @param p_fsm Puntero a la FSM del PWM
 */
void fsm_pwm_init(fsm_t *p_fsm);




#endif // FSM_PWM_H_