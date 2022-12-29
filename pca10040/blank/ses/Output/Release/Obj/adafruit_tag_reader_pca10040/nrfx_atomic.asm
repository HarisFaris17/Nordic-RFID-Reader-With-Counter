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
	.file	"nrfx_atomic.c"
	.text
.Ltext0:
	.section	.text.nrfx_atomic_u32_fetch_store,"ax",%progbits
	.align	1
	.global	nrfx_atomic_u32_fetch_store
	.syntax unified
	.thumb
	.thumb_func
	.type	nrfx_atomic_u32_fetch_store, %function
nrfx_atomic_u32_fetch_store:
.LVL0:
.LFB177:
	.file 1 "C:\\Users\\haris\\Downloads\\DeviceDownload\\nRF5_SDK_17.1.0_ddde560\\nRF5_SDK_17.1.0_ddde560\\modules\\nrfx\\soc\\nrfx_atomic.c"
	.loc 1 55 1 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 59 5 view .LVU1
	.loc 1 60 5 view .LVU2
.LBB2:
	.loc 1 61 5 view .LVU3
	.loc 1 61 5 view .LVU4
.LBE2:
	.loc 1 55 1 is_stmt 0 view .LVU5
	push	{r4, lr}
.LCFI0:
	.loc 1 55 1 view .LVU6
	mov	r3, r0
.LBB3:
	.loc 1 61 5 view .LVU7
	.syntax unified
@ 61 "C:\Users\haris\Downloads\DeviceDownload\nRF5_SDK_17.1.0_ddde560\nRF5_SDK_17.1.0_ddde560\modules\nrfx\soc\nrfx_atomic.c" 1
	1:     ldrex   r0, [r3]
mov r2, r1
       strex   r4, r2, [r3]
       teq     r4, #0
       bne.n     1b
@ 0 "" 2
.LVL1:
	.loc 1 61 5 is_stmt 1 view .LVU8
	.thumb
	.syntax unified
.LBE3:
	.loc 1 61 57 view .LVU9
	.loc 1 62 5 view .LVU10
	.loc 1 63 5 view .LVU11
	.loc 1 71 1 is_stmt 0 view .LVU12
	pop	{r4, pc}
.LFE177:
	.size	nrfx_atomic_u32_fetch_store, .-nrfx_atomic_u32_fetch_store
	.section	.text.nrfx_atomic_u32_store,"ax",%progbits
	.align	1
	.global	nrfx_atomic_u32_store
	.syntax unified
	.thumb
	.thumb_func
	.type	nrfx_atomic_u32_store, %function
nrfx_atomic_u32_store:
.LVL2:
.LFB178:
	.loc 1 74 1 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 79 5 view .LVU14
	.loc 1 80 5 view .LVU15
.LBB4:
	.loc 1 81 5 view .LVU16
	.loc 1 81 5 view .LVU17
.LBE4:
	.loc 1 74 1 is_stmt 0 view .LVU18
	push	{r4, lr}
.LCFI1:
	.loc 1 74 1 view .LVU19
	mov	r3, r0
.LBB5:
	.loc 1 81 5 view .LVU20
	.syntax unified
@ 81 "C:\Users\haris\Downloads\DeviceDownload\nRF5_SDK_17.1.0_ddde560\nRF5_SDK_17.1.0_ddde560\modules\nrfx\soc\nrfx_atomic.c" 1
	1:     ldrex   r2, [r3]
mov r0, r1
       strex   r4, r0, [r3]
       teq     r4, #0
       bne.n     1b
@ 0 "" 2
.LVL3:
	.loc 1 81 5 is_stmt 1 view .LVU21
	.thumb
	.syntax unified
.LBE5:
	.loc 1 81 57 view .LVU22
	.loc 1 82 5 view .LVU23
	.loc 1 83 5 view .LVU24
	.loc 1 90 1 is_stmt 0 view .LVU25
	pop	{r4, pc}
