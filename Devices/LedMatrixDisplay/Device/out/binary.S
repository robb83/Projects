	.file	"main.c"
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
	.text
.Ltext0:
	.cfi_sections	.debug_frame
.global	__vector_11
	.type	__vector_11, @function
__vector_11:
.LFB0:
	.file 1 "src\\main.c"
	.loc 1 27 0
	.cfi_startproc
	push r1
.LCFI0:
	.cfi_def_cfa_offset 3
	.cfi_offset 1, -2
	push r0
.LCFI1:
	.cfi_def_cfa_offset 4
	.cfi_offset 0, -3
	in r0,__SREG__
	push r0
	clr __zero_reg__
	push r24
.LCFI2:
	.cfi_def_cfa_offset 5
	.cfi_offset 24, -4
	push r25
.LCFI3:
	.cfi_def_cfa_offset 6
	.cfi_offset 25, -5
/* prologue: Signal */
/* frame size = 0 */
/* stack size = 5 */
.L__stack_usage = 5
	.loc 1 28 0
	in r24,0x16
	andi r24,lo8(24)
.LVL0:
	.loc 1 30 0
	lds r25,pin_interrupts
	eor r25,r24
	sts pin_changed,r25
	.loc 1 31 0
	sts pin_interrupts,r24
/* epilogue start */
	.loc 1 32 0
	pop r25
	pop r24
.LVL1:
	pop r0
	out __SREG__,r0
.LVL2:
	pop r0
	pop r1
	reti
	.cfi_endproc
.LFE0:
	.size	__vector_11, .-__vector_11
.global	__vector_7
	.type	__vector_7, @function
__vector_7:
.LFB1:
	.loc 1 34 0
	.cfi_startproc
	push r1
.LCFI4:
	.cfi_def_cfa_offset 3
	.cfi_offset 1, -2
	push r0
.LCFI5:
	.cfi_def_cfa_offset 4
	.cfi_offset 0, -3
	in r0,__SREG__
	push r0
	clr __zero_reg__
	push r24
.LCFI6:
	.cfi_def_cfa_offset 5
	.cfi_offset 24, -4
	push r30
.LCFI7:
	.cfi_def_cfa_offset 6
	.cfi_offset 30, -5
	push r31
.LCFI8:
	.cfi_def_cfa_offset 7
	.cfi_offset 31, -6
/* prologue: Signal */
/* frame size = 0 */
/* stack size = 6 */
.L__stack_usage = 6
	.loc 1 35 0
	lds r24,state
	cpse r24,__zero_reg__
	rjmp .L3
	.loc 1 36 0
	in r24,0xc
	sts state,r24
	.loc 1 37 0
	sts state_position,__zero_reg__
	rjmp .L2
.L3:
	.loc 1 38 0
	lds r24,state
	cpi r24,lo8(65)
	brne .L5
	.loc 1 39 0
	lds r24,state_position
	cpi r24,lo8(64)
	brsh .L2
	.loc 1 40 0
	lds r30,state_position
	ldi r24,lo8(1)
	add r24,r30
	sts state_position,r24
	ldi r31,0
	in r24,0xc
	subi r30,lo8(-(buffer))
	sbci r31,hi8(-(buffer))
	rjmp .L9
.L5:
	.loc 1 42 0
	lds r24,state
	cpi r24,lo8(66)
	breq .L14
	.loc 1 46 0
	lds r24,state
	cpi r24,lo8(67)
	brne .L7
.L14:
	.loc 1 47 0
	lds r24,state_position
	cpi r24,lo8(2)
	brsh .L2
	rjmp .L10
.L7:
	.loc 1 50 0
	lds r24,state
	cpi r24,lo8(70)
	breq .L12
	.loc 1 54 0
	lds r24,state
	cpi r24,lo8(71)
	brne .L2
.L12:
	.loc 1 55 0
	lds r24,state_position
	cpi r24,lo8(4)
	brsh .L2
.L10:
	.loc 1 56 0
	lds r30,state_position
	ldi r24,lo8(1)
	add r24,r30
	sts state_position,r24
	ldi r31,0
	in r24,0xc
	subi r30,lo8(-(state_buffer))
	sbci r31,hi8(-(state_buffer))
.L9:
	st Z,r24
.L2:
/* epilogue start */
	.loc 1 59 0
	pop r31
	pop r30
	pop r24
	pop r0
	out __SREG__,r0
	pop r0
	pop r1
	reti
	.cfi_endproc
.LFE1:
	.size	__vector_7, .-__vector_7
.global	uart_send
	.type	uart_send, @function
uart_send:
.LFB2:
	.loc 1 61 0
	.cfi_startproc
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
.LVL3:
.L18:
	.loc 1 62 0 discriminator 1
	sbis 0xb,5
	rjmp .L18
	.loc 1 63 0
	out 0xc,r24
	ret
	.cfi_endproc
.LFE2:
	.size	uart_send, .-uart_send
.global	display_clear
	.type	display_clear, @function
display_clear:
.LFB3:
	.loc 1 66 0
	.cfi_startproc
.LVL4:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
.LBB2:
	.loc 1 67 0
	ldi r18,0
	ldi r19,0
.LVL5:
.L20:
	.loc 1 68 0 discriminator 2
	movw r30,r18
	subi r30,lo8(-(buffer))
	sbci r31,hi8(-(buffer))
	st Z,r24
	.loc 1 67 0 discriminator 2
	subi r18,-1
	sbci r19,-1
.LVL6:
	cpi r18,64
	cpc r19,__zero_reg__
	brne .L20
/* epilogue start */
.LBE2:
	.loc 1 70 0
	ret
	.cfi_endproc
.LFE3:
	.size	display_clear, .-display_clear
.global	display_shift_right
	.type	display_shift_right, @function
display_shift_right:
.LFB4:
	.loc 1 72 0
	.cfi_startproc
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
.LVL7:
	.loc 1 72 0
	ldi r24,lo8(4)
	ldi r25,0
.LVL8:
.L26:
	ldi r20,lo8(-4)
	add r20,r24
.LVL9:
.LBB3:
	.loc 1 76 0
	ldi r21,0
.LVL10:
.L23:
	.loc 1 77 0 discriminator 1
	mov r18,r20
	ldi r19,0
	cp r18,r24
	cpc r19,r25
	brge .L27
.LVL11:
.LBB4:
	.loc 1 78 0 discriminator 2
	movw r30,r18
	subi r30,lo8(-(buffer))
	sbci r31,hi8(-(buffer))
	ld r19,Z
.LVL12:
	.loc 1 82 0 discriminator 2
	mov r18,r19
.LVL13:
	lsr r18
.LVL14:
	.loc 1 83 0 discriminator 2
	or r18,r21
.LVL15:
	.loc 1 85 0 discriminator 2
	st Z,r18
.LVL16:
.LBE4:
	.loc 1 77 0 discriminator 2
	subi r20,lo8(-(1))
