#include <stdio.h>
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "bme280.h"
#include "display.h"
#include "gpio.h"
#include "pid.h"
#include "thermometer.h"
#include "uart.h"

#define true 1
#define false 0

int use_terminal = false;
struct bme280_dev bme_connection;
int uart_filesystem, key_gpio = 1;

void shutdown_program() {
  system("clear");
  printf("Programa finalizado\n");
  turn_resistance_off();
  turn_fan_off();
  close_uart(uart_filesystem);
  exit(0);
}


void pid_routine(int key) {
  system("clear");
  float TI, TR, TE;
  int value_to_send = 0;
  printf("\n================== Iniciada rotina PID ==================\n");
  pid_setup(20, 0.1, 100);
  do {
    write_uart_get(uart_filesystem, GET_INTERNAL_TEMP);
    TI = read_uart(uart_filesystem, GET_INTERNAL_TEMP).float_value;

    double value_to_send = pid_control(TI);
    pwm_control(value_to_send);

    write_uart_get(uart_filesystem, GET_POTENTIOMETER);
    TR = read_uart(uart_filesystem, GET_POTENTIOMETER).float_value;

    pid_reference(TR);
    TE = get_current_temperature(&bme_connection);

    printf("\tTI: %.2f⁰C - TR: %.2f⁰C - TE: %.2f⁰C\n", TI, TR, TE);
    print_display("UART ", TI, TR, TE);

  if (!use_terminal) {
      write_uart_get(uart_filesystem, GET_KEY_VALUE);
      key_gpio = read_uart(uart_filesystem, GET_KEY_VALUE).int_value;
    }

    write_uart_send(uart_filesystem, value_to_send);
  } while (key_gpio == key);
}


void terminal_routine(float TR, int key) {
  system("clear");
  float TI, TE;
  int value_to_send = 0;
  printf("\n================== Iniciada rotina Terminal ==================\n");
  pid_setup(20, 0.1, 100);
  do {
    write_uart_get(uart_filesystem, GET_INTERNAL_TEMP);
    TI = read_uart(uart_filesystem, GET_INTERNAL_TEMP).float_value;

    double value_to_send = pid_controle(TI);
    pwm_control(value_to_send);

    pid_reference(TR);

    TE = get_current_temperature(&bme_connection);
    printf("\tTERMINAL TI: %.2f⁰C - TR: %.2f⁰C - TE: %.2f⁰C\n", TI, TR, TE);
    print_display("Terminal ", TI, TR, TE);

    if (!use_terminal) {
      write_uart_get(uart_filesystem, GET_KEY_VALUE);
      key_gpio = read_uart(uart_filesystem, GET_KEY_VALUE).int_value;
    }

    write_uart_send_REF(uart_filesystem, TR);
  } while (key_gpio == key);
}


void init() {
  wiringPiSetup();
  turn_resistance_off();
  turn_fan_off();
  connect_display();
  bme_connection = connect_bme();
  uart_filesystem = connect_uart();
  system("clear");
}

void menu() {
  int option;
  float tr;
  printf("Deseja controlar o programa por:\n\t1) Terminal\n\t2) Potenciometro\n");
  scanf("%d", &option);
  switch (option) {
    case 1:
      system("clear");
      printf("Insira o valor da Temperatura Referêncial(TR): \n");
      scanf("%f", &tr);
      terminal_routine(0, tr);
      break;
    case 2:
      use_terminal = true;
      pid_routine(0);
      break;
    case 3:
      break;
    default:
      system("clear");
      printf("Opção invalida\n");
      menu();
      break;
}

int main() {
  init();
  signal(SIGINT, shutdown_program);
  menu();
  return 0;
}