.LFE178:
	.size	nrfx_atomic_u32_store, .-nrfx_atomic_u32_store
	.section	.text.nrfx_atomic_u32_fetch_or,"ax",%progbits
	.align	1
	.global	nrfx_atomic_u32_fetch_or
	.syntax unified
	.thumb
	.thumb_func
	.type	nrfx_atomic_u32_fetch_or, %function
nrfx_atomic_u32_fetch_or:
.LVL4:
.LFB179:
	.loc 1 93 1 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 97 5 view .LVU27
	.loc 1 98 5 view .LVU28
.LBB6:
	.loc 1 99 5 view .LVU29
	.loc 1 99 5 view .LVU30
.LBE6:
	.loc 1 93 1 is_stmt 0 view .LVU31
	push	{r4, lr}
.LCFI2:
	.loc 1 93 1 view .LVU32
	mov	r3, r0
.LBB7:
	.loc 1 99 5 view .LVU33
	.syntax unified
@ 99 "C:\Users\haris\Downloads\DeviceDownload\nRF5_SDK_17.1.0_ddde560\nRF5_SDK_17.1.0_ddde560\modules\nrfx\soc\nrfx_atomic.c" 1
	1:     ldrex   r0, [r3]
orr r2, r0, r1
       strex   r4, r2, [r3]
       teq     r4, #0
       bne.n     1b
@ 0 "" 2
.LVL5:
	.loc 1 99 5 is_stmt 1 view .LVU34
	.thumb
	.syntax unified
.LBE7:
	.loc 1 99 57 view .LVU35
	.loc 1 100 5 view .LVU36
	.loc 1 101 5 view .LVU37
	.loc 1 109 1 is_stmt 0 view .LVU38
	pop	{r4, pc}
.LFE179:
	.size	nrfx_atomic_u32_fetch_or, .-nrfx_atomic_u32_fetch_or
	.section	.text.nrfx_atomic_u32_or,"ax",%progbits
	.align	1
	.global	nrfx_atomic_u32_or
	.syntax unified
	.thumb
	.thumb_func
	.type	nrfx_atomic_u32_or, %function
nrfx_atomic_u32_or:
.LVL6:
.LFB180:
	.loc 1 112 1 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 116 5 view .LVU40
	.loc 1 117 5 view .LVU41
.LBB8:
	.loc 1 118 5 view .LVU42
	.loc 1 118 5 view .LVU43
.LBE8:
	.loc 1 112 1 is_stmt 0 view .LVU44
	push	{r4, lr}
.LCFI3:
	.loc 1 112 1 view .LVU45
	mov	r3, r0
.LBB9:
	.loc 1 118 5 view .LVU46
	.syntax unified
@ 118 "C:\Users\haris\Downloads\DeviceDownload\nRF5_SDK_17.1.0_ddde560\nRF5_SDK_17.1.0_ddde560\modules\nrfx\soc\nrfx_atomic.c" 1
	1:     ldrex   r2, [r3]
orr r0, r2, r1
       strex   r4, r0, [r3]
       teq     r4, #0
       bne.n     1b
@ 0 "" 2
.LVL7:
	.loc 1 118 5 is_stmt 1 view .LVU47
	.thumb
	.syntax unified
.LBE9:
	.loc 1 118 57 view .LVU48
	.loc 1 119 5 view .LVU49
	.loc 1 120 5 view .LVU50
	.loc 1 128 1 is_stmt 0 view .LVU51
	pop	{r4, pc}
.LFE180:
	.size	nrfx_atomic_u32_or, .-nrfx_atomic_u32_or
	.section	.text.nrfx_atomic_u32_fetch_and,"ax",%progbits
	.align	1
	.global	nrfx_atomic_u32_fetch_and
	.syntax unified
	.thumb
	.thumb_func
	.type	nrfx_atomic_u32_fetch_and, %function