.LVL17:
.LBB5:
	.loc 1 87 0 discriminator 2
	mov r21,r19
	ror r21
	clr r21
	ror r21
.LVL18:
	rjmp .L23
.LVL19:
.L27:
	adiw r24,4
.LBE5:
.LBE3:
	.loc 1 74 0
	cpi r24,68
	cpc r25,__zero_reg__
	brne .L26
/* epilogue start */
	.loc 1 90 0
	ret
	.cfi_endproc
.LFE4:
	.size	display_shift_right, .-display_shift_right
.global	display_shift_left
	.type	display_shift_left, @function
display_shift_left:
.LFB5:
	.loc 1 92 0
	.cfi_startproc
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
.LVL20:
.LBB6:
.LBB7:
	.loc 1 107 0
	ldi r24,0
	ldi r25,0
.LVL21:
.L29:
	.loc 1 107 0 is_stmt 0 discriminator 1
	ldi r18,0
	ldi r19,0
	ldi r23,0
	movw r20,r24
	subi r20,-3
	sbci r21,-1
.LVL22:
.L32:
	movw r30,r20
	sub r30,r18
	sbc r31,r19
	.loc 1 98 0 is_stmt 1 discriminator 2
	subi r30,lo8(-(buffer))
	sbci r31,hi8(-(buffer))
	ld r22,Z
.LVL23:
	.loc 1 100 0 discriminator 2
	mov r26,r22
	rol r26
	clr r26
	rol r26
.LVL24:
	.loc 1 102 0 discriminator 2
	lsl r22
.LVL25:
	.loc 1 103 0 discriminator 2
	or r22,r23
.LVL26:
	.loc 1 105 0 discriminator 2
	st Z,r22
.LVL27:
	subi r18,-1
	sbci r19,-1
.LVL28:
.LBE7:
	.loc 1 97 0 discriminator 2
	cpi r18,4
	cpc r19,__zero_reg__
	breq .L33
.LBB8:
	.loc 1 107 0
	mov r23,r26
	rjmp .L32
.L33:
	adiw r24,4
.LBE8:
.LBE6:
	.loc 1 94 0
	cpi r24,64
	cpc r25,__zero_reg__
	brne .L29
	ret
	.cfi_endproc
.LFE5:
	.size	display_shift_left, .-display_shift_left
.global	display_shift_up
	.type	display_shift_up, @function
display_shift_up:
.LFB6:
	.loc 1 112 0
	.cfi_startproc
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
.LVL29:
	.loc 1 112 0
	ldi r24,0
	ldi r25,0
.LVL30:
.L36:
	.loc 1 115 0 discriminator 2
	movw r30,r24
	subi r30,lo8(-(buffer+4))
	sbci r31,hi8(-(buffer+4))
	ld r18,Z
	movw r30,r24
	subi r30,lo8(-(buffer))
	sbci r31,hi8(-(buffer))
	st Z,r18
.LVL31:
	adiw r24,1
.LVL32:
	.loc 1 114 0 discriminator 2
	cpi r24,60
	cpc r25,__zero_reg__
	brne .L36
.LVL33:
	.loc 1 118 0 discriminator 1
	sts buffer+60,__zero_reg__
.LVL34:
	sts buffer+61,__zero_reg__
.LVL35:
	sts buffer+62,__zero_reg__
.LVL36:
	sts buffer+63,__zero_reg__
.LVL37:
	ret
	.cfi_endproc
.LFE6:
	.size	display_shift_up, .-display_shift_up
.global	display_shift_down
	.type	display_shift_down, @function
display_shift_down:
.LFB7:
	.loc 1 122 0
	.cfi_startproc
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
.LVL38:
	.loc 1 122 0
	ldi r24,lo8(59)
	ldi r25,0
.LVL39:
.L39:
	.loc 1 125 0 discriminator 2
	movw r30,r24
	subi r30,lo8(-(buffer))
	sbci r31,hi8(-(buffer))
	ld r18,Z
	movw r30,r24
	subi r30,lo8(-(buffer+4))
	sbci r31,hi8(-(buffer+4))
	st Z,r18
.LVL40:
.LVL41:
	sbiw r24,1
	brcc .L39
	.loc 1 128 0 discriminator 1
	sts buffer,__zero_reg__
.LVL42:
	sts buffer+1,__zero_reg__
.LVL43:
	sts buffer+2,__zero_reg__
.LVL44:
	sts buffer+3,__zero_reg__
.LVL45:
	ret
	.cfi_endproc
.LFE7:
	.size	display_shift_down, .-display_shift_down
.global	display_plotting
	.type	display_plotting, @function
display_plotting:
.LFB8:
	.loc 1 132 0
	.cfi_startproc
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	.loc 1 137 0
	sts r.1545,__zero_reg__
	.loc 1 142 0
	ldi r18,lo8(7)
	ldi r19,0
.L41:
	.loc 1 137 0 discriminator 1
	lds r24,r.1545
	cpi r24,lo8(16)
	brlo .+2
	rjmp .L49
	.loc 1 138 0
	sts c.1546,__zero_reg__
.L42:
	.loc 1 138 0 is_stmt 0 discriminator 1
	lds r24,c.1546
	cpi r24,lo8(4)
	brsh .L50
	.loc 1 139 0 is_stmt 1
	sts b.1547,__zero_reg__
.L43:
	.loc 1 139 0 is_stmt 0 discriminator 1
	lds r24,b.1547
	cpi r24,lo8(8)
	brsh .L51
	.loc 1 140 0 is_stmt 1
	cbi 0x18,2
	.loc 1 142 0
	lds r30,r.1545
	ldi r31,0
	lsl r30
	rol r31
	lsl r30
	rol r31
	lds r24,c.1546
	add r30,r24
	adc r31,__zero_reg__
	subi r30,lo8(-(buffer))
	sbci r31,hi8(-(buffer))
	ld r24,Z
	ldi r25,0
	lds r20,b.1547
	movw r22,r18
	sub r22,r20
	sbc r23,__zero_reg__
	mov r0,r22
	rjmp 2f
	1:
	asr r25
	ror r24
	2:
	dec r0
	brpl 1b
	sbrs r24,0
	rjmp .L44
	.loc 1 143 0
	cbi 0x18,0
	rjmp .L45
.L44:
	.loc 1 145 0
	sbi 0x18,0
.L45:
	.loc 1 147 0
	sbi 0x18,2
	.loc 1 139 0
	lds r24,b.1547
	subi r24,lo8(-(1))
	sts b.1547,r24
	rjmp .L43
.L51:
	.loc 1 138 0
	lds r24,c.1546
	subi r24,lo8(-(1))
	sts c.1546,r24
	rjmp .L42
