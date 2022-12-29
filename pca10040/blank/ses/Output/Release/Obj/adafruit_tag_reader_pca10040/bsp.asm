	.cpu cortex-m4
	.arch armv7e-m
	.fpu fpv4-sp-d16
	.eabi_attribute 27, 1
	.eabi_attribute 28, 1
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 4
	.eabi_attribute 34, 1
	.eabi_attribute 38, 1
	.eabi_attribute 18, 4
	.file	"bsp.c"
	.text
.Ltext0:
	.section	.text.alert_timer_handler,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.type	alert_timer_handler, %function
alert_timer_handler:
.LVL0:
.LFB219:
	.file 1 "C:\\Users\\haris\\Downloads\\DeviceDownload\\nRF5_SDK_17.1.0_ddde560\\nRF5_SDK_17.1.0_ddde560\\components\\libraries\\bsp\\bsp.c"
	.loc 1 456 1 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	.loc 1 457 5 view .LVU1
	.loc 1 458 5 view .LVU2
	movs	r0, #2
.LVL1:
	.loc 1 458 5 is_stmt 0 view .LVU3
	b	bsp_board_led_invert
.LVL2:
.LFE219:
	.size	alert_timer_handler, .-alert_timer_handler
	.section	.text.bsp_button_event_handler,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.type	bsp_button_event_handler, %function
bsp_button_event_handler:
.LVL3:
.LFB214:
	.loc 1 133 1 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 134 5 view .LVU5
	.loc 1 135 5 view .LVU6
	.loc 1 136 5 view .LVU7
	.loc 1 137 5 view .LVU8
	.loc 1 138 5 view .LVU9
	.loc 1 140 5 view .LVU10
	.loc 1 133 1 is_stmt 0 view .LVU11
	push	{r4, r5, r6, r7, r8, r9, r10, lr}
.LCFI0:
	.loc 1 133 1 view .LVU12
	mov	r8, r0
	mov	r5, r1
	.loc 1 140 14 view .LVU13
	bl	bsp_board_pin_to_button_idx
.LVL4:
	.loc 1 142 8 view .LVU14
	cmp	r0, #3
	.loc 1 140 14 view .LVU15
	mov	r4, r0
.LVL5:
	.loc 1 142 5 is_stmt 1 view .LVU16
	.loc 1 142 8 is_stmt 0 view .LVU17
	bhi	.L2
	.loc 1 144 9 is_stmt 1 view .LVU18
	cmp	r5, #1
	beq	.L5
	cmp	r5, #2
	beq	.L6
	cbnz	r5, .L2
	.loc 1 159 17 view .LVU19
	.loc 1 159 23 is_stmt 0 view .LVU20
	ldr	r0, .L19
.LVL6:
	.loc 1 159 23 view .LVU21
	bl	app_timer_stop
.LVL7:
	.loc 1 160 17 is_stmt 1 view .LVU22
	.loc 1 160 75 is_stmt 0 view .LVU23
	ldr	r3, .L19+4
	add	r2, r4, r4, lsl #1
	add	r3, r3, r2
	ldrb	r5, [r3, #2]	@ zero_extendqisi2
	.loc 1 160 42 view .LVU24
	ldr	r3, .L19+8
	.loc 1 160 20 view .LVU25
	ldrb	r3, [r3, r4]	@ zero_extendqisi2
	cmp	r3, r5
	beq	.L11
.LVL8:
.L2:
	.loc 1 174 1 view .LVU26
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.LVL9:
.L5:
	.loc 1 147 17 is_stmt 1 view .LVU27
	.loc 1 147 23 is_stmt 0 view .LVU28
	ldr	r6, .L19+4
	add	r3, r0, r0, lsl #1
	adds	r2, r6, r3
	ldrb	r5, [r6, r3]	@ zero_extendqisi2
.LVL10:
	.loc 1 148 17 is_stmt 1 view .LVU29
	.loc 1 148 20 is_stmt 0 view .LVU30
	ldrb	r3, [r2, #1]	@ zero_extendqisi2
	.loc 1 147 23 view .LVU31
	lsls	r7, r0, #1
	.loc 1 148 20 view .LVU32
	cbz	r3, .L9
	.loc 1 150 21 is_stmt 1 view .LVU33
	.loc 1 150 32 is_stmt 0 view .LVU34
	ldr	r9, .L19+16
	ldr	r0, .L19
.LVL11:
	.loc 1 150 32 view .LVU35
	mov	r2, r9
	mov	r1, #16384
	bl	app_timer_start
.LVL12:
	.loc 1 151 21 is_stmt 1 view .LVU36
	.loc 1 151 24 is_stmt 0 view .LVU37
	cbnz	r0, .L9
	.loc 1 153 25 is_stmt 1 view .LVU38
	.loc 1 153 50 is_stmt 0 view .LVU39
	strb	r8, [r9]
.LVL13:
.L9:
	.loc 1 156 17 is_stmt 1 view .LVU40
	.loc 1 156 70 is_stmt 0 view .LVU41
	add	r7, r7, r4
	add	r6, r6, r7
	.loc 1 156 47 view .LVU42
	ldr	r3, .L19+8
	ldrb	r2, [r6, #2]	@ zero_extendqisi2
	strb	r2, [r3, r4]
	.loc 1 157 17 is_stmt 1 view .LVU43
.LVL14:
.L11:
	.loc 1 170 5 view .LVU44
	.loc 1 170 8 is_stmt 0 view .LVU45
	cmp	r5, #0
	beq	.L2
	.loc 1 170 64 discriminator 1 view .LVU46
	ldr	r3, .L19+12
	ldr	r3, [r3]
	.loc 1 170 38 discriminator 1 view .LVU47
	cmp	r3, #0
	beq	.L2
	.loc 1 172 9 is_stmt 1 view .LVU48
	mov	r0, r5
	.loc 1 174 1 is_stmt 0 view .LVU49
	pop	{r4, r5, r6, r7, r8, r9, r10, lr}
.LCFI1:
.LVL15:
	.loc 1 172 9 view .LVU50
	bx	r3	@ indirect register sibling call
.LVL16:
.L6:
.LCFI2:
	.loc 1 166 17 is_stmt 1 view .LVU51
	.loc 1 166 23 is_stmt 0 view .LVU52
	ldr	r3, .L19+4
	add	r4, r0, r0, lsl #1
	add	r3, r3, r4
	ldrb	r5, [r3, #1]	@ zero_extendqisi2
.LVL17:
	.loc 1 166 23 view