nrfx_atomic_u32_fetch_and:
.LVL8:
.LFB181:
	.loc 1 131 1 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 135 5 view .LVU53
	.loc 1 136 5 view .LVU54
.LBB10:
	.loc 1 137 5 view .LVU55
	.loc 1 137 5 view .LVU56
.LBE10:
	.loc 1 131 1 is_stmt 0 view .LVU57
	push	{r4, lr}
.LCFI4:
	.loc 1 131 1 view .LVU58
	mov	r3, r0
.LBB11:
	.loc 1 137 5 view .LVU59
	.syntax unified
@ 137 "C:\Users\haris\Downloads\DeviceDownload\nRF5_SDK_17.1.0_ddde560\nRF5_SDK_17.1.0_ddde560\modules\nrfx\soc\nrfx_atomic.c" 1
	1:     ldrex   r0, [r3]
and r2, r0, r1
       strex   r4, r2, [r3]
       teq     r4, #0
       bne.n     1b
@ 0 "" 2
.LVL9:
	.loc 1 137 5 is_stmt 1 view .LVU60
	.thumb
	.syntax unified
.LBE11:
	.loc 1 137 57 view .LVU61
	.loc 1 138 5 view .LVU62
	.loc 1 139 5 view .LVU63
	.loc 1 147 1 is_stmt 0 view .LVU64
	pop	{r4, pc}
.LFE181:
	.size	nrfx_atomic_u32_fetch_and, .-nrfx_atomic_u32_fetch_and
	.section	.text.nrfx_atomic_u32_and,"ax",%progbits
	.align	1
	.global	nrfx_atomic_u32_and
	.syntax unified
	.thumb
	.thumb_func
	.type	nrfx_atomic_u32_and, %function
nrfx_atomic_u32_and:
.LVL10:
.LFB182:
	.loc 1 150 1 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 154 5 view .LVU66
	.loc 1 155 5 view .LVU67
.LBB12:
	.loc 1 156 5 view .LVU68
	.loc 1 156 5 view .LVU69
.LBE12:
	.loc 1 150 1 is_stmt 0 view .LVU70
	push	{r4, lr}
.LCFI5:
	.loc 1 150 1 view .LVU71
	mov	r3, r0
.LBB13:
	.loc 1 156 5 view .LVU72
	.syntax unified
@ 156 "C:\Users\haris\Downloads\DeviceDownload\nRF5_SDK_17.1.0_ddde560\nRF5_SDK_17.1.0_ddde560\modules\nrfx\soc\nrfx_atomic.c" 1
	1:     ldrex   r2, [r3]
and r0, r2, r1
       strex   r4, r0, [r3]
       teq     r4, #0
       bne.n     1b
@ 0 "" 2
.LVL11:
	.loc 1 156 5 is_stmt 1 view .LVU73
	.thumb
	.syntax unified
.LBE13:
	.loc 1 156 57 view .LVU74
	.loc 1 157 5 view .LVU75
	.loc 1 158 5 view .LVU76
	.loc 1 166 1 is_stmt 0 view .LVU77
	pop	{r4, pc}
.LFE182:
	.size	nrfx_atomic_u32_and, .-nrfx_atomic_u32_and
	.section	.text.nrfx_atomic_u32_fetch_xor,"ax",%progbits
	.align	1
	.global	nrfx_atomic_u32_fetch_xor
	.syntax unified
	.thumb
	.thumb_func
	.type	nrfx_atomic_u32_fetch_xor, %function
nrfx_atomic_u32_fetch_xor:
.LVL12:
.LFB183:
	.loc 1 169 1 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 173 5 view .LVU79
	.loc 1 174 5 view .LVU80
.LBB14:
	.loc 1 175 5 view .LVU81
	.loc 1 175 5 view .LVU82
.LBE14:
	.loc 1 169 1 is_stmt 0 view .LVU83
	push	{r4, lr}
.LCFI6:
	.loc 1 169 1 view .LVU84
	mov	r3, r0