.L50:
	.loc 1 151 0
	sbi 0x12,6
	.loc 1 153 0
	in r25,0x12
	lds r24,r.1545
	andi r24,lo8(15)
	lsl r24
	lsl r24
	andi r25,lo8(-61)
	or r24,r25
	out 0x12,r24
	.loc 1 155 0
	cbi 0x18,1
	.loc 1 156 0
	sbi 0x18,1
	.loc 1 158 0
	cbi 0x12,6
	.loc 1 137 0
	lds r24,r.1545
	subi r24,lo8(-(1))
	sts r.1545,r24
	rjmp .L41
.L49:
/* epilogue start */
	.loc 1 160 0
	ret
	.cfi_endproc
.LFE8:
	.size	display_plotting, .-display_plotting
	.section	.text.startup,"ax",@progbits
.global	main
	.type	main, @function
main:
.LFB9:
	.loc 1 162 0
	.cfi_startproc
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	.loc 1 165 0
	in r24,0x2e
	ori r24,lo8(3)
	out 0x2e,r24
	.loc 1 167 0
	ldi r28,lo8(1)
	out 0x9,r28
	.loc 1 168 0
	out 0x2,__zero_reg__
	.loc 1 169 0
	ldi r24,lo8(-104)
	out 0xa,r24
	.loc 1 171 0
	ldi r24,lo8(126)
	out 0x11,r24
	.loc 1 172 0
	ldi r24,lo8(7)
	out 0x17,r24
	.loc 1 174 0
	in r24,0x18
	ori r24,lo8(24)
	out 0x18,r24
	.loc 1 175 0
	in r24,0x20
	ori r24,lo8(24)
	out 0x20,r24
	.loc 1 176 0
	in r24,0x3b
	ori r24,lo8(32)
	out 0x3b,r24
	.loc 1 179 0
	ldi r24,0
	rcall display_clear
.LVL46:
	.loc 1 180 0
	sts buffer+29,r28
	.loc 1 181 0
	ldi r24,lo8(-128)
	sts buffer+30,r24
	.loc 1 182 0
	sts buffer+33,r28
	.loc 1 183 0
	sts buffer+34,r24
	.loc 1 186 0
/* #APP */
 ;  186 "src\main.c" 1
	sei
 ;  0 "" 2
	.loc 1 203 0
/* #NOAPP */
	ldi r28,lo8(89)
.L53:
	.loc 1 190 0
	lds r24,pin_changed
	tst r24
	breq .L54
	.loc 1 191 0
	ldi r24,lo8(90)
	rcall uart_send
.LVL47:
	.loc 1 192 0
	lds r24,pin_interrupts
	rcall uart_send
.LVL48:
	.loc 1 195 0
	lds r24,pin_changed
	sbrs r24,3
	rjmp .L55
	.loc 1 195 0 is_stmt 0 discriminator 1
	lds r24,pin_interrupts
	sbrs r24,3
	rjmp .L55
	.loc 1 196 0 is_stmt 1
	lds r24,state
	cpi r24,lo8(89)
	brne .L56
	.loc 1 197 0
	ldi r24,0
	rcall display_clear
.LVL49:
	.loc 1 198 0
	sts state,__zero_reg__
	rjmp .L55
.L56:
	.loc 1 200 0
	lds r24,state
	tst r24
	breq .L57
	.loc 1 201 0
	lds r24,state
	rcall uart_send
.LVL50:
.L57:
	.loc 1 203 0
	sts state,r28
	.loc 1 204 0
	ldi r24,lo8(-1)
	rcall display_clear
.LVL51:
.L55:
	.loc 1 208 0
	sts pin_changed,__zero_reg__
.L54:
	.loc 1 212 0
	lds r24,state
	tst r24
	breq .L58
	.loc 1 212 0 is_stmt 0 discriminator 1
	lds r24,state
	cpi r24,lo8(89)
	brne .L59
.L58:
	.loc 1 213 0 is_stmt 1
	rcall display_plotting
.LVL52:
	rjmp .L53
.L59:
	.loc 1 216 0
	lds r24,state
	cpi r24,lo8(65)
	brne .L61
	.loc 1 217 0
	lds r24,state_position
	cpi r24,lo8(64)
	brlo .L53
	.loc 1 218 0
	sts state,__zero_reg__
	.loc 1 219 0
	ldi r24,lo8(65)
	rjmp .L107
.L61:
	.loc 1 223 0
	lds r24,state
	cpi r24,lo8(66)
	breq .+2
	rjmp .L62
	.loc 1 224 0
	lds r24,state_position
	cpi r24,lo8(2)
	brsh .+2
	rjmp .L53
.LBB9:
	.loc 1 225 0
	lds r16,state_buffer
.LVL53:
	.loc 1 226 0
	lds r29,state_buffer+1
.LVL54:
	.loc 1 228 0
	rcall display_shift_right
.LVL55:
	ldi r18,lo8(7)
	ldi r19,0
.LBB10:
.LBB11:
	.loc 1 231 0
	ldi r17,0
.LVL56:
.L65:
	movw r24,r18
	ldi r22,lo8(-4)
	ldi r23,lo8(-1)
	rcall __mulhi3
	.loc 1 230 0
	movw r30,r24
	subi r30,lo8(-(buffer+28))
	sbci r31,hi8(-(buffer+28))
	ld r20,Z
.LVL57:
	.loc 1 231 0
	movw r24,r16
	mov r0,r18
	rjmp 2f
	1:
	asr r25
	ror r24
	2:
	dec r0
	brpl 1b
	sbrc r24,0
	.loc 1 232 0
	ori r20,lo8(-128)
.LVL58:
.L63:
	.loc 1 234 0
	st Z,r20
.LVL59:
.LVL60:
.LBE11:
	subi r18,1
	sbc r19,__zero_reg__
	brcc .L65
	.loc 1 229 0
	ldi r18,lo8(7)
	ldi r19,0
.LVL61:
.LBE10:
.LBB12:
.LBB13:
	.loc 1 238 0
	mov r26,r29
	ldi r27,0
.LVL62:
.L68:
	movw r24,r18
	ldi r22,lo8(-4)
	ldi r23,lo8(-1)
	rcall __mulhi3
	.loc 1 237 0
	movw r30,r24
	subi r30,lo8(-(buffer+60))
	sbci r31,hi8(-(buffer+60))
	ld r20,Z
.LVL63:
	.loc 1 238 0
	movw r24,r26
	mov r0,r18
	rjmp 2f
	1:
	asr r25
	ror r24
	2:
	dec r0
	brpl 1b
	sbrc r24,0
	.loc 1 239 0
	ori r20,lo8(-128)
.LVL64:
.L66:
	.loc 1 241 0
	st Z,r20
.LVL65:
.LVL66:
.LBE13:
	subi r18,1
	sbc r19,__zero_reg__
	brcc .L68
.LBE12:
	.loc 1 243 0
	sts state,__zero_reg__
	.loc 1 244 0
	ldi r24,lo8(66)
	rjmp .L107
