/**
 * @file fsm_pwm.c
 * @author Javier y Manuel
 * @brief Maquina de estados del Skate
 * @version 1.0
 * @date 2023-05-23
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <stddef.h>
#include <stdlib.h>
#include "fsm_pwm.h"
#include "stm32f4xx_hal.h" 
#include "stm32f4xx_hal_adc.h"
#include <stdio.h>
#include "main.h"
#include "MY_NRF24.h" //Hal driver del NRF

/**
 * @brief Estructura de la FSM del Skate-PWM
 * 
 */
typedef struct
{
    fsm_t fsm;              /*!< Internal FSM from the library */
    uint32_t adcVal;        /*!< Valor del Potenciometro ADC de 0-255 */
    bool ack[1];               /*!< AcK de vuelta */
    uint32_t myRxData[32]; /*!< Buffer de recepcion*/
} fsm_pwm_t;

/**
 * @brief Comprueba que el interruptor de cambio de sentido ha sido pulsado,
 * y cambia la direccion de giro hacia adelante
 * 
 * @param p_fsm Puntero a la FSM del PWM
 * @return true 
 * @return false 
 */
bool check_boton_true(fsm_t *p_fsm)
{
    fsm_pwm_t * p_pwm = ( fsm_pwm_t *) p_fsm ;
    if((p_pwm -> myRxData[1]) ==  1)
    {
        return true;
    }
    else{
        return false;
    }
}

/**
 * @brief Comprueba que el interruptor de cambio de sentido ha sido pulsado,
 * y cambia la direccion de giro hacia atras
 * 
 * @param p_fsm Puntero a la FSM del PWM
 * @return true 
 * @return false 
 */
bool check_boton_false(fsm_t *p_fsm)
{
    fsm_pwm_t * p_pwm = ( fsm_pwm_t *) p_fsm ;
    if(((p_pwm -> myRxData[1]) ==  0))
    {
        return true;
    }
    else{
        return false;
    }
}


/**
 * @brief Recibe la informacion del emisor, actualiza los valores y genera el PWM para retroceder
 * 
 * @param p_fsm 
 */
void do_recibe_acciona_motores_atras(fsm_t *p_fsm)
{
fsm_pwm_t * p_pwm = ( fsm_pwm_t *) p_fsm ;
//RECEPCION
if(NRF24_available())
   {
    NRF24_read(p_pwm -> myRxData, 32);
    //ACK
    p_pwm -> ack[0] = true;
		NRF24_writeAckPayload(1, p_pwm -> ack, 1);//Mandar ACK de vuelta true
   }

//GENERA PWM
  htim1.Instance->CCR1  = (uint32_t)1520 - (p_pwm -> myRxData[0] * (uint32_t)2.666);
    

}

/**
 * @brief Recibe la informacion del emisor, actualiza los valores y genera el PWM para avanzar
 * 
 * @param p_fsm 
 */
void do_recibe_acciona_motores_adelante(fsm_t *p_fsm)
{
fsm_pwm_t * p_pwm = ( fsm_pwm_t *) p_fsm ;
//RECEPCION
if(NRF24_available())
   {
    NRF24_read(p_pwm -> myRxData, 32);
    //ACK
    p_pwm -> ack[0] = true;
		NRF24_writeAckPayload(1, p_pwm -> ack, 1);//Mandar ACK de vuelta true
   }
//GENERA PWM
  htim1.Instance->CCR1  = (uint32_t)1520 + (p_pwm -> myRxData[0] * (uint32_t)2.666);
    
}

/**
 * @brief Estados de FSM PWM
 * 
 */
enum FSM_PWM_STATES
{
    FORWARD ,
    BACKWARD

};

static fsm_trans_t fsm_trans_pwm[] = {
    { FORWARD , check_boton_false , BACKWARD , do_recibe_acciona_motores_atras },
 { BACKWARD , check_boton_true , FORWARD , do_recibe_acciona_motores_adelante },
 { BACKWARD , check_boton_false , BACKWARD, do_recibe_acciona_motores_atras },
 { FORWARD , check_boton_true , FORWARD , do_recibe_acciona_motores_adelante },
 { -1 , NULL , -1, NULL }
};

fsm_t *fsm_pwm_new()
{
fsm_t *p_fsm = malloc(sizeof(fsm_pwm_t));
    if (p_fsm)
    {
        fsm_pwm_init(p_fsm);
    }
    return p_fsm;
}

void fsm_pwm_init(fsm_t *p_fsm)
{
    fsm_pwm_t * p_pwm= ( fsm_pwm_t *) p_fsm ;
 fsm_init (& p_pwm -> fsm , fsm_trans_pwm );
 p_pwm -> adcVal = 0;
 p_pwm -> ack[0] = false;
 p_pwm -> myRxData[0] =  0;
 p_pwm -> myRxData[1] =  0;
 
}