.LBB15:
	.loc 1 175 5 view .LVU85
	.syntax unified
@ 175 "C:\Users\haris\Downloads\DeviceDownload\nRF5_SDK_17.1.0_ddde560\nRF5_SDK_17.1.0_ddde560\modules\nrfx\soc\nrfx_atomic.c" 1
	1:     ldrex   r0, [r3]
eor r2, r0, r1
       strex   r4, r2, [r3]
       teq     r4, #0
       bne.n     1b
@ 0 "" 2
.LVL13:
	.loc 1 175 5 is_stmt 1 view .LVU86
	.thumb
	.syntax unified
.LBE15:
	.loc 1 175 57 view .LVU87
	.loc 1 176 5 view .LVU88
	.loc 1 177 5 view .LVU89
	.loc 1 185 1 is_stmt 0 view .LVU90
	pop	{r4, pc}
.LFE183:
	.size	nrfx_atomic_u32_fetch_xor, .-nrfx_atomic_u32_fetch_xor
	.section	.text.nrfx_atomic_u32_xor,"ax",%progbits
	.align	1
	.global	nrfx_atomic_u32_xor
	.syntax unified
	.thumb
	.thumb_func
	.type	nrfx_atomic_u32_xor, %function
nrfx_atomic_u32_xor:
.LVL14:
.LFB184:
	.loc 1 188 1 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 192 5 view .LVU92
	.loc 1 193 5 view .LVU93
.LBB16:
	.loc 1 194 5 view .LVU94
	.loc 1 194 5 view .LVU95
.LBE16:
	.loc 1 188 1 is_stmt 0 view .LVU96
	push	{r4, lr}
.LCFI7:
	.loc 1 188 1 view .LVU97
	mov	r3, r0
.LBB17:
	.loc 1 194 5 view .LVU98
	.syntax unified
@ 194 "C:\Users\haris\Downloads\DeviceDownload\nRF5_SDK_17.1.0_ddde560\nRF5_SDK_17.1.0_ddde560\modules\nrfx\soc\nrfx_atomic.c" 1
	1:     ldrex   r2, [r3]
eor r0, r2, r1
       strex   r4, r0, [r3]
       teq     r4, #0
       bne.n     1b
@ 0 "" 2
.LVL15:
	.loc 1 194 5 is_stmt 1 view .LVU99
	.thumb
	.syntax unified
.LBE17:
	.loc 1 194 57 view .LVU100
	.loc 1 195 5 view .LVU101
	.loc 1 196 5 view .LVU102
	.loc 1 204 1 is_stmt 0 view .LVU103
	pop	{r4, pc}
.LFE184:
	.size	nrfx_atomic_u32_xor, .-nrfx_atomic_u32_xor
	.section	.text.nrfx_atomic_u32_fetch_add,"ax",%progbits
	.align	1
	.global	nrfx_atomic_u32_fetch_add
	.syntax unified
	.thumb
	.thumb_func
	.type	nrfx_atomic_u32_fetch_add, %function
nrfx_atomic_u32_fetch_add:
.LVL16:
.LFB185:
	.loc 1 207 1 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 211 5 view .LVU105
	.loc 1 212 5 view .LVU106
.LBB18:
	.loc 1 213 5 view .LVU107
	.loc 1 213 5 view .LVU108
.LBE18:
	.loc 1 207 1 is_stmt 0 view .LVU109
	push	{r4, lr}
.LCFI8:
	.loc 1 207 1 view .LVU110
	mov	r3, r0
.LBB19:
	.loc 1 213 5 view .LVU111
	.syntax unified
@ 213 "C:\Users\haris\Downloads\DeviceDownload\nRF5_SDK_17.1.0_ddde560\nRF5_SDK_17.1.0_ddde560\modules\nrfx\soc\nrfx_atomic.c" 1
	1:     ldrex   r0, [r3]