.LVL67:
.L62:
.LBE9:
	.loc 1 248 0
	lds r24,state
	cpi r24,lo8(67)
	breq .+2
	rjmp .L69
	.loc 1 249 0
	lds r24,state_position
	cpi r24,lo8(2)
	brsh .+2
	rjmp .L53
.LBB14:
	.loc 1 250 0
	lds r16,state_buffer
.LVL68:
	.loc 1 251 0
	lds r29,state_buffer+1
.LVL69:
	.loc 1 253 0
	rcall display_shift_left
.LVL70:
	ldi r18,lo8(7)
	ldi r19,0
.LBB15:
.LBB16:
	.loc 1 256 0
	ldi r17,0
.LVL71:
.L72:
	movw r24,r18
	ldi r22,lo8(-4)
	ldi r23,lo8(-1)
	rcall __mulhi3
	.loc 1 255 0
	movw r30,r24
	subi r30,lo8(-(buffer+31))
	sbci r31,hi8(-(buffer+31))
	ld r20,Z
.LVL72:
	.loc 1 256 0
	movw r24,r16
	mov r0,r18
	rjmp 2f
	1:
	asr r25
	ror r24
	2:
	dec r0
	brpl 1b
	sbrc r24,0
	.loc 1 257 0
	ori r20,lo8(1)
.LVL73:
.L70:
	.loc 1 259 0
	st Z,r20
.LVL74:
.LVL75:
.LBE16:
	subi r18,1
	sbc r19,__zero_reg__
	brcc .L72
	.loc 1 254 0
	ldi r18,lo8(7)
	ldi r19,0
.LVL76:
.LBE15:
.LBB17:
.LBB18:
	.loc 1 263 0
	mov r26,r29
	ldi r27,0
.LVL77:
.L75:
	movw r24,r18
	ldi r22,lo8(-4)
	ldi r23,lo8(-1)
	rcall __mulhi3
	.loc 1 262 0
	movw r30,r24
	subi r30,lo8(-(buffer+63))
	sbci r31,hi8(-(buffer+63))
	ld r20,Z
.LVL78:
	.loc 1 263 0
	movw r24,r26
	mov r0,r18
	rjmp 2f
	1:
	asr r25
	ror r24
	2:
	dec r0
	brpl 1b
	sbrc r24,0
	.loc 1 264 0
	ori r20,lo8(1)
.LVL79:
.L73:
	.loc 1 266 0
	st Z,r20
.LVL80:
.LVL81:
.LBE18:
	subi r18,1
	sbc r19,__zero_reg__
	brcc .L75
.LBE17:
	.loc 1 268 0
	sts state,__zero_reg__
	.loc 1 269 0
	ldi r24,lo8(67)
	rjmp .L107
.LVL82:
.L69:
.LBE14:
	.loc 1 273 0
	lds r24,state
	cpi r24,lo8(68)
	brne .L76
	.loc 1 274 0
	ldi r24,0
	rcall display_clear
.LVL83:
	.loc 1 275 0
	sts state,__zero_reg__
	.loc 1 276 0
	ldi r24,lo8(68)
	rjmp .L107
.L76:
	.loc 1 279 0
	lds r24,state
	cpi r24,lo8(69)
	brne .L77
	.loc 1 280 0
	ldi r24,lo8(-1)
	rcall display_clear
.LVL84:
	.loc 1 281 0
	sts state,__zero_reg__
	.loc 1 282 0
	ldi r24,lo8(69)
	rjmp .L107
.L77:
	.loc 1 285 0
	lds r24,state
	cpi r24,lo8(70)
	brne .L78
	.loc 1 286 0
	lds r24,state_position
	cpi r24,lo8(4)
	brsh .+2
	rjmp .L53
	.loc 1 287 0
	rcall display_shift_down
.LVL85:
	.loc 1 288 0
	lds r24,state_buffer
	sts buffer,r24
	.loc 1 289 0
	lds r24,state_buffer+1
	sts buffer+1,r24
	.loc 1 290 0
	lds r24,state_buffer+2
	sts buffer+2,r24
	.loc 1 291 0
	lds r24,state_buffer+3
	sts buffer+3,r24
	.loc 1 293 0
	sts state,__zero_reg__
	.loc 1 294 0
	ldi r24,lo8(70)
	rjmp .L107
.L78:
	.loc 1 298 0
	lds r24,state
	cpi r24,lo8(71)
	brne .L79
	.loc 1 299 0
	lds r24,state_position
	cpi r24,lo8(4)
	brsh .+2
	rjmp .L53
	.loc 1 300 0
	rcall display_shift_up
.LVL86:
	.loc 1 301 0
	lds r24,state_buffer
	sts buffer+60,r24
	.loc 1 302 0
	lds r24,state_buffer+1
	sts buffer+61,r24
	.loc 1 303 0
	lds r24,state_buffer+2
	sts buffer+62,r24
	.loc 1 304 0
	lds r24,state_buffer+3
	sts buffer+63,r24
	.loc 1 306 0
	sts state,__zero_reg__
	.loc 1 307 0
	ldi r24,lo8(71)
	rjmp .L107
.L79:
.LBB19:
	.loc 1 312 0
	lds r24,state
.LVL87:
	.loc 1 313 0
	sts state,__zero_reg__
.LVL88:
.L107:
	.loc 1 314 0
	rcall uart_send
.LVL89:
	rjmp .L53
.LBE19:
	.cfi_endproc
.LFE9:
	.size	main, .-main
	.local	b.1547
	.comm	b.1547,1,1
	.local	c.1546
	.comm	c.1546,1,1
	.local	r.1545
	.comm	r.1545,1,1
.global	pin_changed
	.section .bss
	.type	pin_changed, @object
	.size	pin_changed, 1
pin_changed:
	.zero	1
.global	pin_interrupts
	.data
	.type	pin_interrupts, @object
	.size	pin_interrupts, 1
pin_interrupts:
	.byte	-1
	.comm	state_buffer,4,1
.global	state_position
	.section .bss
	.type	state_position, @object
	.size	state_position, 1
state_position:
	.zero	1
.global	state
	.type	state, @object
	.size	state, 1
state:
	.zero	1
	.comm	buffer,64,1
	.text
