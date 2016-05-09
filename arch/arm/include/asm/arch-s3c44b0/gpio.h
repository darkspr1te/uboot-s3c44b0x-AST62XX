#ifndef GPIO_H
#define GPIO_H
hjkhkjhkjhjkhhkj
 unsigned led_id_t;
 void __led_init (led_id_t mask, int state);
 void __led_set (led_id_t mask, int state);
void __led_toggle (led_id_t mask);

#endif	/* __ASM_STATUS_LED_H__ */