add r2, r0, r1
       strex   r4, r2, [r3]
       teq     r4, #0
       bne.n     1b
@ 0 "" 2
.LVL17:
	.loc 1 213 5 is_stmt 1 view .LVU112
	.thumb
	.syntax unified
.LBE19:
	.loc 1 213 57 view .LVU113
	.loc 1 214 5 view .LVU114
	.loc 1 215 5 view .LVU115
	.loc 1 223 1 is_stmt 0 view .LVU116
	pop	{r4, pc}
.LFE185:
	.size	nrfx_atomic_u32_fetch_add, .-nrfx_atomic_u32_fetch_add
	.section	.text.nrfx_atomic_u32_add,"ax",%progbits
	.align	1
	.global	nrfx_atomic_u32_add
	.syntax unified
	.thumb
	.thumb_func
	.type	nrfx_atomic_u32_add, %function
nrfx_atomic_u32_add:
.LVL18:
.LFB186:
	.loc 1 226 1 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 230 5 view .LVU118
	.loc 1 231 5 view .LVU119
.LBB20:
	.loc 1 232 5 view .LVU120
	.loc 1 232 5 view .LVU121
.LBE20:
	.loc 1 226 1 is_stmt 0 view .LVU122
	push	{r4, lr}
.LCFI9:
	.loc 1 226 1 view .LVU123
	mov	r3, r0
.LBB21:
	.loc 1 232 5 view .LVU124
	.syntax unified
@ 232 "C:\Users\haris\Downloads\DeviceDownload\nRF5_SDK_17.1.0_ddde560\nRF5_SDK_17.1.0_ddde560\modules\nrfx\soc\nrfx_atomic.c" 1
	1:     ldrex   r2, [r3]
add r0, r2, r1
       strex   r4, r0, [r3]
       teq     r4, #0
       bne.n     1b
@ 0 "" 2
.LVL19:
	.loc 1 232 5 is_stmt 1 view .LVU125
	.thumb
	.syntax unified
.LBE21:
	.loc 1 232 57 view .LVU126
	.loc 1 233 5 view .LVU127
	.loc 1 234 5 view .LVU128
	.loc 1 242 1 is_stmt 0 view .LVU129
	pop	{r4, pc}
.LFE186:
	.size	nrfx_atomic_u32_add, .-nrfx_atomic_u32_add
	.section	.text.nrfx_atomic_u32_fetch_sub,"ax",%progbits
	.align	1
	.global	nrfx_atomic_u32_fetch_sub
	.syntax unified
	.thumb
	.thumb_func
	.type	nrfx_atomic_u32_fetch_sub, %function
nrfx_atomic_u32_fetch_sub:
.LVL20:
.LFB187:
	.loc 1 245 1 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 249 5 view .LVU131
	.loc 1 250 5 view .LVU132
.LBB22:
	.loc 1 251 5 view .LVU133
	.loc 1 251 5 view .LVU134
.LBE22:
	.loc 1 245 1 is_stmt 0 view .LVU135
	push	{r4, lr}
.LCFI10:
	.loc 1 245 1 view .LVU136
	mov	r3, r0
.LBB23:
	.loc 1 251 5 view .LVU137
	.syntax unified
@ 251 "C:\Users\haris\Downloads\DeviceDownload\nRF5_SDK_17.1.0_ddde560\nRF5_SDK_17.1.0_ddde560\modules\nrfx\soc\nrfx_atomic.c" 1
	1:     ldrex   r0, [r3]
sub r2, r0, r1
       strex   r4, r2, [r3]
       teq     r4, #0
       bne.n     1b
@ 0 "" 2
.LVL21:
	.loc 1 251 5 is_stmt 1 view .LVU138
	.thumb
	.syntax unified
.LBE23:
	.loc 1 251 57 view .LVU139
	.loc 1 252 5 view .LVU140
	.loc 1 253 5 view .LVU141
	.loc 1 261 1 is_stmt 0 view .LVU142
	pop	{r4, pc}