.Letext0:
	.file 2 "c:\\tools\\arduino-nightly\\hardware\\tools\\avr\\avr\\include\\stdint.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0x543
	.word	0x2
	.long	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.long	.LASF27
	.byte	0x1
	.long	.LASF28
	.long	.LASF29
	.long	.Ldebug_ranges0+0x30
	.long	0
	.long	0
	.long	.Ldebug_line0
	.uleb128 0x2
	.long	.LASF0
	.byte	0x2
	.byte	0x79
	.long	0x34
	.uleb128 0x3
	.byte	0x1
	.byte	0x6
	.long	.LASF2
	.uleb128 0x2
	.long	.LASF1
	.byte	0x2
	.byte	0x7a
	.long	0x46
	.uleb128 0x3
	.byte	0x1
	.byte	0x8
	.long	.LASF3
	.uleb128 0x4
	.byte	0x2
	.byte	0x5
	.string	"int"
	.uleb128 0x3
	.byte	0x2
	.byte	0x7
	.long	.LASF4
	.uleb128 0x3
	.byte	0x4
	.byte	0x5
	.long	.LASF5
	.uleb128 0x3
	.byte	0x4
	.byte	0x7
	.long	.LASF6
	.uleb128 0x3
	.byte	0x8
	.byte	0x5
	.long	.LASF7
	.uleb128 0x3
	.byte	0x8
	.byte	0x7
	.long	.LASF8
	.uleb128 0x5
	.byte	0x1
	.long	.LASF9
	.byte	0x1
	.byte	0x1b
	.byte	0x1
	.long	.LFB0
	.long	.LFE0
	.long	.LLST0
	.byte	0x1
	.long	0xa1
	.uleb128 0x6
	.long	.LASF13
	.byte	0x1
	.byte	0x1c
	.long	0x3b
	.long	.LLST1
	.byte	0
	.uleb128 0x7
	.byte	0x1
	.long	.LASF30
	.byte	0x1
	.byte	0x22
	.byte	0x1
	.long	.LFB1
	.long	.LFE1
	.long	.LLST2
	.byte	0x1
	.uleb128 0x8
	.byte	0x1
	.long	.LASF10
	.byte	0x1
	.byte	0x3d
	.byte	0x1
	.long	.LFB2
	.long	.LFE2
	.byte	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 2
	.byte	0x1
	.long	0xdf
	.uleb128 0x9
	.long	.LASF12
	.byte	0x1
	.byte	0x3d
	.long	0x3b
	.byte	0x1
	.byte	0x68
	.byte	0
	.uleb128 0x8
	.byte	0x1
	.long	.LASF11
	.byte	0x1
	.byte	0x42
	.byte	0x1
	.long	.LFB3
	.long	.LFE3
	.byte	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 2
	.byte	0x1
	.long	0x11c
	.uleb128 0xa
	.string	"v"
	.byte	0x1
	.byte	0x42
	.long	0x3b
	.byte	0x1
	.byte	0x68
	.uleb128 0xb
	.long	.LBB2
	.long	.LBE2
	.uleb128 0xc
	.string	"i"
	.byte	0x1
	.byte	0x43
	.long	0x4d
	.long	.LLST3
	.byte	0
	.byte	0
	.uleb128 0x8
	.byte	0x1
	.long	.LASF14
	.byte	0x1
	.byte	0x48
	.byte	0x1
	.long	.LFB4
	.long	.LFE4
	.byte	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 2
	.byte	0x1
	.long	0x18b
	.uleb128 0xc
	.string	"r"
	.byte	0x1
	.byte	0x49
	.long	0x3b
	.long	.LLST4
	.uleb128 0xc
	.string	"c"
	.byte	0x1
	.byte	0x49
	.long	0x3b
	.long	.LLST5
	.uleb128 0xb
	.long	.LBB3
	.long	.LBE3
	.uleb128 0xc
	.string	"t1"
	.byte	0x1
	.byte	0x4c
	.long	0x3b
	.long	.LLST6
	.uleb128 0xd
	.long	.Ldebug_ranges0+0
	.uleb128 0xc
	.string	"t2"
	.byte	0x1
	.byte	0x4e
	.long	0x3b
	.long	.LLST7
	.uleb128 0xc
	.string	"t3"
	.byte	0x1
	.byte	0x50
	.long	0x3b
	.long	.LLST8
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x8
	.byte	0x1
	.long	.LASF15
	.byte	0x1
	.byte	0x5c
	.byte	0x1
	.long	.LFB5
	.long	.LFE5
	.byte	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 2
	.byte	0x1
	.long	0x1f8
	.uleb128 0xc
	.string	"r"
	.byte	0x1
	.byte	0x5d
	.long	0x29
	.long	.LLST9
	.uleb128 0xc
	.string	"c"
	.byte	0x1
	.byte	0x5d
	.long	0x29
	.long	.LLST10
	.uleb128 0xb
	.long	.LBB6
	.long	.LBE6
	.uleb128 0xc
	.string	"t1"
	.byte	0x1
	.byte	0x60
	.long	0x3b
	.long	.LLST11
	.uleb128 0xd
	.long	.Ldebug_ranges0+0x18
	.uleb128 0xc
	.string	"t2"
	.byte	0x1
	.byte	0x62
	.long	0x3b
	.long	.LLST12
	.uleb128 0xe
	.string	"t3"
	.byte	0x1
	.byte	0x64
	.long	0x3b
	.byte	0x1
	.byte	0x6a
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x8
	.byte	0x1
	.long	.LASF16
	.byte	0x1
	.byte	0x70
	.byte	0x1
	.long	.LFB6
	.long	.LFE6
	.byte	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 2
	.byte	0x1
	.long	0x220
	.uleb128 0xc
	.string	"r"
	.byte	0x1
	.byte	0x71
	.long	0x3b
	.long	.LLST13
	.byte	0
	.uleb128 0x8
	.byte	0x1
	.long	.LASF17
	.byte	0x1
	.byte	0x7a
	.byte	0x1
	.long	.LFB7
	.long	.LFE7
	.byte	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 2
	.byte	0x1
	.long	0x248
	.uleb128 0xc
	.string	"r"
	.byte	0x1
	.byte	0x7b
	.long	0x29
	.long	.LLST14
	.byte	0
	.uleb128 0x8
	.byte	0x1
	.long	.LASF18
	.byte	0x1
	.byte	0x84
	.byte	0x1
	.long	.LFB8
	.long	.LFE8
	.byte	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 2
	.byte	0x1
	.long	0x290
	.uleb128 0xe
	.string	"r"
	.byte	0x1
	.byte	0x85
	.long	0x3b
	.byte	0x5
	.byte	0x3
	.long	r.1545
	.uleb128 0xe
	.string	"c"
	.byte	0x1
	.byte	0x86
	.long	0x3b
	.byte	0x5
	.byte	0x3
	.long	c.1546
	.uleb128 0xe
	.string	"b"
	.byte	0x1
	.byte	0x87
	.long	0x3b
	.byte	0x5
	.byte	0x3
	.long	b.1547
	.byte	0
	.uleb128 0xf
	.byte	0x1
	.long	.LASF31
	.byte	0x1
	.byte	0xa2
	.byte	0x1
	.long	0x4d
	.long	.LFB9
	.long	.LFE9
	.byte	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 2
	.byte	0x1
	.long	0x4a4
	.uleb128 0x10
	.long	.LBB9
	.long	.LBE9
	.long	0x343
	.uleb128 0xc
	.string	"a"
	.byte	0x1
	.byte	0xe1
	.long	0x3b
	.long	.LLST15
	.uleb128 0xc
	.string	"b"
	.byte	0x1
	.byte	0xe2
	.long	0x3b
	.long	.LLST16
	.uleb128 0x10
	.long	.LBB10
	.long	.LBE10
	.long	0x307
	.uleb128 0xc
	.string	"c"
	.byte	0x1
	.byte	0xe5
	.long	0x3b
	.long	.LLST17
	.uleb128 0xb
	.long	.LBB11
	.long	.LBE11
	.uleb128 0xc
	.string	"d"
	.byte	0x1
	.byte	0xe6
	.long	0x3b
	.long	.LLST18
	.byte	0
	.byte	0
	.uleb128 0x10
	.long	.LBB12
	.long	.LBE12
	.long	0x339
	.uleb128 0xc
	.string	"c"
	.byte	0x1
	.byte	0xec
	.long	0x3b
	.long	.LLST19
	.uleb128 0xb
	.long	.LBB13
	.long	.LBE13
	.uleb128 0xc
	.string	"d"
	.byte	0x1
	.byte	0xed
	.long	0x3b
	.long	.LLST20
	.byte	0
	.byte	0
	.uleb128 0x11
	.long	.LVL55
	.long	0x11c
	.byte	0
	.uleb128 0x10
	.long	.LBB14
	.long	.LBE14
	.long	0x3da
	.uleb128 0xc
	.string	"a"
	.byte	0x1
	.byte	0xfa
	.long	0x3b
	.long	.LLST21
	.uleb128 0xc
	.string	"b"
	.byte	0x1
	.byte	0xfb
	.long	0x3b
	.long	.LLST22
	.uleb128 0x10
	.long	.LBB15
	.long	.LBE15
	.long	0x39c
	.uleb128 0xc
	.string	"c"
	.byte	0x1
	.byte	0xfe
	.long	0x3b
	.long	.LLST23
	.uleb128 0xb
	.long	.LBB16
	.long	.LBE16
	.uleb128 0xc
	.string	"d"
	.byte	0x1
	.byte	0xff
	.long	0x3b
	.long	.LLST24
	.byte	0
	.byte	0
	.uleb128 0x10
	.long	.LBB17
	.long	.LBE17
	.long	0x3d0
	.uleb128 0x12
	.string	"c"
	.byte	0x1
	.word	0x105
	.long	0x3b
	.long	.LLST25
	.uleb128 0xb
	.long	.LBB18
	.long	.LBE18
	.uleb128 0x12
	.string	"d"
	.byte	0x1
	.word	0x106
	.long	0x3b
	.long	.LLST26
	.byte	0
	.byte	0
	.uleb128 0x11
	.long	.LVL70
	.long	0x18b
	.byte	0
	.uleb128 0x10
	.long	.LBB19
	.long	.LBE19
	.long	0x401
	.uleb128 0x13
	.long	.LASF19
	.byte	0x1
	.word	0x138
	.long	0x3b
	.long	.LLST27
	.uleb128 0x11
	.long	.LVL89
	.long	0xb7
	.byte	0
	.uleb128 0x14
	.long	.LVL46
	.long	0xdf
	.long	0x414
	.uleb128 0x15
	.byte	0x1
	.byte	0x68
	.byte	0x1
	.byte	0x30
	.byte	0
	.uleb128 0x14
	.long	.LVL47
	.long	0xb7
	.long	0x428
	.uleb128 0x15
	.byte	0x1
	.byte	0x68
	.byte	0x2
	.byte	0x8
	.byte	0x5a
	.byte	0
	.uleb128 0x11
	.long	.LVL48
	.long	0xb7
	.uleb128 0x14
	.long	.LVL49
	.long	0xdf
	.long	0x444
	.uleb128 0x15
	.byte	0x1
	.byte	0x68
	.byte	0x1
	.byte	0x30
	.byte	0
	.uleb128 0x11
	.long	.LVL50
	.long	0xb7
	.uleb128 0x14
	.long	.LVL51
	.long	0xdf
	.long	0x461
	.uleb128 0x15
	.byte	0x1
	.byte	0x68
	.byte	0x2
	.byte	0x9
	.byte	0xff
	.byte	0
	.uleb128 0x11
	.long	.LVL52
	.long	0x248
	.uleb128 0x14
	.long	.LVL83
	.long	0xdf
	.long	0x47d
	.uleb128 0x15
	.byte	0x1
	.byte	0x68
	.byte	0x1
	.byte	0x30
	.byte	0
	.uleb128 0x14
	.long	.LVL84
	.long	0xdf
	.long	0x491
	.uleb128 0x15
	.byte	0x1
	.byte	0x68
	.byte	0x2
	.byte	0x9
	.byte	0xff
	.byte	0
	.uleb128 0x11
	.long	.LVL85
	.long	0x220
	.uleb128 0x11
	.long	.LVL86
	.long	0x1f8
	.byte	0
	.uleb128 0x16
	.long	0x3b
	.long	0x4b4
	.uleb128 0x17
	.long	0x4b4
	.byte	0x3f
	.byte	0
	.uleb128 0x3
	.byte	0x2
	.byte	0x7
	.long	.LASF20
	.uleb128 0x18
	.long	.LASF21
	.byte	0x1
	.byte	0x14
	.long	0x4cd
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.long	buffer
	.uleb128 0x19
	.long	0x4a4
	.uleb128 0x18
	.long	.LASF22
	.byte	0x1
	.byte	0x15
	.long	0x4e4
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.long	state
	.uleb128 0x19
	.long	0x3b
	.uleb128 0x18
	.long	.LASF23
	.byte	0x1
	.byte	0x16
	.long	0x4e4
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.long	state_position
	.uleb128 0x16
	.long	0x3b
	.long	0x50b
	.uleb128 0x17
	.long	0x4b4
	.byte	0x3
	.byte	0
	.uleb128 0x18
	.long	.LASF24
	.byte	0x1
	.byte	0x17
	.long	0x51d
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.long	state_buffer
	.uleb128 0x19
	.long	0x4fb
	.uleb128 0x18
	.long	.LASF25
	.byte	0x1
	.byte	0x18
	.long	0x4e4
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.long	pin_interrupts
	.uleb128 0x18
	.long	.LASF26
	.byte	0x1
	.byte	0x19
	.long	0x4e4
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.long	pin_changed
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x55
	.uleb128 0x6
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x52
	.uleb128 0x1
	.uleb128 0x10
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0x6
	.uleb128 0x2117
	.uleb128 0xc
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0x6
	.uleb128 0x2117
	.uleb128 0xc
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0xa
	.uleb128 0x2117
	.uleb128 0xc
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x55
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0xa
	.uleb128 0x2117
	.uleb128 0xc
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x11
	.uleb128 0x4109
	.byte	0
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x31
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x12
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x14
	.uleb128 0x4109
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x15
	.uleb128 0x410a
	.byte	0
	.uleb128 0x2
	.uleb128 0xa
	.uleb128 0x2111
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x16
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x17
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x18
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x2
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x19
	.uleb128 0x35
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_loc,"",@progbits
.Ldebug_loc0:
.LLST0:
	.long	.LFB0
	.long	.LCFI0
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 2
	.long	.LCFI0
	.long	.LCFI1
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 3
	.long	.LCFI1
	.long	.LCFI2
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 4
	.long	.LCFI2
	.long	.LCFI3
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 5
	.long	.LCFI3
	.long	.LFE0
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 6
	.long	0
	.long	0