.LFE187:
	.size	nrfx_atomic_u32_fetch_sub, .-nrfx_atomic_u32_fetch_sub
	.section	.text.nrfx_atomic_u32_sub,"ax",%progbits
	.align	1
	.global	nrfx_atomic_u32_sub
	.syntax unified
	.thumb
	.thumb_func
	.type	nrfx_atomic_u32_sub, %function
nrfx_atomic_u32_sub:
.LVL22:
.LFB188:
	.loc 1 264 1 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 268 5 view .LVU144
	.loc 1 269 5 view .LVU145
.LBB24:
	.loc 1 270 5 view .LVU146
	.loc 1 270 5 view .LVU147
.LBE24:
	.loc 1 264 1 is_stmt 0 view .LVU148
	push	{r4, lr}
.LCFI11:
	.loc 1 264 1 view .LVU149
	mov	r3, r0
.LBB25:
	.loc 1 270 5 view .LVU150
	.syntax unified
@ 270 "C:\Users\haris\Downloads\DeviceDownload\nRF5_SDK_17.1.0_ddde560\nRF5_SDK_17.1.0_ddde560\modules\nrfx\soc\nrfx_atomic.c" 1
	1:     ldrex   r2, [r3]
sub r0, r2, r1
       strex   r4, r0, [r3]
       teq     r4, #0
       bne.n     1b
@ 0 "" 2
.LVL23:
	.loc 1 270 5 is_stmt 1 view .LVU151
	.thumb
	.syntax unified
.LBE25:
	.loc 1 270 57 view .LVU152
	.loc 1 271 5 view .LVU153
	.loc 1 272 5 view .LVU154
	.loc 1 280 1 is_stmt 0 view .LVU155
	pop	{r4, pc}
.LFE188:
	.size	nrfx_atomic_u32_sub, .-nrfx_atomic_u32_sub
	.section	.text.nrfx_atomic_u32_cmp_exch,"ax",%progbits
	.align	1
	.global	nrfx_atomic_u32_cmp_exch
	.syntax unified
	.thumb
	.thumb_func
	.type	nrfx_atomic_u32_cmp_exch, %function
nrfx_atomic_u32_cmp_exch:
.LVL24:
.LFB189:
	.loc 1 285 1 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 294 5 view .LVU157
.LBB28:
.LBI28:
	.file 2 "C:\\Users\\haris\\Downloads\\DeviceDownload\\nRF5_SDK_17.1.0_ddde560\\nRF5_SDK_17.1.0_ddde560\\modules\\nrfx\\soc\\nrfx_atomic_internal.h"
	.loc 2 286 20 view .LVU158
.LBB29:
	.loc 2 290 5 view .LVU159
	.loc 2 294 5 view .LVU160
	.loc 2 295 5 view .LVU161
	.loc 2 296 5 view .LVU162
	.loc 2 297 5 view .LVU163
	movs	r3, #0
.LBE29:
.LBE28:
	.loc 1 285 1 is_stmt 0 view .LVU164
	push	{r4, r5, r6, lr}
.LCFI12:
.LBB31:
.LBB30:
	.loc 2 297 5 view .LVU165
	mov	r4, r3
	mov	r5, r3
	.syntax unified
@ 297 "C:\Users\haris\Downloads\DeviceDownload\nRF5_SDK_17.1.0_ddde560\nRF5_SDK_17.1.0_ddde560\modules\nrfx\soc\nrfx_atomic_internal.h" 1
	1:     ldrex   r3, [r0]
       ldr     r5, [r1]
       cmp     r3, r5
       ittee   eq
       strexeq r6, r2, [r0]
       moveq   r4, #1
       strexne r6, r3, [r0]
       strne   r3, [r1]
       cmp     r6, #0
       itt     ne
       movne   r4, #0
       bne.n   1b
@ 0 "" 2
.LVL25:
	.loc 2 323 5 is_stmt 1 view .LVU166
	.loc 2 324 5 view .LVU167
	.loc 2 324 5 is_stmt 0 view .LVU168
	.thumb
	.syntax unified
.LBE30:
.LBE31:
	.loc 1 311 1 view .LVU169
	uxtb	r0, r4
.LVL26:
	.loc 1 311 1 view .LVU170
	pop	{r4, r5, r6, pc}
.LFE189:
	.size	nrfx_atomic_u32_cmp_exch, .-nrfx_atomic_u32_cmp_exch
	.section	.text.nrfx_atomic_u32_fetch_sub_hs,"ax",%progbits
	.align	1
	.global	nrfx_atomic_u32_fetch_sub_hs
	.syntax unified
	.thumb
	.thumb_func
	.type	nrfx_atomic_u32_fetch_sub_hs, %function
nrfx_atomic_u32_fetch_sub_hs:
.LVL27:
.LFB190:
	.loc 1 314 1 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 331 5 view .LVU172
	.loc 1 332 5 view .LVU173
.LBB32:
	.loc 1 333 5 view .LVU174
	.loc 1 333 5 view .LVU175
.LBE32:
	.loc 1 314 1 is_stmt 0 view .LVU176
	push	{r4, lr}
.LCFI13:
	.loc 1 314 1 view .LVU177
	mov	r3, r0
.LBB33:
	.loc 1 333 5 view .LVU178
	.syntax unified
@ 333 "C:\Users\haris\Downloads\DeviceDownload\nRF5_SDK_17.1.0_ddde560\nRF5_SDK_17.1.0_ddde560\modules\nrfx\soc\nrfx_atomic.c" 1
	1:     ldrex   r0, [r3]
cmp r0, r1
 ite hs
subhs r2, r0, r1
movlo r2, r0
       strex   r4, r2, [r3]
       teq     r4, #0
       bne.n     1b
@ 0 "" 2
.LVL28:
	.loc 1 333 5 is_stmt 1 view .LVU179
	.thumb
	.syntax unified
.LBE33:
	.loc 1 333 60 view .LVU180
	.loc 1 334 5 view .LVU181
	.loc 1 335 5 view .LVU182
	.loc 1 343 1 is_stmt 0 view .LVU183
	pop	{r4, pc}
.LFE190:
	.size	nrfx_atomic_u32_fetch_sub_hs, .-nrfx_atomic_u32_fetch_sub_hs
	.section	.text.nrfx_atomic_u32_sub_hs,"ax",%progbits
	.align	1
	.global	nrfx_atomic_u32_sub_hs
	.syntax unified
	.thumb
	.thumb_func
	.type	nrfx_atomic_u32_sub_hs, %function
nrfx_atomic_u32_sub_hs:
.LVL29:
.LFB191:
	.loc 1 346 1 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 363 5 view .LVU185
	.loc 1 364 5 view .LVU186
.LBB34:
	.loc 1 365 5 view .LVU187
	.loc 1 365 5 view .LVU188
.LBE34:
	.loc 1 346 1 is_stmt 0 view .LVU189
	push	{r4, lr}
.LCFI14:
	.loc 1 346 1 view .LVU190
	mov	r3, r0
.LBB35:
	.loc 1 365 5 view .LVU191
	.syntax unified
@ 365 "C:\Users\haris\Downloads\DeviceDownload\nRF5_SDK_17.1.0_ddde560\nRF5_SDK_17.1.0_ddde560\modules\nrfx\soc\nrfx_atomic.c" 1
	1:     ldrex   r2, [r3]
cmp r2, r1
 ite hs
subhs r0, r2, r1
movlo r0, r2
       strex   r4, r0, [r3]
       teq     r4, #0
       bne.n     1b
@ 0 "" 2
.LVL30:
	.loc 1 365 5 is_stmt 1 view .LVU192
	.thumb
	.syntax unified