.LLST1:
	.long	.LVL0
	.long	.LVL1
	.word	0x1
	.byte	0x68
	.long	.LVL1
	.long	.LVL2
	.word	0x5
	.byte	0x3
	.long	pin_interrupts
	.long	0
	.long	0
.LLST2:
	.long	.LFB1
	.long	.LCFI4
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 2
	.long	.LCFI4
	.long	.LCFI5
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 3
	.long	.LCFI5
	.long	.LCFI6
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 4
	.long	.LCFI6
	.long	.LCFI7
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 5
	.long	.LCFI7
	.long	.LCFI8
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 6
	.long	.LCFI8
	.long	.LFE1
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 7
	.long	0
	.long	0
.LLST3:
	.long	.LVL4
	.long	.LVL5
	.word	0x2
	.byte	0x30
	.byte	0x9f
	.long	.LVL5
	.long	.LFE3
	.word	0x6
	.byte	0x62
	.byte	0x93
	.uleb128 0x1
	.byte	0x63
	.byte	0x93
	.uleb128 0x1
	.long	0
	.long	0
.LLST4:
	.long	.LVL7
	.long	.LVL8
	.word	0x2
	.byte	0x30
	.byte	0x9f
	.long	0
	.long	0
.LLST5:
	.long	.LVL9
	.long	.LVL11
	.word	0x1
	.byte	0x64
	.long	.LVL11
	.long	.LVL13
	.word	0x1
	.byte	0x62
	.long	.LVL13
	.long	.LVL19
	.word	0x1
	.byte	0x64
	.long	.LVL19
	.long	.LFE4
	.word	0x1
	.byte	0x62
	.long	0
	.long	0