.LBE35:
	.loc 1 365 60 view .LVU193
	.loc 1 366 5 view .LVU194
	.loc 1 367 5 view .LVU195
	.loc 1 375 1 is_stmt 0 view .LVU196
	pop	{r4, pc}
.LFE191:
	.size	nrfx_atomic_u32_sub_hs, .-nrfx_atomic_u32_sub_hs
	.section	.text.nrfx_atomic_flag_set_fetch,"ax",%progbits
	.align	1
	.global	nrfx_atomic_flag_set_fetch
	.syntax unified
	.thumb
	.thumb_func
	.type	nrfx_atomic_flag_set_fetch, %function
nrfx_atomic_flag_set_fetch:
.LVL31:
.LFB192:
	.loc 1 378 1 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	.loc 1 379 5 view .LVU198
	.loc 1 379 12 is_stmt 0 view .LVU199
	movs	r1, #1
	b	nrfx_atomic_u32_fetch_or
.LVL32:
	.loc 1 379 12 view .LVU200
.LFE192:
	.size	nrfx_atomic_flag_set_fetch, .-nrfx_atomic_flag_set_fetch
	.section	.text.nrfx_atomic_flag_set,"ax",%progbits
	.align	1
	.global	nrfx_atomic_flag_set
	.syntax unified
	.thumb
	.thumb_func
	.type	nrfx_atomic_flag_set, %function
nrfx_atomic_flag_set:
.LVL33:
.LFB193:
	.loc 1 383 1 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	.loc 1 384 5 view .LVU202
	.loc 1 384 12 is_stmt 0 view .LVU203
	movs	r1, #1
	b	nrfx_atomic_u32_or
.LVL34:
	.loc 1 384 12 view .LVU204
.LFE193:
	.size	nrfx_atomic_flag_set, .-nrfx_atomic_flag_set
	.section	.text.nrfx_atomic_flag_clear_fetch,"ax",%progbits
	.align	1
	.global	nrfx_atomic_flag_clear_fetch
	.syntax unified
	.thumb
	.thumb_func
	.type	nrfx_atomic_flag_clear_fetch, %function
nrfx_atomic_flag_clear_fetch:
.LVL35:
.LFB194:
	.loc 1 388 1 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	.loc 1 389 5 view .LVU206
	.loc 1 389 12 is_stmt 0 view .LVU207
	movs	r1, #0
	b	nrfx_atomic_u32_fetch_and
.LVL36:
	.loc 1 389 12 view .LVU208
.LFE194:
	.size	nrfx_atomic_flag_clear_fetch, .-nrfx_atomic_flag_clear_fetch
	.section	.text.nrfx_atomic_flag_clear,"ax",%progbits
	.align	1
	.global	nrfx_atomic_flag_clear
	.syntax unified
	.thumb
	.thumb_func
	.type	nrfx_atomic_flag_clear, %function
nrfx_atomic_flag_clear:
.LVL37:
.LFB195:
	.loc 1 393 1 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	.loc 1 394 5 view .LVU210
	.loc 1 394 12 is_stmt 0 view .LVU211
	movs	r1, #0
	b	nrfx_atomic_u32_and
.LVL38:
	.loc 1 394 12 view .LVU212
.LFE195:
	.size	nrfx_atomic_flag_clear, .-nrfx_atomic_flag_clear
	.section	.debug_frame,"",%progbits
.Lframe0:
	.4byte	.LECIE0-.LSCIE0
.LSCIE0:
	.4byte	0xffffffff
	.byte	0x3
	.ascii	"\000"
	.uleb128 0x1
	.sleb128 -4
	.uleb128 0xe
	.byte	0xc
	.uleb128 0xd
	.uleb128 0
	.align	2
.LECIE0:
.LSFDE0:
	.4byte	.LEFDE0-.LASFDE0
.LASFDE0:
	.4byte	.Lframe0
	.4byte	.LFB177
	.4byte	.LFE177