.LLST6:
	.long	.LVL8
	.long	.LVL10
	.word	0x2
	.byte	0x30
	.byte	0x9f
	.long	.LVL10
	.long	.LVL16
	.word	0x1
	.byte	0x65
	.long	.LVL16
	.long	.LVL18
	.word	0x5
	.byte	0x83
	.sleb128 0
	.byte	0x37
	.byte	0x24
	.byte	0x9f
	.long	.LVL18
	.long	.LFE4
	.word	0x1
	.byte	0x65
	.long	0
	.long	0
.LLST7:
	.long	.LVL12
	.long	.LVL14
	.word	0x1
	.byte	0x63
	.long	.LVL14
	.long	.LVL19
	.word	0x1
	.byte	0x62
	.long	0
	.long	0
.LLST8:
	.long	.LVL12
	.long	.LVL18
	.word	0x5
	.byte	0x83
	.sleb128 0
	.byte	0x37
	.byte	0x24
	.byte	0x9f
	.long	.LVL18
	.long	.LVL19
	.word	0x1
	.byte	0x65
	.long	0
	.long	0
.LLST9:
	.long	.LVL20
	.long	.LVL21
	.word	0x2
	.byte	0x30
	.byte	0x9f
	.long	0
	.long	0
.LLST10:
	.long	.LVL21
	.long	.LVL22
	.word	0x2
	.byte	0x33
	.byte	0x9f
	.long	.LVL22
	.long	.LVL27
	.word	0x5
	.byte	0x33
	.byte	0x82
	.sleb128 0
	.byte	0x1c
	.byte	0x9f
	.long	.LVL27
	.long	.LVL28
	.word	0x5
	.byte	0x32
	.byte	0x82
	.sleb128 0
	.byte	0x1c
	.byte	0x9f
	.long	.LVL28
	.long	.LFE5
	.word	0x5
	.byte	0x33
	.byte	0x82
	.sleb128 0
	.byte	0x1c
	.byte	0x9f
	.long	0
	.long	0
.LLST11:
	.long	.LVL21
	.long	.LVL22
	.word	0x2
	.byte	0x30
	.byte	0x9f
	.long	.LVL22
	.long	.LVL27
	.word	0x1
	.byte	0x67
	.long	.LVL27
	.long	.LFE5
	.word	0x1
	.byte	0x6a
	.long	0
	.long	0
.LLST12:
	.long	.LVL23
	.long	.LVL25
	.word	0x1
	.byte	0x66
	.long	.LVL26
	.long	.LFE5
	.word	0x1
	.byte	0x66
	.long	0
	.long	0
.LLST13:
	.long	.LVL29
	.long	.LVL30
	.word	0x2
	.byte	0x30
	.byte	0x9f
	.long	.LVL30
	.long	.LVL31
	.word	0x1
	.byte	0x68
	.long	.LVL31
	.long	.LVL32
	.word	0x3
	.byte	0x88
	.sleb128 1
	.byte	0x9f
	.long	.LVL32
	.long	.LVL33
	.word	0x1
	.byte	0x68
	.long	.LVL33
	.long	.LVL34
	.word	0x3
	.byte	0x8
	.byte	0x3c
	.byte	0x9f
	.long	.LVL34
	.long	.LVL35
	.word	0x3
	.byte	0x8
	.byte	0x3d
	.byte	0x9f
	.long	.LVL35
	.long	.LVL36
	.word	0x3
	.byte	0x8
	.byte	0x3e
	.byte	0x9f
	.long	.LVL36
	.long	.LVL37
	.word	0x3
	.byte	0x8
	.byte	0x3f
	.byte	0x9f
	.long	.LVL37
	.long	.LFE6
	.word	0x3
	.byte	0x8
	.byte	0x40
	.byte	0x9f
	.long	0
	.long	0
.LLST14:
	.long	.LVL38
	.long	.LVL39
	.word	0x3
	.byte	0x8
	.byte	0x3b
	.byte	0x9f
	.long	.LVL39
	.long	.LVL40
	.word	0x1
	.byte	0x68
	.long	.LVL40
	.long	.LVL41
	.word	0x3
	.byte	0x88
	.sleb128 -1
	.byte	0x9f
	.long	.LVL41
	.long	.LVL42
	.word	0x2
	.byte	0x30
	.byte	0x9f
	.long	.LVL42
	.long	.LVL43
	.word	0x2
	.byte	0x31
	.byte	0x9f
	.long	.LVL43
	.long	.LVL44
	.word	0x2
	.byte	0x32
	.byte	0x9f
	.long	.LVL44
	.long	.LVL45
	.word	0x2
	.byte	0x33
	.byte	0x9f
	.long	.LVL45
	.long	.LFE7
	.word	0x2
	.byte	0x34
	.byte	0x9f
	.long	0
	.long	0
.LLST15:
	.long	.LVL53
	.long	.LVL67
	.word	0x1
	.byte	0x60
	.long	0
	.long	0
.LLST16:
	.long	.LVL54
	.long	.LVL62
	.word	0x1
	.byte	0x6d
	.long	.LVL62
	.long	.LVL67
	.word	0x1
	.byte	0x6a
	.long	0
	.long	0
.LLST17:
	.long	.LVL55
	.long	.LVL56
	.word	0x2
	.byte	0x30
	.byte	0x9f
	.long	.LVL56
	.long	.LVL59
	.word	0x5
	.byte	0x37
	.byte	0x82
	.sleb128 0
	.byte	0x1c
	.byte	0x9f
	.long	.LVL59
	.long	.LVL60
	.word	0x5
	.byte	0x38
	.byte	0x82
	.sleb128 0
	.byte	0x1c
	.byte	0x9f
	.long	.LVL60
	.long	.LVL61
	.word	0x5
	.byte	0x37
	.byte	0x82
	.sleb128 0
	.byte	0x1c
	.byte	0x9f
	.long	0
	.long	0
.LLST18:
	.long	.LVL57
	.long	.LVL62
	.word	0x1
	.byte	0x64
	.long	0
	.long	0
.LLST19:
	.long	.LVL62
	.long	.LVL65
	.word	0x5
	.byte	0x37
	.byte	0x82
	.sleb128 0
	.byte	0x1c
	.byte	0x9f
	.long	.LVL65
	.long	.LVL66
	.word	0x5
	.byte	0x38
	.byte	0x82
	.sleb128 0
	.byte	0x1c
	.byte	0x9f
	.long	.LVL66
	.long	.LVL67
	.word	0x5
	.byte	0x37
	.byte	0x82
	.sleb128 0
	.byte	0x1c
	.byte	0x9f
	.long	0
	.long	0
.LLST20:
	.long	.LVL63
	.long	.LVL67
	.word	0x1
	.byte	0x64
	.long	0
	.long	0
.LLST21:
	.long	.LVL68
	.long	.LVL82
	.word	0x1
	.byte	0x60
	.long	0
	.long	0
.LLST22:
	.long	.LVL69
	.long	.LVL77
	.word	0x1
	.byte	0x6d
	.long	.LVL77
	.long	.LVL82
	.word	0x1
	.byte	0x6a
	.long	0
	.long	0
.LLST23:
	.long	.LVL70
	.long	.LVL71
	.word	0x2
	.byte	0x30
	.byte	0x9f
	.long	.LVL71
	.long	.LVL74
	.word	0x5
	.byte	0x37
	.byte	0x82
	.sleb128 0
	.byte	0x1c
	.byte	0x9f
	.long	.LVL74
	.long	.LVL75
	.word	0x5
	.byte	0x38
	.byte	0x82
	.sleb128 0
	.byte	0x1c
	.byte	0x9f
	.long	.LVL75
	.long	.LVL76
	.word	0x5
	.byte	0x37
	.byte	0x82
	.sleb128 0
	.byte	0x1c
	.byte	0x9f
	.long	0
	.long	0
.LLST24:
	.long	.LVL72
	.long	.LVL77
	.word	0x1
	.byte	0x64
	.long	0
	.long	0
.LLST25:
	.long	.LVL77
	.long	.LVL80
	.word	0x5
	.byte	0x37
	.byte	0x82
	.sleb128 0
	.byte	0x1c
	.byte	0x9f
	.long	.LVL80
	.long	.LVL81
	.word	0x5
	.byte	0x38
	.byte	0x82
	.sleb128 0
	.byte	0x1c
	.byte	0x9f
	.long	.LVL81
	.long	.LVL82
	.word	0x5
	.byte	0x37
	.byte	0x82
	.sleb128 0
	.byte	0x1c
	.byte	0x9f
	.long	0
	.long	0
.LLST26:
	.long	.LVL78
	.long	.LVL82
	.word	0x1
	.byte	0x64
	.long	0
	.long	0
.LLST27:
	.long	.LVL87
	.long	.LVL88
	.word	0x1
	.byte	0x68
	.long	0
	.long	0
	.section	.debug_aranges,"",@progbits
	.long	0x24
	.word	0x2
	.long	.Ldebug_info0
	.byte	0x4
	.byte	0
	.word	0
	.word	0
	.long	.Ltext0
	.long	.Letext0-.Ltext0
	.long	.LFB9
	.long	.LFE9-.LFB9
	.long	0
	.long	0
	.section	.debug_ranges,"",@progbits
.Ldebug_ranges0:
	.long	.LBB4
	.long	.LBE4
	.long	.LBB5
	.long	.LBE5
	.long	0
	.long	0
	.long	.LBB7
	.long	.LBE7
	.long	.LBB8
	.long	.LBE8
	.long	0
	.long	0
	.long	.Ltext0
	.long	.Letext0
	.long	.LFB9
	.long	.LFE9
	.long	0
	.long	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF18:
	.string	"display_plotting"
.LASF13:
	.string	"buttons"
.LASF17:
	.string	"display_shift_down"
.LASF16:
	.string	"display_shift_up"
.LASF0:
	.string	"int8_t"
.LASF9:
	.string	"__vector_11"
.LASF22:
	.string	"state"
.LASF11:
	.string	"display_clear"
.LASF12:
	.string	"data"
.LASF3:
	.string	"unsigned char"
.LASF23:
	.string	"state_position"
.LASF6:
	.string	"long unsigned int"
.LASF19:
	.string	"temp"
.LASF26:
	.string	"pin_changed"
.LASF15:
	.string	"display_shift_left"
.LASF31:
	.string	"main"
.LASF29:
	.string	"D:\\Workspace\\led_display\\Device"
.LASF4:
	.string	"unsigned int"
.LASF8:
	.string	"long long unsigned int"
.LASF1:
	.string	"uint8_t"
.LASF28:
	.string	"src\\main.c"
.LASF20:
	.string	"sizetype"
.LASF25:
	.string	"pin_interrupts"
.LASF7:
	.string	"long long int"
.LASF10:
	.string	"uart_send"
.LASF27:
	.string	"GNU C 4.8.1 -mmcu=attiny2313 -msp8 -g -Os -std=gnu99"
.LASF21:
	.string	"buffer"
.LASF30:
	.string	"__vector_7"
.LASF5:
	.string	"long int"
.LASF24:
	.string	"state_buffer"
.LASF2:
	.string	"signed char"
.LASF14:
	.string	"display_shift_right"
	.ident	"GCC: (GNU) 4.8.1"
.global __do_copy_data
.global __do_clear_bss
